#pragma once
#include "Model.h"
class ModelGen {
public:
	enum Axis {
		X,
		Y,
		Z
	};
	static Model* makeQuad(ModelGen::Axis facing, float width, float height);
	static Model* makeCube(float width, float height, float depth);
};