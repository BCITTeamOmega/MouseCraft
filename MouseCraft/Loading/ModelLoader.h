#pragma once
#include "../Graphics/Model.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include <string>
#include <vector>

class ModelLoader
{
public:
	static Model* loadModel(std::string filename);
private:
	static void processMeshes(std::vector<aiMesh*>& meshes, const aiScene* scene, Geometry* g);
	static std::vector<aiMesh*> processNode(aiNode* node, const aiScene* scene);
};

