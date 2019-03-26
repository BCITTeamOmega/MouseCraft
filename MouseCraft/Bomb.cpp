#include "Bomb.h"

#include "Mouse.h"

const float Bomb::RADIUS	= 10.0f;
const float Bomb::LIFETIME	= 2.0f;
const float Bomb::SPEED		= 12.5f;
const int Bomb::DAMAGE		= 2;


Bomb::Bomb()
{
}


Bomb::~Bomb()
{
	Explode();
}

void Bomb::OnInitialized()
{
	Contraption::OnInitialized();
	_timed = GetEntity()->GetComponent<TimedDestruction>();
	_dcollision = GetEntity()->GetComponent<DamageOnCollision>();
}

bool Bomb::use(Mouse* m) {
	std::cout << "BOMB is being used" << std::endl;

	// launching position
	auto dir = GetEntity()->t().wForward();
	auto pos = GetEntity()->t().wPos() + dir * 5.0f;
	auto up = GetEntity()->GetParent()->GetComponent<PhysicsComponent>()->isUp;

	auto ptype = up ? PhysObjectType::PROJECTILE_UP : PhysObjectType::PROJECTILE_DOWN;
	_physics = PhysicsManager::instance()->createObject(pos.x, pos.y, 4, 4, 0, ptype);

	// need to dissapear after exploding
	GetEntity()->SetParent(OmegaEngine::Instance().GetRoot());
	GetEntity()->transform.setLocalPosition(pos);	// remember physics will override this 

	// make contraption "active"
	_physics->SetEnabled(true);
	_timed->SetEnabled(true);
	_dcollision->SetEnabled(true);
	
	// configure final settings 
	_checkFor =  
	{
		PhysObjectType::CONTRAPTION_DOWN,
		PhysObjectType::CAT_DOWN,
		PhysObjectType::WALL,
		PhysObjectType::CONTRAPTION_UP,
		PhysObjectType::CAT_UP
	};
	_dcollision->SetLayers(_checkFor);
	_physics->moveBody(new Vector2D(pos.x, pos.z), 0);
	auto vel = glm::vec2(dir.x, dir.z) * SPEED;
	_physics->velocity = Vector2D(vel);

	return true;
}

void Bomb::show() {
	//Contraption::use();
}

void Bomb::Explode()
{
	auto pos = GetEntity()->t().wPos();
	auto bl = pos + glm::vec3(-1, 0, -1) * RADIUS;
	auto tr = pos + glm::vec3(1, 0, 1) * RADIUS;
	auto hits = PhysicsManager::instance()->areaCheck(nullptr, _checkFor, new Vector2D(bl.x, bl.z), new Vector2D(tr.x, tr.z));

	for (auto p : hits)
	{
		auto health = p->GetEntity()->GetComponent<HealthComponent>();
		if (health) health->Damage(DAMAGE);
	}
}
