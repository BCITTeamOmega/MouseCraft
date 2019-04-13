#include "ModelLoader.h"
#include <fstream>
#include <iostream>
#include "ObjFileParser.h"

using std::ifstream;
using std::string;
using std::vector;

Model* ModelLoader::loadModel(string filename) {
	/*
	/// Old model loading code

	ObjFileParser parser = ObjFileParser();
	ifstream file;
	file.open(filename);
	if (!file.is_open()) {
		std::cerr << "Problem opening file " << filename << std::endl;
		return nullptr;
	}
	Geometry* g = parser.parse(file);
	file.close();
	return new Model(g);
	*/

	// Model loading adapted from https://learnopengl.com/Model-Loading/Model

	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(
		filename,
		aiProcess_Triangulate
		| aiProcess_GenSmoothNormals
	);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cerr << "Problem loading file " << filename << ": " << importer.GetErrorString() << std::endl;
		return nullptr;
	}
	string dir = filename.substr(0, filename.find_last_of('/'));
	Geometry* g = new Geometry();
	vector<aiMesh*> meshes = processNode(scene->mRootNode, scene);
	processMeshes(meshes, scene, g);
	return new Model(g);
}

vector<aiMesh*> ModelLoader::processNode(aiNode* node, const aiScene *scene) {
	vector<aiMesh*> meshes;
	for (int i = 0; i < node->mNumMeshes; i++) {
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(mesh);
	}
	for (int i = 0; i < node->mNumChildren; i++) {
		vector<aiMesh*> childMesh = processNode(node->mChildren[i], scene);
		meshes.insert(meshes.end(), childMesh.begin(), childMesh.end());
	}
	return meshes;
}

void ModelLoader::processMeshes(vector<aiMesh*>& meshes, const aiScene* scene, Geometry* g) {
	std::vector<GLfloat> vertPositions;
	std::vector<GLfloat> vertNormals;
	std::vector<GLfloat> vertTexCoord;
	std::vector<GLuint> indices;
	int meshStartIndex = 0;
	for (aiMesh* mesh : meshes) {
		bool hasTex = mesh->mTextureCoords[0];
		for (int i = 0; i < mesh->mNumVertices; i++) {
			aiVector3D v = mesh->mVertices[i];
			aiVector3D nv = mesh->mNormals[i];
			vertPositions.push_back(v.x);
			vertPositions.push_back(v.y);
			vertPositions.push_back(v.z);
			vertNormals.push_back(nv.x);
			vertNormals.push_back(nv.y);
			vertNormals.push_back(nv.z);

			if (hasTex) {
				aiVector3D tv = mesh->mTextureCoords[0][i];
				vertTexCoord.push_back(tv.x);
				vertTexCoord.push_back(tv.y);
			}
			else {
				vertTexCoord.push_back(0.0f);
				vertTexCoord.push_back(0.0f);
			}
		}
		for (int i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for (int j = 0; j < face.mNumIndices; j++) {
				indices.push_back(face.mIndices[j] + meshStartIndex);
			}
		}
		meshStartIndex = indices.size();
	}
	g->setVertexData(vertPositions);
	g->setNormalData(vertNormals);
	g->setTexCoordData(vertTexCoord);
	g->setIndices(indices);
}