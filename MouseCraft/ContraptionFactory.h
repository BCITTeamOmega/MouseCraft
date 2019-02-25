#pragma once

#include <glm/glm.hpp>
#include "Core/EntityManager.h"
#include "Core/ComponentManager.h"
#include "Loading/ModelLoader.h"
#include "Graphics/Renderable.h"
#include "Graphics/Model.h"
#include "MOUSECRAFT_ENUMS.h"
#include "Contraption.h"
#include "Bomb.h"
#include "Coil.h"
#include "Gun.h"
#include "Overcharge.h"
#include "Platform.h"
#include "Swords.h"

class ContraptionFactory
{
public:
	static ContraptionFactory& Instance() 
	{
		static ContraptionFactory _instance;
		return _instance;
	}

	ContraptionFactory(ContraptionFactory const&) = delete;
	void operator=(ContraptionFactory const&) = delete;

private:
	ContraptionFactory();
	~ContraptionFactory();

public:
	Entity* Create(CONTRAPTIONS type, glm::vec3 position);

private:
	Model* _platformModel;
	Model* _gunModel;
	Model* _coilModel;
	Model* _bombModel;
	Model* _overchargeModel;
	Model* _swordsModel;
};

