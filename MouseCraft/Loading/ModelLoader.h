#pragma once
#include "../Graphics/Model.h"
#include <string>

class ModelLoader
{
public:
	static Model* loadModel(std::string filename);
};

