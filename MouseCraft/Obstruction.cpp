#include "Obstruction.h"

#include "HealthComponent.h"

const int Obstruction::BOX_SIZE = 10;
const int Obstruction::BOOK_SIZE = 4;

Obstruction::Obstruction() : 
	HandleMouseCollide(this, &Obstruction::OnMouseCollide)
{
}

Obstruction::~Obstruction()
{
}

void Obstruction::OnInitialized()
{
	Obstacle::OnInitialized();
	_physics = GetEntity()->GetComponent<PhysicsComponent>();
	_physics->onCollide.Attach(HandleMouseCollide);
}

void Obstruction::Update(float deltaTime)
{
}

void Obstruction::HitByCat(Vector2D dir) 
{
	std::cout << "Cat hit an obstruction (book/box)." << std::endl;
	
	auto grid = PhysicsManager::instance()->getGrid();
	auto pos = GetEntity()->t().getWorldPosition2D();
	auto newPos = pos + (dir * grid->scale);
	auto halfsize = Vector2D(size / 2.0f, size / 2.0f);

	// first cleanup our area to check
	auto tl = new Vector2D(pos - halfsize);
	auto br = new Vector2D(pos + halfsize);
	grid->removeArea(tl, br);

	// now check 
	auto ntl = new Vector2D(newPos - halfsize);
	auto nbr = new Vector2D(newPos + halfsize);
	// auto hits = grid->objectsInArea(ntl, nbr);

	bool stop = false;
	auto results = PhysicsManager::instance()->areaCheck(_physics, ntl, nbr);
	for (auto pc : results)
	{
		if (pc == _physics)
			continue;

		if (_physics->isUp)
		{
			if (pc->pType == PhysObjectType::OBSTACLE_UP || pc->pType == PhysObjectType::WALL || pc->pType == PhysObjectType::MOUSE_UP)
			{
				stop = true;
				break;
			}
		}
		else
		{
			if (pc->pType == PhysObjectType::PLATFORM || pc->pType == PhysObjectType::OBSTACLE_DOWN || pc->pType == PhysObjectType::WALL || pc->pType == PhysObjectType::MOUSE_DOWN)
			{
				stop = true;
				break;
			}
		}
	}
	/*if (hits)
	{
		for (auto pc : *hits)
		{
			if (_physics->pType == PhysObjectType::OBSTACLE_UP)
			{
				if (pc->pType == PhysObjectType::OBSTACLE_UP)
				{
					stop = true;
					break;
				}
			}
			else 
			{
				if (pc->pType == PhysObjectType::PLATFORM || pc->pType == PhysObjectType::OBSTACLE_DOWN)
				{
					stop = true;
					break;
				}
			}

		}
	}
	else
	{
		stop = true;
	}*/

	if (stop)
	{
		// oh no git reset --HARD
		grid->createArea(*tl, *br, _physics, _physics->pType);
	}
	else
	{
		// hell yea, knock this chonk over there 
		for (auto pc : results)
			if (pc != _physics && pc->pType != PhysObjectType::WALL && pc->pType != PhysObjectType::PLATFORM)
				pc->GetEntity()->Destroy();
		grid->createArea(*ntl, *nbr, _physics, _physics->pType);
		_physics->moveBody(&newPos, 0);
		_physics->updateFalling();
	}
}

void Obstruction::DestroyedByMouse()
{
	GetEntity()->Destroy();
}

void Obstruction::OnMouseCollide(PhysicsComponent * other)
{
	if (!_physics->isFalling)
		return;

	if (other->pType == PhysObjectType::MOUSE_DOWN || other->pType == PhysObjectType::MOUSE_UP)
	{
		auto health = other->GetEntity()->GetComponent<HealthComponent>();
		health->Damage(100);	// C R U S H E D.
	}
}

Component * Obstruction::Create(json json)
{
	auto c = ComponentManager<UpdatableComponent>::Instance().Create<Obstruction>();
	return c;
}

PrefabRegistrar Obstruction::reg("Obstruction", &Obstruction::Create);
