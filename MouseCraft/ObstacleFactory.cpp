#include "ObstacleFactory.h"

#include "Core/EntityManager.h"
#include "Core/ComponentManager.h"
#include "Graphics/Model.h"
#include "Graphics/ModelGen.h"
#include "Graphics/Renderable.h"
#include "Physics/PhysicsManager.h"
#include "Physics/PhysicsComponent.h"
#include "Loading/ModelLoader.h"
#include "HealthComponent.h"
#include "YarnBall.h"
#include "Lamp.h"
#include "Obstruction.h"
#include "Vase.h"

ObstacleFactory::ObstacleFactory()
{
	_ballModel = ModelLoader::loadModel("res/models/test/teapot.obj"); // ball temp
	_cylinderModel = ModelLoader::loadModel("res/models/test/Cylinder.obj"); // vase / lamp temp
	_boxModel = ModelGen::makeCube(4, 4, 4);
	_bookModel = ModelGen::makeCube(2, 2, 1);
}

ObstacleFactory::~ObstacleFactory()
{
}

Entity * ObstacleFactory::Create(OBSTACLES type, glm::vec3 pos, bool isUp)
{
	auto e = EntityManager::Instance().Create();
	Renderable* c_render = ComponentManager<Renderable>::Instance().Create<Renderable>();
	PhysicsComponent* c_phys;
	HealthComponent* c_health = ComponentManager<HealthComponent>::Instance().Create<HealthComponent>();

	switch (type)
	{
	case BOOK:
	{
		c_render->setModel(*_bookModel);
		c_render->setColor(Color(1.0, 0.0, 1.0));
		c_phys = PhysicsManager::instance()->createGridObject(pos.x, pos.z, 2, 2, isUp ? PhysObjectType::OBSTACLE_UP : PhysObjectType::OBSTACLE_DOWN);
		Obstruction* c_obs = ComponentManager<UpdatableComponent>::Instance().Create<Obstruction>();
		e->AddComponent(c_obs);
		break;
	}
	case YARNBALL:
	{
		c_render->setModel(*_ballModel);
		c_render->setColor(Color(1.0, 0.5, 0.5));
		c_phys = PhysicsManager::instance()->createGridObject(pos.x, pos.z, 2, 2, isUp ? PhysObjectType::OBSTACLE_UP : PhysObjectType::OBSTACLE_DOWN);
		YarnBall* c_ball = ComponentManager<UpdatableComponent>::Instance().Create<YarnBall>();
		e->AddComponent(c_ball);
		break;
	}
	case VASE:
	{
		c_render->setModel(*_cylinderModel);
		c_render->setColor(Color(0.0, 1.0, 0.0));
		c_phys = PhysicsManager::instance()->createGridObject(pos.x, pos.z, 2, 2, isUp ? PhysObjectType::OBSTACLE_UP : PhysObjectType::OBSTACLE_DOWN);
		Vase* c_vase = ComponentManager<UpdatableComponent>::Instance().Create<Vase>();
		e->AddComponent(c_vase);
		break;
	}
	case BOX:
	{
		c_render->setModel(*_boxModel);
		c_render->setColor(Color(1.0, 0.0, 0.0));
		c_phys = PhysicsManager::instance()->createGridObject(pos.x, pos.z, 7, 7, isUp ? PhysObjectType::OBSTACLE_UP : PhysObjectType::OBSTACLE_DOWN);
		Obstruction* c_obs = ComponentManager<UpdatableComponent>::Instance().Create<Obstruction>();
		e->AddComponent(c_obs);
		break;
	}
	case LAMP:
	{	
		auto fieldModel = ModelGen::makeCube(16, 0.1, 16);
		c_render->setModel(*fieldModel);
		c_render->setColor(Color(1.0, 1.0, 0.0));
		c_render->SetEnabled(false);	// this is the field 
		c_phys = PhysicsManager::instance()->createGridObject(pos.x, pos.z, 2, 2, isUp ? PhysObjectType::OBSTACLE_UP : PhysObjectType::OBSTACLE_DOWN);
		Lamp* c_lamp = ComponentManager<UpdatableComponent>::Instance().Create<Lamp>();
		e->AddComponent(c_lamp);

		// this is the lamp visual
		auto e_lampModel = EntityManager::Instance().Create();
		auto c_lampRender = ComponentManager<Renderable>::Instance().Create<Renderable>();
		c_lampRender->setModel(*_cylinderModel);
		c_lampRender->setColor(Color(1.0, 1.0, 0.0));
		e_lampModel->AddComponent(c_lampRender);
		e->AddChild(e_lampModel);

		c_lamp->visualsEntity = e_lampModel;
		break;
	}
	default:
		std::cerr << "ERROR: Obstacle Factory making unknown type!" << std::endl;
		c_phys = new PhysicsComponent(PhysObjectType::OBSTACLE_DOWN, 0, 0, 0, 0);
		break;
	}
	
	e->AddComponent(c_render);
	e->AddComponent(c_phys);
	e->AddComponent(c_health);

	c_phys->initPosition();

	return e;
}
