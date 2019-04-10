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
#include "Network/NetworkComponent.h"
#include "Network/NetworkSystem.h"

ObstacleFactory::ObstacleFactory()
{
	// load models 
	_ballModel = ModelLoader::loadModel("res/models/sphere.obj");
	_lampModel = ModelLoader::loadModel("res/models/lamp.obj");
	_cylinderModel = ModelLoader::loadModel("res/models/test/Cylinder.obj"); // vase / lamp temp
	_boxModel = ModelLoader::loadModel("res/models/cube.obj");
	_bookModel = ModelLoader::loadModel("res/models/book.obj");
	// load textures 
	std::string* boxTex = new std::string("res/textures/box.png");
	_boxModel->setTexture(boxTex);
}

ObstacleFactory::~ObstacleFactory()
{
}

Entity * ObstacleFactory::CreateSimulated(OBSTACLES type, glm::vec3 position, bool isUp, std::vector<unsigned int>* netIds)
{
	auto e = EntityManager::Instance().Create();
	e->transform.setLocalPosition(position);
	Renderable* c_render = ComponentManager<Renderable>::Instance().Create<Renderable>();

	if (netIds)
	{
		auto c_net = NetworkSystem::Instance()->CreateComponent((*netIds)[0]);
		e->AddComponent(c_net);
	}
	else
	{
		std::cout << "WARNING: NETWORK IDS NULLPTR" << std::endl;
	}

	switch (type)
	{
	case BOOK:
	{
		c_render->setModel(*_bookModel);
		c_render->setColor(Color(0.24f, 0.0f, 0.0f));
		break;
	}
	case YARNBALL:
	{
		c_render->setModel(*_ballModel);
		c_render->setColor(Color(1.0, 0.5, 0.5));
		break;
	}
	case VASE:
	{
		auto fieldModel = ModelGen::makeCube(16, 0.1, 16);
		c_render->setModel(*fieldModel);
		c_render->setColor(Color(0.0, 0.0, 1.0));
		c_render->SetEnabled(false);	// this is the field 

		auto e_vaseModel = EntityManager::Instance().Create();
		auto c_vaseRender = ComponentManager<Renderable>::Instance().Create<Renderable>();
		c_vaseRender->setModel(*_cylinderModel);
		c_vaseRender->setColor(Color(0.0, 1.0, 0.0));
		e_vaseModel->AddComponent(c_vaseRender);
		e->AddChild(e_vaseModel);

		if (netIds)
		{
			auto c_net = NetworkSystem::Instance()->CreateComponent((*netIds)[1]);
			e_vaseModel->AddComponent(c_net);
		}

		break;
	}
	case BOX:
	{
		c_render->setModel(*_boxModel);
		c_render->setColor(Color(1.0, 1.0, 1.0));
		e->transform.setLocalScale(glm::vec3(4.0f));
		break;
	}
	case LAMP:
	{
		auto fieldModel = ModelGen::makeCube(16, 0.1, 16);
		c_render->setModel(*fieldModel);
		c_render->setColor(Color(1.0, 1.0, 0.0));
		c_render->SetEnabled(false);	// this is the field 

		// this is the lamp visual
		auto e_lampModel = EntityManager::Instance().Create();
		auto c_lampRender = ComponentManager<Renderable>::Instance().Create<Renderable>();
		c_lampRender->setModel(*_lampModel);
		c_lampRender->setColor(Color(0.63f, 0.32f, 0.18f));
		e_lampModel->AddComponent(c_lampRender);
		e->AddChild(e_lampModel);

		if (netIds)
		{
			auto c_net = NetworkSystem::Instance()->CreateComponent((*netIds)[1]);
			e_lampModel->AddComponent(c_net);
		}

		break;
	}
	default:
		std::cerr << "ERROR: Obstacle Factory making unknown type!" << std::endl;
		break;
	}

	e->AddComponent(c_render);

	return e;
}

