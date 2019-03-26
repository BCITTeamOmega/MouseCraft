#include "ObstacleFactory.h"

#include "Core/EntityManager.h"
#include "Core/ComponentManager.h"
#include "Graphics/Model.h"
#include "Graphics/ModelGen.h"
#include "Graphics/Renderable.h"
#include "Physics/PhysicsManager.h"
#include "Physics/PhysicsComponent.h"
#include "Loading/ModelLoader.h"
#include "YarnBall.h"
#include "HealthComponent.h"

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

Entity * ObstacleFactory::Create(OBSTACLES type, glm::vec3 pos)
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
		c_phys = PhysicsManager::instance()->createGridObject(pos.x, pos.z, 5, 5, PhysObjectType::OBSTACLE_UP);
		break;
	}
	case YARNBALL:
	{
		c_render->setModel(*_ballModel);
		c_render->setColor(Color(1.0, 0.5, 0.5));
		c_phys = PhysicsManager::instance()->createGridObject(pos.x, pos.z, 5, 5, PhysObjectType::OBSTACLE_UP);
		YarnBall* c_ball = ComponentManager<UpdatableComponent>::Instance().Create<YarnBall>();
		e->AddComponent(c_ball);
		break;
	}
	case VASE:
	{
		c_render->setModel(*_cylinderModel);
		c_render->setColor(Color(0.0, 1.0, 0.0));
		c_phys = PhysicsManager::instance()->createGridObject(pos.x, pos.z, 5, 5, PhysObjectType::OBSTACLE_UP);
		break;
	}
	case BOX:
	{
		c_render->setModel(*_boxModel);
		c_render->setColor(Color(1.0, 0.0, 0.0));
		c_phys = PhysicsManager::instance()->createGridObject(pos.x, pos.z, 7, 7, PhysObjectType::OBSTACLE_UP);
		break;
	}
	case LAMP:
	{	
		c_render->setModel(*_cylinderModel);
		c_render->setColor(Color(1.0, 1.0, 0.0));
		c_phys = PhysicsManager::instance()->createGridObject(pos.x, pos.z, 5, 5, PhysObjectType::OBSTACLE_UP);
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
