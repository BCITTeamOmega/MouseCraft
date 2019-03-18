#include "Mice.h"

Mice::Mice() : 
	HandleOnCollide(this, &Mice::OnCollision),
	HandleOnDeath(this, &Mice::OnDeath),
	HandleOnHit(this, &Mice::OnHit)
{
	std::cout << std::setprecision(2);
	EventManager::Subscribe(EventName::INPUT_AXIS, this);
	EventManager::Subscribe(EventName::INPUT_BUTTON, this);
}


Mice::~Mice()
{
	EventManager::Unsubscribe(EventName::INPUT_AXIS, this);
	EventManager::Unsubscribe(EventName::INPUT_BUTTON, this);
}

void Mice::OnInitialized() 
{
	//Listens for collisions with the physics component
	PhysicsComponent* pComp = GetEntity()->GetComponent<PhysicsComponent>();
	if (pComp != nullptr)
	{
		HandleOnCollide.Observe(pComp->onCollide);
		HandleOnHit.Observe(pComp->onHit);
	}

	// Listen for death 
	HealthComponent* c_health = GetEntity()->GetComponent<HealthComponent>();
	HandleOnDeath.Observe(c_health->OnDeath);

    player = GetEntity()->GetComponent<PlayerComponent>()->GetID();
}

void Mice::Update(float deltaTime) 
{
	if (shoot)
	{
		std::cout << std::endl << "Mouse[" << player << "] - Pew pew!" << std::endl;
		shoot = false;
		
		if (newItem != nullptr)
		{
			use(newItem);
		}
	}

	if (drop)
	{
		drop = false;
		dropItem();
	}	
}

void Mice::Notify(EventName eventName, Param * params)
{
	// for updatable
	UpdatableComponent::Notify(eventName, params);

	// handle buttons
	if (eventName == EventName::INPUT_BUTTON)
	{
		auto data = static_cast<TypeParam<ButtonEvent>*>(params)->Param;

		if (data.player != player)
			return;

		if (data.button == Button::PRIMARY && data.isDown)
			shoot = true;	// or do it right away, no post processing required.

		if (data.button == Button::AUX2)
			drop = data.isDown;
	}
}

void Mice::OnCollision(PhysicsComponent * pc)
{
	if (pc->type == PhysObjectType::PART)
	{
		// collided with part 
		addItem(pc->GetEntity()->GetComponent<Pickup>());
	}
}

void Mice::OnHit(PhysicsComponent* e)
{

}

void Mice::OnDeath()
{
	// on death
	downed = true;
	GetEntity()->SetEnabled(false);
}



void Mice::addItem(Pickup* item) {

	if (newItem != nullptr) {
		return;
	}

	if (baseItem == nullptr && newItem == nullptr) {
		item->Grab();
		baseItem = item;

		GetEntity()->AddChild(item->GetEntity());
		item->GetEntity()->transform.setLocalPosition(glm::vec3(2, 1, 0));
		std::cout << "Mouse has pickup up a " << item << std::endl;
	}
	else if (baseItem != nullptr && newItem == nullptr) {
		std::cout << "Mouse will combine the " << baseItem << " and the " << item << std::endl;
		combine(item);
		
		// destroy pickups
		// baseItem destroyed in combine 
		item->GetEntity()->Destroy();


	}
	else return;
}

void Mice::dropItem() {
	
	if (baseItem != nullptr) {
		auto dropPos = GetEntity()->t().wPos() + GetEntity()->t().wForward() * 5.0f;	// drop in front of mice
		auto e = baseItem->GetEntity();
		e->SetParent(OmegaEngine::Instance().GetRoot(), true);	// forced (instant and unmanaged)	
		e->transform.setLocalPosition(dropPos);
		baseItem->Drop();
		baseItem = nullptr;
	}

	if (newItem != nullptr) {
		auto e = newItem->GetEntity();
		e->SetParent(OmegaEngine::Instance().GetRoot());
		e->transform.setLocalPosition(e->transform.getWorldPosition());

		newItem->Drop();
		newItem = nullptr;
	}
}

void Mice::use(Contraption* item) {
	
	if (item->use())
	{
		// todo: this syntax is kinda weird. should pass reference to pointer or just use newItem directly
		newItem = nullptr;	
	}
}

