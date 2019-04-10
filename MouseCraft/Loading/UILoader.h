#pragma once
#include <string>
//#include <tinyxml2/tinyxml2.h>
#include "../Core/Entity.h"

class UILoader {
	static Entity* loadUI(std::string path, float width, float height);
private:
	//static Entity* readChild(const tinyxml2::XMLElement* element);
};