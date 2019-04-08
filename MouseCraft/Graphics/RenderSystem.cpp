#include "RenderSystem.h"
#include "../Loading/TextLoader.h"
#include "../Core/ComponentManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Renderable.h"
#include "../UI/UIComponent.h"
#include "ModelGen.h"
#include "../Loading/ImageLoader.h"
#include "RenderUtil.h"

#define TEXTURE_SIZE 2048

using std::string;
using std::vector;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::perspective;
using glm::translate;
using glm::identity;
using glm::inverse;
using glm::transpose;

RenderSystem::RenderSystem() : System() {
	initShaders();
	initVertexBuffers();
	initTextures();
	initRenderBuffers();

	_screenQuad = ModelGen::makeQuad(ModelGen::Axis::Z, 2, 2);

	_renderingList = new vector<RenderData>();
	_accumulatingList = new vector<RenderData>();

	_uiRenderingList = new vector<RenderData>();
	_uiAccumulatingList = new vector<RenderData>();

	_lightRenderingList = new vector<LightData>();
	_lightRenderingList->reserve(MAX_LIGHTS);
	_lightAccumulatingList = new vector<LightData>();
	_lightAccumulatingList->reserve(MAX_LIGHTS);

	_masterGeometry = new CombinedGeometry();

	glEnable(GL_FRAMEBUFFER_SRGB);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	profiler.InitializeTimers(1);
	profiler.LogOutput("Rendering.log");	// optional

	loadTexture("res/models/test/blank.bmp");

	_vao->bind();
}

RenderSystem::~RenderSystem() {
	delete _vao;
	delete _positionVBO;
	delete _ebo;
	delete _textures;
	delete _fbo;
	delete _screenQuad;
	delete _masterGeometry;
	delete _ubo;

	for (auto a : *_staticGeometries) {
		delete a;
	}

	for (auto a : *_staticTextures) {
		delete a;
	}
}

void RenderSystem::initVertexBuffers() {
	_vao = new VertexArrayObject();
	_positionVBO = new VertexBufferObject(3);
	_normalVBO = new VertexBufferObject(3);
	_texCoordVBO = new VertexBufferObject(2);
	_ebo = new ElementBufferObject();

	_vao->setBuffer(0, *_positionVBO);
	_vao->setBuffer(1, *_normalVBO);
	_vao->setBuffer(2, *_texCoordVBO);

	_vao->setElementBuffer(*_ebo);
}

void RenderSystem::initTextures() {
	_staticTextures = new vector<Image*>();
	_textures = new GLTextureArray(TEXTURE_SIZE, TEXTURE_SIZE, 50, 5, GL_SRGB8_ALPHA8);
}

void RenderSystem::initRenderBuffers() {
	_albedoBuffer = new GLTexture();
	_normalBuffer = new GLTexture();
	_positionBuffer = new GLTexture();

	vector<GLTexture*> buffers = { _albedoBuffer, _normalBuffer, _positionBuffer };
	_fbo = new FrameBufferObject(1280, 720, buffers);

	vector<GLTexture*> noBuffers = {};
	_resizeInFBO = new FrameBufferObject(TEXTURE_SIZE, TEXTURE_SIZE, noBuffers);
	_resizeOutFBO = new FrameBufferObject(TEXTURE_SIZE, TEXTURE_SIZE, noBuffers);

	_ubo = new UniformBufferObject();
}

void RenderSystem::setWindow(Window* window) {
	_window = window;
}

void RenderSystem::initShaders() {
	loadShader("gbuffer");
	loadShader("lighting");
	loadShader("ui");
}

void RenderSystem::setShader(Shader& shader) {
	glUseProgram(shader.getProgram());
	_shader = &shader;
}

void RenderSystem::clearShader() {
	glUseProgram(0);
	_shader = nullptr;
}

void RenderSystem::Update(float dt) {
	profiler.StartTimer(0);

	clearBuffers();
	renderScene();

	accumulateList();
	swapLists();

	profiler.StopTimer(0);
	profiler.FrameFinish();
}

