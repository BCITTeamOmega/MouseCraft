#pragma once
#include "../Core/System.h"
#include "Window.h"
#include <string>
#include <vector>
#include <map>
#include "Shader.h"
#include "RenderData.h"

class RenderSystem : System {
public:
	RenderSystem();
	void setWindow(Window* window);
	void Update(float dt) override;
private:
	bool loadShader(std::string shaderName);
	void initShaders();
	void setShader(Shader& s);
	void clearShader();

	Window* _window;
	std::vector<RenderData>* _renderingList;
	std::vector<RenderData>* _accumulatingList;
	std::map<std::string, Shader> _shaders;
	Shader* _shader;
};