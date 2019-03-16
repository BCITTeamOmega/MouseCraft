#include "RenderSystem.h"
#include "../Loading/TextLoader.h"
#include "../Core/ComponentManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Renderable.h"
#include "ModelGen.h"

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

	_screenQuad = ModelGen::makeQuad(ModelGen::Axis::Z, 2, 2);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	std::vector<GLTexture*> buffers = { _albedoBuffer, _normalBuffer, _positionBuffer };
	setOutBuffers(buffers);

	profiler.InitializeTimers(1);
	profiler.LogOutput("Rendering.log");	// optional
}

RenderSystem::~RenderSystem() {
	delete _vao;
	delete _positionVBO;
	delete _ebo;
	delete _texture;
	delete _fbo;
	delete _screenQuad;
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
	_fbo->bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_fbo->unbind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float windowRatio = (float)_window->getWidth() / _window->getHeight();

	_vao->bind();
	setShader(_shaders["gbuffer"]);

	if (_camera != nullptr) {
		// G-Buffer Pass

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
				_texture->setImage(*tex, true, GL_SRGB_ALPHA, GL_UNSIGNED_BYTE);
			}

			_positionVBO->buffer(g->getVertexData());
			_normalVBO->buffer(g->getNormalData());
			_texCoordVBO->buffer(g->getTexCoordData());
			_ebo->buffer(g->getIndices());
			_texture->bind(GL_TEXTURE0);

			_shader->setUniformVec3("color", color);
			_shader->setUniformMatrix("transform", mvp);
			_shader->setUniformMatrix("invTransform", invMVP);
			_shader->setUniformTexture("texture0", 0);

			glDrawElements(GL_TRIANGLES, g->getIndices().size(), GL_UNSIGNED_INT, 0);
		}

		_fbo->unbind();

		// Lighting Pass
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
void RenderSystem::setOutBuffers(std::vector<GLTexture*>& buffers) {
	int w = 1280;
	int h = 720;
	Image* img = new Image(NULL, w, h); // Temp image so we can use GLtextures
	int num = 0;
	vector<GLuint> attachments;
	for (GLTexture* b : buffers) {
		b->setImage(*img, false, GL_RGBA16F, GL_FLOAT);
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
	_fbo->bind();
	auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer not complete: " << fboStatus << std::endl;
	}
	glDrawBuffers(attachments.size(), &attachments[0]);
	delete img;
}