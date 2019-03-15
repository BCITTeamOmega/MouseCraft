#include "ContraptionFactory.h"

#include "Loading/ImageLoader.h"

#include "Graphics/ModelGen.h"

ContraptionFactory::ContraptionFactory()
{
	_platformModel = ModelLoader::loadModel("res/models/spring.obj");
	_gunModel = ModelLoader::loadModel("res/models/screw.obj");
	_coilModel = ModelLoader::loadModel("res/models/spring.obj");
	_bombModel = ModelLoader::loadModel("res/models/battery.obj");
	_overchargeModel = ModelLoader::loadModel("res/models/battery.obj");
	_swordsModel = ModelLoader::loadModel("res/models/screw.obj");
	_coilFieldModel = ModelGen::makeCube(16, 0.1, 16);
	
	_texture = ImageLoader::loadImage("res/models/test/blank.bmp");
	_platformModel->setTexture(_texture);
	_gunModel->setTexture(_texture);
	_coilModel->setTexture(_texture);
	_bombModel->setTexture(_texture);
	_overchargeModel->setTexture(_texture);
	_swordsModel->setTexture(_texture);
	_coilFieldModel->setTexture(_texture);
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

		// the field 
		auto e_coilField = EntityManager::Instance().Create();
		e_coilField->SetEnabled(false);
		auto c_coilRender = ComponentManager<Renderable>::Instance().Create<Renderable>();
		c_coilRender->setModel(*_coilFieldModel);
		c_coilRender->setColor(Color(0.9f, 1.0f, 0.9f));
		e_coilField->AddComponent(c_coilRender);

		contraption->AddChild(e_coilField);
		c_coil->fieldEntity = e_coilField;
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

	auto pSys = OmegaEngine::Instance().GetSystem<PhysicsManager>();
	auto c_physics = pSys->createObject(position.x, position.z, 1, 1, 0.0, PhysObjectType::CONTRAPTION_DOWN);

	contraption->AddComponent(c_renderable);
	contraption->AddComponent(c_physics);
	return contraption;
}
