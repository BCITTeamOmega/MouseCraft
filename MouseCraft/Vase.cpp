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
			if (_affected.find(p) != _affected.end())
			{
				if (_affected[p]->x == 0 && _affected[p]->y == 0)
					_affected[p]->x = 50;
				p->velocity = *_affected[p];
				p->GetEntity()->GetComponent<PlayerComponent>()->SetDisabled(true);
			}
			else
			{
				_affected[p] = new Vector2D(p->velocity.x, p->velocity.y);
			}
		}
		_found.clear();
		for (auto p : _affected)
		{
			if (hits.size() != 0)
			{
				auto it = std::find(hits.begin(), hits.end(), p.first);
				if (it == hits.end())
				{
					_found.push_back(*it);
				}
			}
			else
			{
				_found.push_back(p.first);
			}
		}
		for (auto& p : _found)
		{
			p->GetEntity()->GetComponent<PlayerComponent>()->SetDisabled(false);
			_affected.erase(p);
		}
	}
}

void Vase::HitByCat(Vector2D dir)
{
	if (!_isPlaced)
	{
		visualsEntity->transform.rotate(glm::vec3(0, 0, M_PI / 2));
		GetEntity()->GetComponent<Renderable>()->SetEnabled(true);
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