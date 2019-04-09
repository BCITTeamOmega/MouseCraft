#pragma once

#include "MOUSECRAFT_ENUMS.h"
#include "Core/Entity.h"
#include "Graphics/Model.h"

class ObstacleFactory
{
// singleton 
public:
	static ObstacleFactory& Instance()
	{
		static ObstacleFactory _instance;
		return _instance;
	}
	ObstacleFactory(ObstacleFactory const&) = delete;
	void operator=(ObstacleFactory const&) = delete;
private:
	ObstacleFactory();
	~ObstacleFactory();

// factory 
public: 
	Entity* Create(OBSTACLES type, glm::vec3 position, bool isUp);
private:
	Model* _ballModel;
	Model* _lampModel;
	Model* _cylinderModel;
	Model* _boxModel;
	Model* _bookModel;
};

