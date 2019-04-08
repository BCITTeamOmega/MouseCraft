#include "ContraptionFactory.h"

#include "Loading/ImageLoader.h"

#include "Graphics/ModelGen.h"

#include "TimedDestruction.h"

#include "DamageOnCollision.h"

ContraptionFactory::ContraptionFactory()
{
	_platformModel = ModelLoader::loadModel("res/models/spring.obj");
	_gunModel = ModelLoader::loadModel("res/models/screw.obj");
	_coilModel = ModelLoader::loadModel("res/models/spring.obj");
	_bombModel = ModelLoader::loadModel("res/models/battery.obj");
	_overchargeModel = ModelLoader::loadModel("res/models/battery.obj");
	_swordsModel = ModelLoader::loadModel("res/models/screw.obj");
	_coilFieldModel = ModelGen::makeCube(16, 0.1, 16);
}


ContraptionFactory::~ContraptionFactory()
{
}

Entity * ContraptionFactory::Create(CONTRAPTIONS type, glm::vec3 position) {

	Entity* contraption = EntityManager::Instance().Create();
	contraption->transform.setLocalPosition(position);

	auto c_renderable = ComponentManager<Renderable>::Instance().Create<Renderable>();

	switch (type) {
	case TRAMPOLINE: {
		c_renderable->setModel(*_platformModel);
		auto c_trampoline = ComponentManager<Contraption>::Instance().Create<Trampoline>();
		contraption->AddComponent(c_trampoline);	

		auto e_trampolineField = EntityManager::Instance().Create();
		e_trampolineField->SetEnabled(false);
		auto c_trampolineRender = ComponentManager<Renderable>::Instance().Create<Renderable>();
		c_trampolineRender->setModel(*_coilFieldModel);
		c_trampolineRender->setColor(Color(0.9f, 1.0f, 0.9f));
		e_trampolineField->AddComponent(c_trampolineRender);

		contraption->AddChild(e_trampolineField);
		c_trampoline->fieldEntity = e_trampolineField;
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
		auto c_phys = PhysicsManager::instance()->createObject(0, 0, 1, 1, 0, PhysObjectType::PROJECTILE_DOWN);
		c_phys->SetEnabled(false);
		contraption->AddComponent(c_phys);
		auto c_timed = ComponentManager<UpdatableComponent>::Instance().Create<TimedDestruction>();
		c_timed->delay = Bomb::LIFETIME;
		c_timed->SetEnabled(false);
		contraption->AddComponent(c_timed);
		auto c_dcol = ComponentManager<Component>::Instance().Create<DamageOnCollision>();
		c_dcol->damage = 0;
		c_dcol->SetEnabled(false);
		contraption->AddComponent(c_dcol);
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
	
	contraption->AddComponent(c_renderable);

	return contraption;
}
