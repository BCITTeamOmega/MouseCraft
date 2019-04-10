#include "Obstruction.h"

#include "HealthComponent.h"

const int Obstruction::BOX_SIZE = 9;
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
	auto bl = new Vector2D(pos - halfsize);
	auto tr = new Vector2D(pos + halfsize);
	grid->removeArea(bl, tr);

	// now check 
	auto nbl = new Vector2D(newPos - halfsize);
	auto ntr = new Vector2D(newPos + halfsize);
	auto hits = grid->objectsInArea(ntr, nbl);

	std::cout << "OBSTRUCTION CHECKING" << std::endl;
	std::cout << "original: " << pos.x << "," << pos.y << " attempt: " << newPos.x << "," << newPos.y << std::endl;
	std::cout << "bl: " << nbl->x << "," << nbl->y << " tr: " << ntr->x << "," << ntr->y << std::endl;

	if (hits)
	{
		for (auto pc : *hits)
		{
			auto p = pc->GetEntity()->transform.getWorldPosition2D();
			b2Vec2 foo = pc->body->GetPosition();
			std::cout << "hit something at " << foo.x << "," << foo.y << std::endl;
		}
	}
	else
	{
		std::cout << "no hits" << std::endl;
	}

	bool stop = false;
	if (hits)
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
	}

	if (stop)
	{
		// oh no git reset --HARD
		grid->createArea(*bl, *tr, _physics, _physics->pType);
	}
	else
	{
		// hell yea, knock this chonk over there 
		for (auto pc : *hits)
			if (pc)
				pc->GetEntity()->Destroy();
		grid->createArea(*nbl, *ntr, _physics, _physics->pType);
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