void RenderSystem::clearBuffers() {
	_fbo->bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_fbo->unbind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderSystem::renderScene() {
	if (_camera != nullptr) {
		float windowRatio = (float)_window->getWidth() / _window->getHeight();
		Transform viewTransform = _camera->getTransform();
		mat4 view = inverse(viewTransform.getWorldTransformation());

		float fov = _camera->getFOV();
		float closeClip = _camera->getCloseClip();
		float farClip = _camera->getFarClip();

		mat4 projection = perspective(fov, windowRatio, closeClip, farClip);

		gBufferPass(view, projection);
		makeLightsViewSpace(view);
		lightingPass();
	}
	uiPass();
}

void RenderSystem::gBufferPass(glm::mat4 viewMatrix, glm::mat4 projectionMatrix) {

	// Set up all of the vertex data for all objects to be rendered
	combineMasterGeometry(*_renderingList);
	_positionVBO->buffer(_masterGeometry->getVertexData());
	_normalVBO->buffer(_masterGeometry->getNormalData());
	_texCoordVBO->buffer(_masterGeometry->getTexCoordData());
	_ebo->buffer(_masterGeometry->getIndices());

	setShader(_shaders["gbuffer"]);
	_fbo->bind();

	int index = 0;
	for (RenderData render : *_renderingList) {
		int texID = getTexture(render.getModel()->getTexture());
		vec4 color = convertColor(render.getColor());
		mat4 model = render.getTransform();

		mat4 mv =  viewMatrix * model;
		mat4 mvp = projectionMatrix * mv;
		mat4 invMVP = transpose(inverse(viewMatrix * model));

		int loc = _masterGeometry->getVertexStart(index);
		_vao->setBuffer(0, *_positionVBO, loc * 3 * sizeof(GLfloat));
		_vao->setBuffer(1, *_normalVBO, loc * 3 * sizeof(GLfloat));
		_vao->setBuffer(2, *_texCoordVBO, loc * 2 * sizeof(GLfloat));

		_textures->bind(GL_TEXTURE0);

		_shader->setUniformVec3("color", color);
		_shader->setUniformMatrix("transform", mvp);
		_shader->setUniformMatrix("transformNoPerspective", mv);
		_shader->setUniformMatrix("invTransform", invMVP);

		_shader->setUniformInt("textureID", texID);
		_shader->setUniformTexture("albedoTex", 0);

		int start = _masterGeometry->getIndexStart(index);
		int size = _masterGeometry->getIndexEnd(index) - start + 1;
		glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, (void *)(start * sizeof(GLuint)));

		index++;
	}
	_fbo->unbind();
}

void RenderSystem::combineMasterGeometry(vector<RenderData>& data) {
	_masterGeometry->clear();
	std::vector<GLfloat>& vert = _masterGeometry->getVertexData();
	std::vector<GLfloat>& texCoord = _masterGeometry->getTexCoordData();
	std::vector<GLfloat>& normal = _masterGeometry->getNormalData();
	std::vector<GLuint>& indices = _masterGeometry->getIndices();
	for (RenderData render : data) {
		Geometry* g = render.getModel()->getGeometry();

		int startVertex = vert.size() / 3;
		int startIndex = indices.size();
		_masterGeometry->getIndexIndices().push_back(startIndex);
		_masterGeometry->getVertexIndices().push_back(startVertex);

		vert.insert(vert.end(), g->getVertexData().begin(), g->getVertexData().end());
		texCoord.insert(texCoord.end(), g->getTexCoordData().begin(), g->getTexCoordData().end());
		normal.insert(normal.end(), g->getNormalData().begin(), g->getNormalData().end());
		indices.insert(indices.end(), g->getIndices().begin(), g->getIndices().end());
	}
	_masterGeometry->setVertexData(vert);
	_masterGeometry->setTexCoordData(texCoord);
	_masterGeometry->setNormalData(normal);
	_masterGeometry->setIndices(indices);
}

void RenderSystem::makeLightsViewSpace(glm::mat4 viewMatrix) {
	glm::mat4 normalMatrix = transpose(inverse(viewMatrix));
	for (LightData& l : *_lightRenderingList) {
		l.position = viewMatrix * l.position;
		l.direction = normalMatrix * l.direction;
	}
}