Entity * ObstacleFactory::Create(OBSTACLES type, glm::vec3 pos, bool isUp, std::vector<unsigned int>* netIds)
{
	auto e = EntityManager::Instance().Create();
	
	Component* c_render = nullptr; // ComponentManager<Renderable>::Instance().Create<Renderable>();
	PhysicsComponent* c_phys;
	HealthComponent* c_health = ComponentManager<HealthComponent>::Instance().Create<HealthComponent>();
	NetworkComponent* c_net = NetworkSystem::Instance()->CreateComponent();

	switch (type)
	{
	case BOOK:
	{
		c_render = PrefabLoader::LoadComponent("res/prefabs/components/obstacles/book_renderable.json");
		c_net->AddComponentData({ {"type", "file"}, {"value", "res/prefabs/components/obstacles/book_renderable.json"} });
		c_phys = PhysicsManager::instance()->createGridObject(pos.x, pos.z, 5, 5, isUp ? PhysObjectType::OBSTACLE_UP : PhysObjectType::OBSTACLE_DOWN);
		Obstruction* c_obs = ComponentManager<UpdatableComponent>::Instance().Create<Obstruction>();
		e->AddComponent(c_obs);
		break;
	}
	case YARNBALL:
	{
		c_render = PrefabLoader::LoadComponent("res/prefabs/components/obstacles/ball_renderable.json");
		c_net->AddComponentData({ {"type", "file"}, {"value", "res/prefabs/components/obstacles/ball_renderable.json"} });
		c_phys = PhysicsManager::instance()->createObject(pos.x, pos.z, 5, 5, 0, isUp ? PhysObjectType::OBSTACLE_UP : PhysObjectType::OBSTACLE_DOWN);
		YarnBall* c_ball = ComponentManager<UpdatableComponent>::Instance().Create<YarnBall>();
		e->AddComponent(c_ball);
		break;
	}
	case VASE:
	{
		// base entity is field 
		c_render = PrefabLoader::LoadComponent("res/prefabs/components/obstacles/vase_field_renderable.json");
		c_net->AddComponentData({ {"type", "file"}, {"value", "res/prefabs/components/obstacles/vase_field_renderable.json"} });
		c_phys = PhysicsManager::instance()->createGridObject(pos.x, pos.z, 5, 5, isUp ? PhysObjectType::OBSTACLE_UP : PhysObjectType::OBSTACLE_DOWN);
		Vase* c_vase = ComponentManager<UpdatableComponent>::Instance().Create<Vase>();
		e->AddComponent(c_vase);

		// child entity is vase model (so we can rotate without affecting parent)
		auto e_vaseModel = EntityManager::Instance().Create();
		Component* c_vaseRender = PrefabLoader::LoadComponent("res/prefabs/components/obstacles/vase_renderable.json");
		e_vaseModel->AddComponent(c_vaseRender);
		NetworkComponent* c_vaseNet = NetworkSystem::Instance()->CreateComponent();
		c_vaseNet->AddComponentData({ {"type", "file"}, {"value", "res/prefabs/components/obstacles/vase_renderable.json"} });
		e_vaseModel->AddComponent(c_vaseNet);

		// bindings
		e->AddChild(e_vaseModel);
		c_vase->visualsEntity = e_vaseModel;
		
		break;
	}
	case BOX:
	{
		c_render = PrefabLoader::LoadComponent("res/prefabs/components/obstacles/box_renderable.json");
		c_net->AddComponentData({ {"type", "file"}, {"value", "res/prefabs/components/obstacles/box_renderable.json"} });

		c_phys = PhysicsManager::instance()->createGridObject(pos.x, pos.z, 10, 10, isUp ? PhysObjectType::OBSTACLE_UP : PhysObjectType::OBSTACLE_DOWN);
		Obstruction* c_obs = ComponentManager<UpdatableComponent>::Instance().Create<Obstruction>();
		e->AddComponent(c_obs);

		e->transform.setLocalScale(glm::vec3(4.0f));
		break;
	}
	case LAMP:
	{	
		// base entity is field 
		auto fieldModel = ModelGen::makeCube(15, 0.1, 15);
		c_render = PrefabLoader::LoadComponent("res/prefabs/components/obstacles/lamp_field_renderable.json");
		c_net->AddComponentData({ {"type", "file"}, {"value", "res/prefabs/components/obstacles/lamp_field_renderable.json"} });
		c_phys = PhysicsManager::instance()->createGridObject(pos.x, pos.z, 5, 5, isUp ? PhysObjectType::OBSTACLE_UP : PhysObjectType::OBSTACLE_DOWN);
		Lamp* c_lamp = ComponentManager<UpdatableComponent>::Instance().Create<Lamp>();
		e->AddComponent(c_lamp);

		// child entity is lamp model (so we can rotate without affecting parent)
		auto e_lampModel = EntityManager::Instance().Create();
		Component* c_lampRender = PrefabLoader::LoadComponent("res/prefabs/components/obstacles/lamp_renderable.json");
		e_lampModel->AddComponent(c_lampRender);
		NetworkComponent* c_lampNet = NetworkSystem::Instance()->CreateComponent();
		c_lampNet->AddComponentData({ {"type", "file"}, {"value", "res/prefabs/components/obstacles/lamp_renderable.json"} });
		e_lampModel->AddComponent(c_lampNet);

		// bindings 
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
	e->AddComponent(c_net);

	c_phys->initPosition();

	return e;
}