void Mice::combine(Pickup *material) {

	Entity* entity;

	if (baseItem == nullptr) {
		return;
	}
	else {
		switch (baseItem->type)
		{

		case BATTERY:
			switch (material->type)
			{
			case BATTERY:
				std::cout << "Mouse is creating the OVERCHARGE" << std::endl;
				baseItem->GetEntity()->Destroy();
				baseItem = nullptr;
				entity = ContraptionFactory::Instance().Create(CONTRAPTIONS::OVERCHARGE, glm::vec3(0, 0, 1));
				GetEntity()->AddChild(entity);
				newItem = entity->GetComponent<Contraption>();
				break;

			case SCREW:
				std::cout << "Mouse is creating the BOMB" << std::endl;
				baseItem->GetEntity()->Destroy();
				baseItem = nullptr;
				entity = ContraptionFactory::Instance().Create(CONTRAPTIONS::BOMB, glm::vec3(0, 0, 1));
				GetEntity()->AddChild(entity);
				newItem = entity->GetComponent<Contraption>();
				break;

			case SPRING:
				std::cout << "Mouse is creating the COIL" << std::endl;
				baseItem->GetEntity()->Destroy();
				baseItem = nullptr;
				entity = ContraptionFactory::Instance().Create(CONTRAPTIONS::COIL, glm::vec3(0, 0, 1));
				GetEntity()->AddChild(entity);
				newItem = entity->GetComponent<Contraption>();
				break;

			default:
				break;
			}
			break;

		case SCREW:
			switch (material->type)
			{
			case BATTERY:
				std::cout << "Mouse is creating the BOMB" << std::endl;
				baseItem->GetEntity()->Destroy();
				baseItem = nullptr;
				entity = ContraptionFactory::Instance().Create(CONTRAPTIONS::BOMB, glm::vec3(0, 0, 1));
				GetEntity()->AddChild(entity);
				newItem = entity->GetComponent<Contraption>();
				break;

			case SCREW:
				std::cout << "Mouse is creating the SWORDS" << std::endl;
				baseItem->GetEntity()->Destroy();
				baseItem = nullptr;
				entity = ContraptionFactory::Instance().Create(CONTRAPTIONS::SWORDS, glm::vec3(0, 0, 1));
				GetEntity()->AddChild(entity);
				newItem = entity->GetComponent<Contraption>();
				break;

			case SPRING:
				std::cout << "Mouse is creating the GUN" << std::endl;
				baseItem->GetEntity()->Destroy();
				baseItem = nullptr;
				entity = ContraptionFactory::Instance().Create(CONTRAPTIONS::GUN, glm::vec3(0, 0, 1));
				GetEntity()->AddChild(entity);
				newItem = entity->GetComponent<Contraption>();
				break;

			default:
				break;
			}
			break;

		case SPRING:
			switch (material->type)
			{
			case BATTERY:
				std::cout << "Mouse is creating the COIL" << std::endl;
				baseItem->GetEntity()->Destroy();
				baseItem = nullptr;
				entity = ContraptionFactory::Instance().Create(CONTRAPTIONS::COIL, glm::vec3(0, 0, 1));
				GetEntity()->AddChild(entity);
				newItem = entity->GetComponent<Contraption>();
				break;

			case SCREW:
				std::cout << "Mouse is creating the GUN" << std::endl;
				baseItem->GetEntity()->Destroy();
				baseItem = nullptr;
				entity = ContraptionFactory::Instance().Create(CONTRAPTIONS::GUN, glm::vec3(0, 0, 1));
				GetEntity()->AddChild(entity);
				newItem = entity->GetComponent<Contraption>();
				break;

			case SPRING:
				std::cout << "Mouse is creating the PLATFORM" << std::endl;
				baseItem->GetEntity()->Destroy();
				baseItem = nullptr;
				entity = ContraptionFactory::Instance().Create(CONTRAPTIONS::PLATFORM, glm::vec3(0, 0, 1));
				GetEntity()->AddChild(entity);
				newItem = entity->GetComponent<Contraption>();
				break;

			default:
				break;
			}
			break;

		default:
			break;
		}
	}
}