void RenderSystem::lightingPass() {
	_vao->setBuffer(0, *_positionVBO);
	_vao->setBuffer(1, *_normalVBO);
	_vao->setBuffer(2, *_texCoordVBO);

	Geometry* quad = _screenQuad->getGeometry();
	setShader(_shaders["lighting"]);

	_albedoBuffer->bind(GL_TEXTURE0);
	_normalBuffer->bind(GL_TEXTURE1);
	_positionBuffer->bind(GL_TEXTURE2);

	_shader->setUniformTexture("albedoTex", 0);
	_shader->setUniformTexture("normalTex", 1);
	_shader->setUniformTexture("positionTex", 2);

	_shader->setUniformInt("numLights", _lightRenderingList->size());
	_shader->setUniformVec3("ambientColor", vec3(0.06f, 0.17f, 0.27f));

	_ubo->bind(0);
	_shader->setBindingPoint("Lights", 0);
	_ubo->buffer((*_lightRenderingList)[0], MAX_LIGHTS);

	_positionVBO->buffer(quad->getVertexData());
	_normalVBO->buffer(quad->getNormalData());
	_texCoordVBO->buffer(quad->getTexCoordData());
	_ebo->buffer(quad->getIndices());
	glDrawElements(GL_TRIANGLES, quad->getIndices().size(), GL_UNSIGNED_INT, 0);
	_ubo->unbind(0);
}

void RenderSystem::uiPass() {
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	float windowRatio = (float)_window->getWidth() / _window->getHeight();

	setShader(_shaders["ui"]);

	int index = 0;
	for (RenderData render : *_uiRenderingList) {
		int texID = getTexture(render.getModel()->getTexture());
		Geometry* g = render.getModel()->getGeometry();
		vec4 color = convertColor(render.getColor());
		mat4 model = render.getTransform();

		mat4 matrix = 2.0f * glm::translate(model, vec3(-0.5, -0.5, 0));
		matrix[3][3] = 1.0f; // To fix the scaling to be what we want

		int loc = 0;
		_vao->setBuffer(0, *_positionVBO, loc * 3 * sizeof(GLfloat));
		_vao->setBuffer(1, *_normalVBO, loc * 3 * sizeof(GLfloat));
		_vao->setBuffer(2, *_texCoordVBO, loc * 2 * sizeof(GLfloat));
		
		_positionVBO->buffer(g->getVertexData());
		_normalVBO->buffer(g->getNormalData());
		_texCoordVBO->buffer(g->getTexCoordData());
		_ebo->buffer(g->getIndices());

		_textures->bind(GL_TEXTURE0);

		_shader->setUniformVec4("color", color);
		_shader->setUniformMatrix("transform", matrix);

		_shader->setUniformInt("textureID", texID);
		_shader->setUniformTexture("tex", 0);

		int start = 0;
		int size = g->getIndices().size();
		glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, (void *)(start * sizeof(GLuint)));

		index++;
	}
	glDisable(GL_BLEND);
}

void RenderSystem::swapLists() {
	swap(_renderingList, _accumulatingList);
	swap(_uiRenderingList, _uiAccumulatingList);
	// Sort the UI rendering list from back to front
	std::sort(_uiRenderingList->begin(), _uiRenderingList->end(), [](RenderData a, RenderData b) {
		return a.getTransform()[3][2] > b.getTransform()[3][2];
	});
	swap(_lightRenderingList, _lightAccumulatingList);
	_accumulatingList->clear();
	_uiAccumulatingList->clear();
	_lightAccumulatingList->clear();
}

int RenderSystem::getTexture(string* path) {
	if (path == nullptr) {
		return 0; // Use the default texture
	}
	return (_texturePathToID.find(*path) != _texturePathToID.end()) ?
		_texturePathToID[*path] : loadTexture(*path);
}

int RenderSystem::loadTexture(const string& path, bool scale) {
	Image* img = ImageLoader::loadImage(path);
	if (scale) {
		Image* tmp = img;
		img = scaleImage(tmp, _textures->getWidth(), _textures->getHeight());
		delete tmp;
	}
	int id = _staticTextures->size();
	_texturePathToID[path] = id;
	_staticTextures->push_back(img);
	_textures->setImage(id, *img, GL_UNSIGNED_BYTE);
	_textures->genMipmaps();
	return id;
}

