#include "Bomb.h"

#include "Mouse.h"

const float Bomb::RADIUS	= 10.0f;
const float Bomb::LIFETIME	= 2.0f;
const float Bomb::SPEED		= 12.5f;
const int   Bomb::DAMAGE	= 2;
const float Bomb::EXPLOSION_LIFETIME = 0.6f;
// curve isn't perfect as it cannot go over 3 height (threshold).
const float Bomb::VER_VEL = 8.8f;

Bomb::Bomb()
{
	Contraption::type = CONTRAPTIONS::BOMB;
}

Bomb::~Bomb()
{
	if (_activated)
		Explode();
}

void Bomb::OnInitialized()
{
	Contraption::OnInitialized();
	_physics = GetEntity()->GetComponent<PhysicsComponent>();
	_timed = GetEntity()->GetComponent<TimedDestruction>();
	_dcollision = GetEntity()->GetComponent<DamageOnCollision>();
	_rotator = GetEntity()->GetComponent<Rotator>();
}

bool Bomb::use(Mouse* m) {
	std::cout << "BOMB is being used" << std::endl;

	_activated = true;

	// launching position
	auto dir = GetEntity()->t().wForward();
	auto pos = GetEntity()->t().wPos() + dir * 5.0f;
	auto up = GetEntity()->GetParent()->GetComponent<PhysicsComponent>()->isUp;

	// need to dissapear after exploding
	GetEntity()->SetParent(OmegaEngine::Instance().GetRoot());
	GetEntity()->transform.setLocalPosition(pos);	// remember physics will override this 
	_physics->zPos = (up) ? Z_UPPER : Z_LOWER;		// TODO: not 100% sure if PhysicsManager will automatically resolve masking
	_physics->moveBody(new Vector2D(pos.x, pos.z), 0);

	// make contraption "active"
	_physics->SetEnabled(true);
	_timed->SetEnabled(true);
	_dcollision->SetEnabled(true);
	_rotator->SetEnabled(true);
	
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
	_physics->zVelocity = Bomb::VER_VEL;
	_physics->isFalling = true;
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

	OmegaEngine::Instance().AddEntity(explosion);
	explosion->transform.setLocalPosition(GetEntity()->transform.getWorldPosition());
}

Component* Bomb::Create(json json)
{
	auto c = ComponentManager<Bomb>::Instance().Create<Bomb>();
	return c;
}

PrefabRegistrar Bomb::reg("Bomb", Bomb::Create);