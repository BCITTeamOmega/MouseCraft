#include "RenderSystem.h"
#include "../Loading/TextLoader.h"
#include "../Core/ComponentManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Renderable.h"

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
	glEnable(GL_FRAMEBUFFER_SRGB);

	_renderingList = new vector<RenderData>();
	_accumulatingList = new vector<RenderData>();
	_vao = new VAO();
	_positionVBO = new VBO(3);
	_normalVBO = new VBO(3);
	_texCoordVBO = new VBO(2);
	_fbo = new FBO();
	_ebo = new EBO();
	_vao->setBuffer(0, *_positionVBO);
	_vao->setBuffer(1, *_normalVBO);
	_vao->setBuffer(2, *_texCoordVBO);
	_vao->setElementBuffer(*_ebo);

	_texture = new GLTexture();
	_albedoBuffer = new GLTexture();
	_normalBuffer = new GLTexture();
	_positionBuffer = new GLTexture();

	profiler.InitializeTimers(1);
	profiler.LogOutput("Rendering.log");	// optional
}

RenderSystem::~RenderSystem() {
	delete _vao;
	delete _positionVBO;
	delete _ebo;
	delete _texture;
}

void RenderSystem::setWindow(Window* window) {
	_window = window;
}

void RenderSystem::initShaders() {
	loadShader("gbuffer");
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

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float windowRatio = (float)_window->getWidth() / _window->getHeight();

	_vao->bind();
	setShader(_shaders["gbuffer"]);

	// First Pass
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	setOutBuffers({_albedoBuffer, _normalBuffer, _positionBuffer});

	if (_camera != nullptr) {
		_fbo->bind();
		Transform viewTransform = _camera->getTransform();
		mat4 view = inverse(viewTransform.getWorldTransformation());

		float fov = _camera->getFOV();
		float closeClip = _camera->getCloseClip();
		float farClip = _camera->getFarClip();

		mat4 projection = perspective(fov, windowRatio, closeClip, farClip);

		mat4 vp = projection * view;

		for (RenderData render : *_renderingList) {
			Geometry* g = render.getModel()->getGeometry();
			Image* tex = render.getModel()->getTexture();
			vec3 color = convertColor(render.getColor());
			mat4 model = render.getTransform();

			mat4 mvp = vp * model;
			mat4 invMVP = transpose(inverse(view * model));

			if (tex != nullptr) {
				// _texture->setImage(*tex, true, GL_SRGB_ALPHA);
				_texture->setImage(*tex);
			}

			_positionVBO->buffer(g->getVertexData());
			_normalVBO->buffer(g->getNormalData());
			_texCoordVBO->buffer(g->getTexCoordData());
			_ebo->buffer(g->getIndices());

			_shader->setUniformVec3("color", color);
			_shader->setUniformMatrix("transform", mvp);
			_shader->setUniformMatrix("invTransform", invMVP);

			glDrawElements(GL_TRIANGLES, g->getIndices().size(), GL_UNSIGNED_INT, 0);
		}
	}

	glClearColor(0.3f, 0.75f, 1.0f, 1.0f);

	accumulateList();
	swapLists();

	profiler.StopTimer(0);
	profiler.FrameFinish();
}

void RenderSystem::swapLists() {
	swap(_renderingList, _accumulatingList);
	_accumulatingList->clear();
}

bool RenderSystem::loadShader(string shaderName) {
	static const string shaderPath = "res/shaders/";
	string vsh = TextLoader::load(shaderPath + shaderName + ".vsh");
	string fsh = TextLoader::load(shaderPath + shaderName + ".fsh");
	_shaders[shaderName] = Shader(vsh, fsh);
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
void RenderSystem::setOutBuffers(std::vector<GLTexture*> buffers) {
	int w = _window->getWidth();
	int h = _window->getHeight();
	Image* img = new Image(NULL, w, h); // Temp image so we can use GLtextures
	int num = 0;
	vector<GLuint> attachments;
	for (GLTexture* b : buffers) {
		b->setImage(*img, false, GL_RGBA16F);
		GLuint attachment = 0;
		switch (num) {
		case 0:
			attachment = GL_COLOR_ATTACHMENT0;
			break;
		case 1:
			attachment = GL_COLOR_ATTACHMENT1;
			break;
		case 2:
			attachment = GL_COLOR_ATTACHMENT2;
			break;
		case 3:
			attachment = GL_COLOR_ATTACHMENT3;
			break;
		case 4:
			attachment = GL_COLOR_ATTACHMENT4;
			break;
		}
		attachments.push_back(attachment);
		_fbo->buffer(attachment, *b);
		num++;
	}
	glDrawBuffers(attachments.size(), &attachments[0]);
	delete img;
}