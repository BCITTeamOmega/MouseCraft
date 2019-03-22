#include "RenderSystem.h"
#include "../Loading/TextLoader.h"
#include "../Core/ComponentManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Renderable.h"
#include "ModelGen.h"
#include "../Loading/ImageLoader.h"
#include "RenderUtil.h"

using std::string;
using std::vector;
using glm::vec3;
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

	for (auto a : *_staticGeometries) {
		delete a;
	}

	for (auto a : *_staticTextures) {
		delete a;
	}
}

void RenderSystem::initVertexBuffers() {
	_vao = new VAO();
	_positionVBO = new VBO(3);
	_normalVBO = new VBO(3);
	_texCoordVBO = new VBO(2);
	_ebo = new EBO();

	_vao->setBuffer(0, *_positionVBO);
	_vao->setBuffer(1, *_normalVBO);
	_vao->setBuffer(2, *_texCoordVBO);
	_vao->setElementBuffer(*_ebo);
}

void RenderSystem::initTextures() {
	_staticTextures = new vector<Image*>();
	_textures = new GLTextureArray(1024, 1024, 50, 2, GL_SRGB8_ALPHA8);
}

void RenderSystem::initRenderBuffers() {
	_albedoBuffer = new GLTexture();
	_normalBuffer = new GLTexture();
	_positionBuffer = new GLTexture();

	vector<GLTexture*> buffers = { _albedoBuffer, _normalBuffer, _positionBuffer };
	_fbo = new FBO(1280, 720, buffers);

	vector<GLTexture*> noBuffers = {};
	_resizeInFBO = new FBO(1024, 1024, noBuffers);
	_resizeOutFBO = new FBO(1024, 1024, noBuffers);
}

void RenderSystem::setWindow(Window* window) {
	_window = window;
}

void RenderSystem::initShaders() {
	loadShader("gbuffer");
	loadShader("lighting");
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
		gBufferPass();
		lightingPass();
	}
}

void RenderSystem::gBufferPass() {
	float windowRatio = (float)_window->getWidth() / _window->getHeight();
	Transform viewTransform = _camera->getTransform();
	mat4 view = inverse(viewTransform.getWorldTransformation());

	float fov = _camera->getFOV();
	float closeClip = _camera->getCloseClip();
	float farClip = _camera->getFarClip();

	mat4 projection = perspective(fov, windowRatio, closeClip, farClip);
	mat4 vp = projection * view;

	// Set up all of the vertex data for all objects to be rendered
	CombinedGeometry* masterGeometry = combineGeometries(*_renderingList);
	_positionVBO->buffer(masterGeometry->getVertexData());
	_normalVBO->buffer(masterGeometry->getNormalData());
	_texCoordVBO->buffer(masterGeometry->getTexCoordData());
	_ebo->buffer(masterGeometry->getIndices());

	setShader(_shaders["gbuffer"]);
	_fbo->bind();

	int index = 0;
	for (RenderData render : *_renderingList) {
		int texID = getTexture(render.getModel()->getTexture());
		vec3 color = convertColor(render.getColor());
		mat4 model = render.getTransform();

		mat4 mvp = vp * model;
		mat4 invMVP = transpose(inverse(view * model));

		/*
		_positionVBO->buffer(g->getVertexData());
		_normalVBO->buffer(g->getNormalData());
		_texCoordVBO->buffer(g->getTexCoordData());
		_ebo->buffer(g->getIndices());
		*/
		_textures->bind(GL_TEXTURE0);

		_shader->setUniformVec3("color", color);
		_shader->setUniformMatrix("transform", mvp);
		_shader->setUniformMatrix("invTransform", invMVP);

		_shader->setUniformInt("textureID", texID);
		_shader->setUniformTexture("albedoTex", 0);

		int start = masterGeometry->getIndexStart(index);
		int size = masterGeometry->getIndexEnd(index) - start + 1;
		glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, (void *)(start * sizeof(GLuint)));

		index++;
	}
	_fbo->unbind();
	delete masterGeometry;
}

CombinedGeometry* RenderSystem::combineGeometries(vector<RenderData>& data) {
	CombinedGeometry* result = new CombinedGeometry();
	std::vector<GLfloat> vert;
	std::vector<GLfloat> texCoord;
	std::vector<GLfloat> normal;
	std::vector<GLuint> indices;
	for (RenderData render : data) {
		Geometry* g = render.getModel()->getGeometry();

		int startVertex = vert.size();
		int startIndex = indices.size();
		result->getIndexIndices().push_back(startIndex);

		vert.insert(vert.end(), g->getVertexData().begin(), g->getVertexData().end());
		texCoord.insert(texCoord.end(), g->getTexCoordData().begin(), g->getTexCoordData().end());
		normal.insert(normal.end(), g->getNormalData().begin(), g->getNormalData().end());

		std::vector<GLuint> objectIndices = g->getIndices();
		int vertexOffset = startVertex / 3;
		for (GLuint i : objectIndices) {
			indices.push_back(i + vertexOffset);
		}
	}
	result->setVertexData(vert);
	result->setTexCoordData(texCoord);
	result->setNormalData(normal);
	result->setIndices(indices);
	return result;
}

void RenderSystem::lightingPass() {
	Geometry* quad = _screenQuad->getGeometry();
	setShader(_shaders["lighting"]);

	_albedoBuffer->bind(GL_TEXTURE0);
	_normalBuffer->bind(GL_TEXTURE1);
	_positionBuffer->bind(GL_TEXTURE2);

	_shader->setUniformTexture("albedoTex", 0);
	_shader->setUniformTexture("normalTex", 1);
	_shader->setUniformTexture("positionTex", 2);

	_positionVBO->buffer(quad->getVertexData());
	_normalVBO->buffer(quad->getNormalData());
	_texCoordVBO->buffer(quad->getTexCoordData());
	_ebo->buffer(quad->getIndices());
	glDrawElements(GL_TRIANGLES, quad->getIndices().size(), GL_UNSIGNED_INT, 0);
}

void RenderSystem::swapLists() {
	swap(_renderingList, _accumulatingList);
	_accumulatingList->clear();
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

	tmpTex.setImage(*input, false, GL_RGBA8, GL_UNSIGNED_BYTE);
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

vec3 RenderSystem::convertColor(Color c) {
	return vec3(c.getRed(), c.getGreen(), c.getBlue());
}

void RenderSystem::accumulateList() {
	auto renderables = ComponentManager<Renderable>::Instance().All();
	auto cameras = ComponentManager<Camera>::Instance().All();
	for (Renderable* r : renderables) {
		Transform t = r->getTransform();
		_accumulatingList->push_back(
			RenderData(
				r->getModel(),
				t.getWorldTransformation(),
				r->getColor()
			)
		);
	}
	for (Camera* c : cameras) {
		// Todo: Support for multiple cameras
		// For now we will just take the first camera and leave;
		_camera = c;
		break;
	}
}