#pragma once
#include "../Core/System.h"
#include "Window.h"
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <map>
#include "Shader.h"
#include "RenderData.h"
#include "BufferObjects/VertexArrayObject.h"
#include "BufferObjects/VertexBufferObject.h"
#include "BufferObjects/ElementBufferObject.h"
#include "BufferObjects/FrameBufferObject.h"
#include "BufferObjects/UniformBufferObject.h"
#include "CombinedGeometry.h"
#include "Camera.h"
#include "GLTexture.h"
#include "GLTextureArray.h"
#include "Light.h"
#include "../Util/CpuProfiler.h"
#include "TextureInfo.h"

#define MAX_LIGHTS 50

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
private:						        // Data Alignment
	struct LightData {        // (Total: 16N)
		Light::LightType type;	// 1N
		int blank1;				// 1N
		int blank2;				// 1N
		int blank3;				// 1N
		glm::vec4 color;		// 4N
		glm::vec4 position;		// 4N
		glm::vec4 direction;	// 4N
		glm::vec4 attenuation;  // 4N (Constant, Linear, Quadratic, unused)
	};

	bool loadShader(std::string shaderName);
	void initShaders();
	void setShader(Shader& s);
	void clearShader();
	void accumulateList();
	void clearBuffers();
	void renderScene();
	void gBufferPass(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
	void outlinePass(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
	void lightingPass();
	void uiPass();
	void combineMasterGeometry(std::vector<RenderData>& data);
	void combineOutlineGeometry(std::vector<RenderData>& data);
	void makeLightsViewSpace(glm::mat4 viewMatrix);
	TextureInfo& getTexture(std::string* path, bool scale = true);
	TextureInfo& loadTexture(const std::string& path, bool scaleImage = true);
	std::vector<GLfloat>* fetchSmoothNormals(Geometry* g);
	std::vector<GLfloat>* calcSmoothNormals(Geometry* geometry);
	Image* scaleImage(Image* input, int width, int height);
	glm::vec4 convertColor(Color c);

	Window* _window;
	std::vector<RenderData>* _renderingList;
	std::vector<RenderData>* _accumulatingList;
	std::map<std::string, Shader> _shaders;
	Shader* _shader;

	std::vector<RenderData>* _uiRenderingList;
	std::vector<RenderData>* _uiAccumulatingList;

	std::vector<RenderData>* _outlineRenderingList;
	std::vector<RenderData>* _outlineAccumulatingList;

	VertexArrayObject* _vao;
	VertexBufferObject* _positionVBO;
	VertexBufferObject* _normalVBO;
	VertexBufferObject* _texCoordVBO;
	ElementBufferObject* _ebo;

	FrameBufferObject* _fbo;
	FrameBufferObject* _outlineFBO;

	UniformBufferObject* _ubo;
	Camera* _camera;

	FrameBufferObject* _resizeInFBO;
	FrameBufferObject* _resizeOutFBO;
	
	GLTextureArray* _textures;
	GLTexture* _albedoBuffer;
	GLTexture* _normalBuffer;
	GLTexture* _positionBuffer;
	GLTexture* _specularBuffer;
	GLTexture* _outlineBuffer;

	CombinedGeometry* _masterGeometry;
	CombinedGeometry* _masterOutlineGeometry;

	Model* _screenQuad;
	CpuProfiler profiler;

	std::map<std::string, TextureInfo> _texturePathToInfo;
	
	std::map<Geometry*, std::vector<GLfloat>*> _smoothNormalCache;

	std::vector<Geometry*>* _staticGeometries;
	std::vector<Image*>* _staticTextures;

	std::vector<LightData>* _lightRenderingList;
	std::vector<LightData>* _lightAccumulatingList;

	TextureInfo _defaultTextureValue = TextureInfo(0, false, 1, 1);
};