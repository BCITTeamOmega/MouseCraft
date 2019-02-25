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

RenderSystem::RenderSystem() : System() {
	initShaders();
	glClearColor(0.3f, 0.75f, 1.0f, 1.0f);

	_renderingList = new vector<RenderData>();
	_accumulatingList = new vector<RenderData>();
	_vao = new VAO();
	_positionVBO = new VBO(3);
	_normalVBO = new VBO(3);
	_texCoordVBO = new VBO(2);
	_ebo = new EBO();
	_vao->setBuffer(0, *_positionVBO);
	_vao->setBuffer(1, *_normalVBO);
	_vao->setBuffer(2, *_texCoordVBO);
	_vao->setElementBuffer(*_ebo);

	_texture = new GLTexture();
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float windowRatio = (float)_window->getWidth() / _window->getHeight();

	_vao->bind();
	setShader(_shaders["gbuffer"]);

	if (_camera != nullptr) {
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

			if (tex != nullptr) {
				_texture->setImage(*tex);
			}

			_positionVBO->buffer(g->getVertexData());
			_normalVBO->buffer(g->getNormalData());
			_texCoordVBO->buffer(g->getTexCoordData());
			_ebo->buffer(g->getIndices());

			_shader->setUniformVec3("color", color);
			_shader->setUniformMatrix("transform", mvp);

			glDrawElements(GL_TRIANGLES, g->getIndices().size(), GL_UNSIGNED_INT, 0);
		}
	}
	accumulateList();
	swapLists();
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