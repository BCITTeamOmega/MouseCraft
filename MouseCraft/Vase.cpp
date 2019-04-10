#include "Vase.h"

#include <algorithm>
#include "Graphics/Renderable.h"
#include "Physics/PhysicsComponent.h"
#include "PlayerComponent.h"
#define _USE_MATH_DEFINES
#include <math.h>

Vase::Vase()
{
}


Vase::~Vase()
{
}

void Vase::OnInitialized()
{
	auto physics = GetEntity()->GetComponent<PhysicsComponent>();
	if (physics->isUp)
		_checkFor = { PhysObjectType::MOUSE_UP };
	else
		_checkFor = { PhysObjectType::MOUSE_DOWN };
}

void Vase::Update(float deltaTime)
{
	if (_isPlaced)
	{
		auto pos = GetEntity()->t().wPos();
		auto bl = pos + glm::vec3(-0.5, 0, -0.5) * FIELD_RANGE;
		auto tr = pos + glm::vec3(0.5, 0, 0.5) * FIELD_RANGE;
		auto hits = PhysicsManager::instance()->areaCheck(nullptr, _checkFor, new Vector2D(bl.x, bl.z), new Vector2D(tr.x, tr.z));

		for (auto& p : hits)
		{
			auto it = std::find(_affected.begin(), _affected.end(), p);
			if (it == _affected.end())
			{
				_affected.push_back(p);
				p->GetEntity()->GetComponent<PlayerComponent>()->SetSpeed(50.0f * SLOW_RATIO);
			}
		}
		_found.clear();
		for (auto p : _affected)
		{
			if (hits.size() != 0)
			{
				auto it = std::find(hits.begin(), hits.end(), p);
				if (it == hits.end())
				{
					_found.push_back(*it);
				}
			}
			else
			{
				_found.push_back(p);
			}
		}
		for (auto p : _found)
		{
			p->GetEntity()->GetComponent<PlayerComponent>()->SetSpeed(50.0f);
			_affected.erase(std::find(_affected.begin(), _affected.end(), p));
		}
	}
}

void Vase::HitByCat(Vector2D dir)
{
	if (!_isPlaced)
	{
		visualsEntity->transform.rotate(glm::vec3(0, 0, M_PI / 2));
		GetEntity()->GetComponent<Renderable>()->SetEnabled(true);
		GetEntity()->GetComponent<PhysicsComponent>()->SetEnabled(false);
		_isPlaced = true;
	}
}

void Vase::DestroyedByMouse()
{
	GetEntity()->Destroy();
}

Component * Vase::Create(json json)
{
	auto c = ComponentManager<UpdatableComponent>::Instance().Create<Vase>();
	return c;
}

PrefabRegistrar Vase::reg("Vase", &Vase::Create);