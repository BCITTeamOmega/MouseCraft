#include "RenderSystem.h"
#include "../Loading/TextLoader.h"

using std::string;

RenderSystem::RenderSystem() : System() {
	initShaders();
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

}

bool RenderSystem::loadShader(string shaderName) {
	static const string shaderPath = "res/shaders/";
	string vsh = TextLoader::load(shaderPath + shaderName + "vsh");
	string fsh = TextLoader::load(shaderPath + shaderName + "fsh");
	_shaders[shaderName] = Shader(vsh, fsh);
	return _shaders[shaderName].compile();
}