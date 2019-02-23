#include "ContraptionFactory.h"



ContraptionFactory::ContraptionFactory()
{
	_platformModel = ModelLoader::loadModel("res/models/platform.obj");
	_gunModel = ModelLoader::loadModel("res/models/gun.obj");
	_coilModel = ModelLoader::loadModel("res/models/coil.obj");
	_bombModel = ModelLoader::loadModel("res/models/bomb.obj");
	_overchargeModel = ModelLoader::loadModel("res/models/overcharge.obj");
	_swordsModel = ModelLoader::loadModel("res/models/swords.obj");
}


ContraptionFactory::~ContraptionFactory()
{
}

Entity * ContraptionFactory::Create(CONTRAPTIONS type, glm::vec3 position) {

	Entity* contraption = EntityManager::Instance().Create();
	contraption->transform.setLocalPosition(position);

	auto c_renderable = ComponentManager<Renderable>::Instance().Create<Renderable>();

	switch (type) {
	case PLATFORM:
		c_renderable->setModel(*_platformModel);

	case GUN:
		c_renderable->setModel(*_gunModel);

	case COIL:
		c_renderable->setModel(*_coilModel);

	case BOMB:
		c_renderable->setModel(*_bombModel);

	case OVERCHARGE:
		c_renderable->setModel(*_overchargeModel);

	case SWORDS:
		c_renderable->setModel(*_swordsModel);

	default:
		break;
	}

	auto c_contraption = ComponentManager<Contraption>::Instance().Create<Contraption>();
	c_contraption->type = type;

	auto c_collider = ComponentManager<DebugColliderComponent>::Instance().Create<DebugColliderComponent>();
	c_collider->tag = "contraption";

	contraption->AddComponent(c_renderable);
	contraption->AddComponent(c_contraption);
	contraption->AddComponent(c_collider);

	return contraption;
}
