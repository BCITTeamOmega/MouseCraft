#pragma once
#include "Model.h"
class Renderer {
public:
	void initialize();
	void queueRender(Model m);
	void render();
private:
	
};