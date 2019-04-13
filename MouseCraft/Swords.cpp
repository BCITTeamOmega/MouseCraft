#include "Swords.h"

#include "Mouse.h"

Swords::Swords() :
	HandleOnCollision(this, &Swords::OnCollision)

{
	Contraption::type = CONTRAPTIONS::SWORDS;
}


Swords::~Swords()
{
}

bool Swords::use(Mouse* m) {
	std::cout << "SWORDS is being used" << std::endl;
	_phys = GetEntity()->GetComponent<PhysicsComponent>();
	bool isUp = GetEntity()->GetParent()->GetComponent<PhysicsComponent>()->isUp;

	if (isUp) {
		checkFor.insert(PhysObjectType::CAT_UP);
		//checkFor.insert(PhysObjectType::OBSTACLE_UP);
	}
	else 
	{
		checkFor.insert(PhysObjectType::CAT_DOWN);
		//checkFor.insert(PhysObjectType::OBSTACLE_DOWN);
	}

	auto p1 = GetEntity()->transform;
	auto pos = p1.getWorldPosition() + p1.getWorldForward() * 5.0f;
	auto bl = pos + glm::vec3(-RADIUS, 0, -RADIUS);
	auto tr = pos + glm::vec3(RADIUS, 0, RADIUS);

	auto hits = _phys->areaCheck(checkFor, new Vector2D(bl.x, bl.z), new Vector2D(tr.x, tr.z));
	bool hit = hits.size() > 0;

	if (isUp && !_collidedObjects && hit) {
		/**
		auto t1 = std::find_if(target.begin(), target.end(), [&checkFor](const PhysicsComponent* p) {
			return p->type == PhysObjectType::CAT_UP; });

		auto t2 = std::find_if(target.begin(), target.end(), [&checkFor](const PhysicsComponent* p) {
			return p->type == PhysObjectType::OBSTACLE_UP; });
			*/

		std::cout << "Swords hit Object" << std::endl;
		_collidedObjects = hits[0];
		_collidedObjects->GetEntity()->GetComponent<HealthComponent>()->Damage(DAMAGE);
        if (hits[0]->pType == PhysObjectType::CAT_UP) {
            //play cat hit sound
            hits[0]->GetEntity()->GetComponent<SoundComponent>()->ChangeSound(SoundsList::CatScream); //set sound to squeak for mouse
            auto targetPos = hits[0]->GetEntity()->transform.getLocalPosition(); //get mouse current position
            hits[0]->GetEntity()->GetComponent<SoundComponent>()->PlaySound(targetPos.x, targetPos.y, targetPos.z); //play sound
        }
		this->GetEntity()->Destroy();

		return true;
		
	}
	else if (!isUp && !_collidedObjects && hit)
	{
		/**
		auto t1 = std::find_if(target.begin(), target.end(), [&checkFor](const PhysicsComponent* p) {
			return p->type == PhysObjectType::CAT_DOWN; });

		auto t2 = std::find_if(target.begin(), target.end(), [&checkFor](const PhysicsComponent* p) {
			return p->type == PhysObjectType::OBSTACLE_DOWN; });
		*/

		std::cout << "Swords hit Object" << std::endl;
		_collidedObjects = hits[0];
		_collidedObjects->GetEntity()->GetComponent<HealthComponent>()->Damage(DAMAGE);
        if (hits[0]->pType == PhysObjectType::CAT_DOWN) {
            //play cat hit sound
            hits[0]->GetEntity()->GetComponent<SoundComponent>()->ChangeSound(SoundsList::CatScream); //set sound to squeak for mouse
            auto targetPos = hits[0]->GetEntity()->transform.getLocalPosition(); //get mouse current position
            hits[0]->GetEntity()->GetComponent<SoundComponent>()->PlaySound(targetPos.x, targetPos.y, targetPos.z); //play sound
        }
		this->GetEntity()->Destroy();

		return true;
	} 
	else if (_collidedObjects && !hit) {
		_collidedObjects = nullptr;
	}

	return false;

}

void Swords::show() {
	Contraption::show();
}

void Swords::OnCollision(PhysicsComponent* other) {

}

void Swords::OnInitialized() {
	Contraption::OnInitialized();
}

void Swords::Update(float dt) {
	
}

Component* Swords::Create(json json) 
{
	auto c = ComponentManager<Swords>::Instance().Create<Swords>();
	return c;
}

PrefabRegistrar Swords::reg("Swords", Swords::Create);