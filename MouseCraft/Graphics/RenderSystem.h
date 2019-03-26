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
#include "CombinedGeometry.h"
#include "Camera.h"
#include "GLTexture.h"
#include "GLTextureArray.h"
#include "../Util/CpuProfiler.h"

class RenderSystem : public System {
public:
	RenderSystem();
	~RenderSystem();
	void initVertexBuffers();
	void initTextures();
	void initRenderBuffers();
	void setWindow(Window* window);
	void Update(float dt) override;
	void swapLists();
private:
	bool loadShader(std::string shaderName);
	void initShaders();
	void setShader(Shader& s);
	void clearShader();
	void accumulateList();
	void clearBuffers();
	void renderScene();
	void gBufferPass();
	void lightingPass();
	void uiPass();
	void combineMasterGeometry(std::vector<RenderData>& data);
	int getTexture(std::string* path);
	int loadTexture(const std::string& path, bool scaleImage = true);
	Image* scaleImage(Image* input, int width, int height);
	glm::vec3 convertColor(Color c);

	Window* _window;
	std::vector<RenderData>* _renderingList;
	std::vector<RenderData>* _accumulatingList;
	std::map<std::string, Shader> _shaders;
	Shader* _shader;


	std::vector<RenderData>* _uiRenderingList;
	std::vector<RenderData>* _uiAccumulatingList;

	VAO* _vao;
	VBO* _positionVBO;
	VBO* _normalVBO;
	VBO* _texCoordVBO;
	EBO* _ebo;
	FBO* _fbo;
	Camera* _camera;

	FBO* _resizeInFBO;
	FBO* _resizeOutFBO;
	
	GLTextureArray* _textures;
	GLTexture* _albedoBuffer;
	GLTexture* _normalBuffer;
	GLTexture* _positionBuffer;

	CombinedGeometry* _masterGeometry;

	Model* _screenQuad;
	CpuProfiler profiler;

	std::map<std::string, int> _texturePathToID;

	std::vector<Geometry*>* _staticGeometries;
	std::vector<Image*>* _staticTextures;
};