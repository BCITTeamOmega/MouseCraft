#pragma once
#include "../Core/System.h"
#include "Window.h"
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <map>
#include "Shader.h"
#include "RenderData.h"
#include "BufferObjects/VAO.h"
#include "BufferObjects/VBO.h"
#include "BufferObjects/EBO.h"
#include "BufferObjects/FBO.h"
#include "Camera.h"
#include "GLTexture.h"
#include "../Util/CpuProfiler.h"

class RenderSystem : public System {
public:
	RenderSystem();
	~RenderSystem();
	void setWindow(Window* window);
	void Update(float dt) override;
	void swapLists();
private:
	bool loadShader(std::string shaderName);
	void initShaders();
	void setShader(Shader& s);
	void clearShader();
	void accumulateList();
	void setOutBuffers(std::vector<GLTexture*> buffers);
	glm::vec3 convertColor(Color c);

	Window* _window;
	std::vector<RenderData>* _renderingList;
	std::vector<RenderData>* _accumulatingList;
	std::map<std::string, Shader> _shaders;
	Shader* _shader;

	VAO* _vao;
	VBO* _positionVBO;
	VBO* _normalVBO;
	VBO* _texCoordVBO;
	EBO* _ebo;
	FBO* _fbo;
	Camera* _camera;
	
	GLTexture* _texture;
	GLTexture* _albedoBuffer;
	GLTexture* _normalBuffer;
	GLTexture* _positionBuffer;

	CpuProfiler profiler;
};