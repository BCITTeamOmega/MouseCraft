#include "PickupFactory.h"

#include "Core/EntityManager.h"
#include "Core/ComponentManager.h"
#include "Core/OmegaEngine.h"
#include "Loading/ModelLoader.h"
#include "Graphics/Renderable.h"
#include "Loading/ImageLoader.h"
#include "Physics/PhysicsManager.h"

PickupFactory::PickupFactory()
{
	_screwModel = ModelLoader::loadModel("res/models/screw.obj");
	_springModel = ModelLoader::loadModel("res/models/spring.obj");
	_batteryModel = ModelLoader::loadModel("res/models/battery.obj");

	_spawnAnim = new Animation();
	_spawnAnim->name = "spawn";
	_spawnAnim->duration = 1.0f;
	_spawnAnim->AddScale(0.0f, glm::vec3(0));
	_spawnAnim->AddScale(0.8f, glm::vec3(1.2f));
	_spawnAnim->AddScale(1.0f, glm::vec3(1.0f));
	_spawnAnim->SetCurve(new SineConverter());
}

PickupFactory::~PickupFactory()
{
}

Entity * PickupFactory::Create(PICKUPS type, glm::vec3 position)
{
	// create entity 
	Entity* pickup = EntityManager::Instance().Create();
	pickup->transform.setLocalPosition(position);
	pickup->transform.setLocalRotation(glm::vec3(0.42f, 0.0f, 0.0f));
	pickup->transform.setLocalScale(glm::vec3(0.0f));
	
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

	auto c_anim = ComponentManager<UpdatableComponent>::Instance().Create<TransformAnimator>();
	c_anim->SetOneShot(true);
	c_anim->AddAnimation(_spawnAnim);
	
	auto c_rotator = ComponentManager<UpdatableComponent>::Instance().Create<Rotator>();
	c_rotator->rotationSpeed = glm::vec3(0.0, glm::pi<float>() * 0.5f, 0.0);
	
	// create logic 
	auto c_pickup = ComponentManager<Pickup>::Instance().Create<Pickup>();
	c_pickup->type = type;

	auto pSys = OmegaEngine::Instance().GetSystem<PhysicsManager>();
	auto c_physics = pSys->createGridObject(position.x, position.z, 1, 1, PhysObjectType::PART);
	// auto c_physics = pSys->createObject(0, 0, 0.1, 0.1, 0.0, PhysObjectType::OBSTACLE_DOWN); this works

	// ASSEMBLE
	pickup->AddComponent(c_renderable);
	pickup->AddComponent(c_anim);
	pickup->AddComponent(c_rotator);
	pickup->AddComponent(c_pickup);
	pickup->AddComponent(c_physics);

	return pickup;
}