Image* RenderSystem::scaleImage(Image* input, int width, int height) {
	GLTexture tmpTex, tmpTex2;
	Image* tmpImg = new Image(NULL, width, height);

	GLuint imageFormat = GL_RGBA8;
	switch (input->getChannels()) {
	case 1:
		imageFormat = GL_R8;
		break;
	case 2:
		imageFormat = GL_RG8;
		break;
	case 3:
		imageFormat = GL_RGB8;
		break;
	}
	tmpTex.setImage(*input, false, imageFormat, GL_UNSIGNED_BYTE);
	tmpTex2.setImage(*tmpImg, false, GL_RGBA8, GL_UNSIGNED_BYTE);
	_resizeInFBO->buffer(GL_COLOR_ATTACHMENT0, tmpTex);
	_resizeOutFBO->buffer(GL_COLOR_ATTACHMENT0, tmpTex2);
	_resizeInFBO->bind(GL_READ_FRAMEBUFFER);
	_resizeOutFBO->bind(GL_DRAW_FRAMEBUFFER);

	glBlitFramebuffer(0, 0, input->getWidth(), input->getHeight(), 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	RenderUtil::checkGLError("glBlitFramebuffer");

	_resizeOutFBO->bind(GL_READ_FRAMEBUFFER);
	delete tmpImg;
	std::vector<unsigned char>* data = new std::vector<unsigned char>(width * height * 4);
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, &(*data)[0]);
	RenderUtil::checkGLError("glReadPixels");
	tmpImg = new Image(&(*data)[0], width, height, 4);
	glClear(GL_DEPTH_BUFFER_BIT);
	_resizeOutFBO->unbind(GL_READ_FRAMEBUFFER);
	_resizeOutFBO->unbind(GL_DRAW_FRAMEBUFFER);

	return tmpImg;
}

bool RenderSystem::loadShader(string shaderName) {
	static const string shaderPath = "res/shaders/";
	string vsh = TextLoader::load(shaderPath + shaderName + ".vsh");
	string fsh = TextLoader::load(shaderPath + shaderName + ".fsh");
	_shaders[shaderName] = Shader(shaderName, vsh, fsh);
	return _shaders[shaderName].compile();
}

vec4 RenderSystem::convertColor(Color c) {
	return vec4(c.getRed(), c.getGreen(), c.getBlue(), c.getAlpha());
}

void RenderSystem::accumulateList() {
	auto renderables = ComponentManager<Renderable>::Instance().All();
	auto uiRenderables = ComponentManager<UIComponent>::Instance().All();
	auto cameras = ComponentManager<Camera>::Instance().All();
	auto lights = ComponentManager<Light>::Instance().All();
	for (Renderable* r : renderables) {
		if (!r->GetActive()) continue;
		Transform t = r->getTransform();
		_accumulatingList->push_back(
			RenderData(
				r->getModel(),
				t.getWorldTransformation(),
				r->getColor()
			)
		);
	}
	for (UIComponent* r : uiRenderables) {
		Transform t = r->GetEntity()->transform;
		vector<Model*> models = r->models;
		Color c = r->color;
		for (Model* m : models) {
			_uiAccumulatingList->push_back(
				RenderData(
					m,
					t.getWorldTransformation(),
					c
				)
			);
		}
	}
	for (Camera* c : cameras) {
		// Todo: Support for multiple cameras
		// For now we will just take the first camera and leave;
		_camera = c;
		break;
	}
	for (int i = 0; i < lights.size() && i < MAX_LIGHTS; i++) {
		Light* l = lights[i];
		Entity* e = l->GetEntity();

		LightData internalLight; // The light used by the rendering system
		internalLight.type = l->getType();
		internalLight.blank1 = 0;
		internalLight.blank2 = 0;
		internalLight.blank3 = 0;
		internalLight.color = convertColor(l->getColor());
		internalLight.attenuation = glm::vec4(
			l->getConstantAttenuation(),
			l->getLinearAttenuation(),
			l->getQuadraticAttenuation(),
			0.0f
		);
		internalLight.position = glm::vec4(e->transform.getWorldPosition(), 0.0f);
		internalLight.direction = glm::vec4(e->transform.getWorldForward(), 0.0f);

		_lightAccumulatingList->push_back(internalLight);
	}
}