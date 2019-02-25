#include "ContraptionFactory.h"



ContraptionFactory::ContraptionFactory()
{
	_platformModel = ModelLoader::loadModel("res/models/spring.obj");
	_gunModel = ModelLoader::loadModel("res/models/screw.obj");
	_coilModel = ModelLoader::loadModel("res/models/spring.obj");
	_bombModel = ModelLoader::loadModel("res/models/battery.obj");
	_overchargeModel = ModelLoader::loadModel("res/models/battery.obj");
	_swordsModel = ModelLoader::loadModel("res/models/screw.obj");
}


ContraptionFactory::~ContraptionFactory()
{
}

Entity * ContraptionFactory::Create(CONTRAPTIONS type, glm::vec3 position) {

	Entity* contraption = EntityManager::Instance().Create();
	contraption->transform.setLocalPosition(position);

	auto c_renderable = ComponentManager<Renderable>::Instance().Create<Renderable>();

	switch (type) {
	case PLATFORM: {
		c_renderable->setModel(*_platformModel);
		auto c_platform = ComponentManager<Contraption>::Instance().Create<Platform>();
		contraption->AddComponent(c_platform);
		break;
	}

	case GUN: {
		c_renderable->setModel(*_gunModel);
		auto c_gun = ComponentManager<Contraption>::Instance().Create<Gun>();
		contraption->AddComponent(c_gun);
		break;
	}

	case COIL: {
		c_renderable->setModel(*_coilModel);
		auto c_coil = ComponentManager<Contraption>::Instance().Create<Coil>();
		contraption->AddComponent(c_coil);
		break;
	}

	case BOMB: {
		c_renderable->setModel(*_bombModel);
		auto c_bomb = ComponentManager<Contraption>::Instance().Create<Bomb>();
		contraption->AddComponent(c_bomb);
		break;
	}

	case OVERCHARGE: {
		c_renderable->setModel(*_overchargeModel);
		auto c_overcharge = ComponentManager<Contraption>::Instance().Create<Overcharge>();
		contraption->AddComponent(c_overcharge);
		break;
	}

	case SWORDS: {
		c_renderable->setModel(*_swordsModel);
		auto c_swords = ComponentManager<Contraption>::Instance().Create<Swords>();
		contraption->AddComponent(c_swords);
		break;
	}

	default:
		break;
	}
	auto c_collider = ComponentManager<DebugColliderComponent>::Instance().Create<DebugColliderComponent>();
	c_collider->tag = "contraption";

	contraption->AddComponent(c_renderable);
	contraption->AddComponent(c_collider);

	return contraption;
}
