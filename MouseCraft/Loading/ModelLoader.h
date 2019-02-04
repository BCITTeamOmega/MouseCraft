#pragma once
#include "../Graphics/Model.h"
#include <string>

using std::string;

class ModelLoader
{
public:
	static Model* loadModel(string filename);
};

