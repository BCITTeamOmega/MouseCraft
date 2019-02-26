#include "PickupFactory.h"

#include "Core/EntityManager.h"
#include "Core/ComponentManager.h"
#include "Loading/ModelLoader.h"
#include "Graphics/Renderable.h"
#include "Loading/ImageLoader.h"

PickupFactory::PickupFactory()
{
	_screwModel = ModelLoader::loadModel("res/models/screw.obj");
	_springModel = ModelLoader::loadModel("res/models/spring.obj");
	_batteryModel = ModelLoader::loadModel("res/models/battery.obj");

	_texture = ImageLoader::loadImage("res/models/test/blank.bmp");
	_screwModel->setTexture(_texture);
	_springModel->setTexture(_texture);
	_batteryModel->setTexture(_texture);
}

PickupFactory::~PickupFactory()
{
}

Entity * PickupFactory::Create(PICKUPS type, glm::vec3 position)
{
	// create entity 
	Entity* pickup = EntityManager::Instance().Create();
	pickup->transform.setLocalPosition(position);
	
	// create visuals 
	auto c_renderable = ComponentManager<Renderable>::Instance().Create<Renderable>();
	
	switch (type)
	{
	case BATTERY:
		c_renderable->setModel(*_batteryModel);
		c_renderable->setColor(Color(1.0, 0.6, 0.0));
		break;
	case SCREW:
		c_renderable->setModel(*_screwModel);
		c_renderable->setColor(Color(0.5, 0.5, 0.5));
		break;
	case SPRING:
		c_renderable->setModel(*_springModel);
		c_renderable->setColor(Color(0.8, 0.1, 0.0));
		break;
	default:
		break;
	}

	// create logic 
	auto c_pickup = ComponentManager<Pickup>::Instance().Create<Pickup>();
	c_pickup->type = type;
	
	auto c_collider = ComponentManager<DebugColliderComponent>::Instance().Create<DebugColliderComponent>();
	c_collider->tag = "pickup";

	// ASSEMBLE
	pickup->AddComponent(c_renderable);
	pickup->AddComponent(c_pickup);
	pickup->AddComponent(c_collider);

	return pickup;
}
