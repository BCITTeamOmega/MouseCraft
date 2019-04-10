#include "Lamp.h"

#include "Graphics/Renderable.h"
#include "HealthComponent.h"
#define _USE_MATH_DEFINES
#include <math.h>

Lamp::Lamp()
{
}


Lamp::~Lamp()
{
}

void Lamp::OnInitialized()
{
	auto physics = GetEntity()->GetComponent<PhysicsComponent>();
	if (physics->isUp)
		_checkFor = { PhysObjectType::MOUSE_UP };
	else
		_checkFor = { PhysObjectType::MOUSE_DOWN };
}

void Lamp::Update(float deltaTime)
{
	if (_isPlaced)
	{
		_counter += deltaTime;
		if (_counter >= DAMAGE_RATE)
		{
			_counter -= DAMAGE_RATE;

			auto pos = GetEntity()->t().wPos();
			auto bl = pos + glm::vec3(-0.5, 0, -0.5) * FIELD_RANGE;
			auto tr = pos + glm::vec3(0.5, 0, 0.5) * FIELD_RANGE;
			auto hits = PhysicsManager::instance()->areaCheck(nullptr, _checkFor, new Vector2D(bl.x, bl.z), new Vector2D(tr.x, tr.z));
			for (auto& p : hits)
			{
				auto health = p->GetEntity()->GetComponent<HealthComponent>();
				health->Damage(1);
			}
		}
	}
}

void Lamp::HitByCat(Vector2D dir)
{
	if (!_isPlaced)
	{
		visualsEntity->transform.rotate(glm::vec3(0, 0, M_PI / 2));
		GetEntity()->GetComponent<Renderable>()->SetEnabled(true);
		_isPlaced = true;
	}
}

void Lamp::DestroyedByMouse()
{
	GetEntity()->Destroy();
}

Component * Lamp::Create(json json)
{
	auto c = ComponentManager<UpdatableComponent>::Instance().Create<Lamp>();
	return c;
}

PrefabRegistrar Lamp::reg("Lamp", &Lamp::Create);