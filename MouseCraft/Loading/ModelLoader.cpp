#include "ModelLoader.h"
#include <fstream>
#include <iostream>
#include "ObjFileParser.h"

using std::ifstream;

Model* ModelLoader::loadModel(string filename) {
	ObjFileParser parser = ObjFileParser();
	ifstream file;
	file.open(filename);
	if (!file.is_open()) {
		std::cerr << "Problem opening file " << filename << std::endl;
		return nullptr;
	}
	Geometry g = parser.parse(file);
	file.close();
	return new Model(g);
}
