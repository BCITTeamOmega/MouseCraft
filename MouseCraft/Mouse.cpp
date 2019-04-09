#include "Mouse.h"

Mouse::Mouse() : 
	HandleOnCollide(this, &Mouse::OnCollision),
	HandleOnDeath(this, &Mouse::OnDeath),
	HandleOnHit(this, &Mouse::OnHit),
	HandleOnBounce(this, &Mouse::OnBounce),
	HandleOnRevive(this, &Mouse::OnRevived)
{
	std::cout << std::setprecision(2);
	EventManager::Subscribe(EventName::INPUT_AXIS, this);
	EventManager::Subscribe(EventName::INPUT_BUTTON, this);
}


Mouse::~Mouse()
{
	EventManager::Unsubscribe(EventName::INPUT_AXIS, this);
	EventManager::Unsubscribe(EventName::INPUT_BUTTON, this);
}

void Mouse::OnInitialized() 
{
	//Listens for collisions with the physics component
	_phys = GetEntity()->GetComponent<PhysicsComponent>();
	if (_phys != nullptr)
	{
		HandleOnCollide.Observe(_phys->onCollide);
		HandleOnHit.Observe(_phys->onHit);
		HandleOnBounce.Observe(_phys->onBounce);
	}

	// Listen for death 
	HealthComponent* c_health = GetEntity()->GetComponent<HealthComponent>();
	HandleOnDeath.Observe(c_health->OnDeath);
	HandleOnRevive.Observe(c_health->OnRevive);

    player = GetEntity()->GetComponent<PlayerComponent>()->GetID();

	render = GetEntity()->GetComponent<Renderable>();
	initialColor = render->getColor();
}

void Mouse::Update(float deltaTime) 
{
	if (downed) 
		return;
	
	if (interact)
	{
		interact = false;
		float RADIUS = 5.0f;
		auto p1 = GetEntity()->transform;
		auto pos = p1.getWorldPosition() + p1.getWorldForward() * 5.0f;
		auto bl = pos + glm::vec3(-RADIUS, 0, -RADIUS);
		auto tr = pos + glm::vec3(RADIUS, 0, RADIUS);
		bool isUp = GetEntity()->GetComponent<PhysicsComponent>()->isUp;

		if (isUp) {
			checkFor.insert(PhysObjectType::MOUSE_UP);
			checkFor.insert(PhysObjectType::PART);
		}
		else
		{
			checkFor.insert(PhysObjectType::MOUSE_DOWN);
			checkFor.insert(PhysObjectType::PART);
		}

		auto hits = _phys->areaCheck(checkFor, new Vector2D(bl.x, bl.z), new Vector2D(tr.x, tr.z));
		bool hit = hits.size() > 0;

		for (auto pc : hits)
		{
			if (pc->pType == PhysObjectType::MOUSE_DOWN || pc->pType == PhysObjectType::MOUSE_UP)
			{
				revive(pc);
			}
			else
			{
				addItem(pc->GetEntity()->GetComponent<Pickup>());
			}
		}
	}

	if (shoot && newItem != nullptr) 
	{
		shoot = false;
		use(newItem);
	}

	if (drop)
	{
		drop = false;
		dropItem();
	}

	_phys->updateFalling();
}

void Mouse::Notify(EventName eventName, Param * params)
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
			interact = true;	// or do it right away, no post processing required.

		if (data.button == Button::AUX1)
			shoot = data.isDown;

		if (data.button == Button::AUX2)
			drop = data.isDown;

	}
}

void Mouse::OnCollision(PhysicsComponent * pc)
{

}

void Mouse::OnHit(PhysicsComponent* e)
{

}

void Mouse::OnBounce(PhysicsComponent* e)
{
	PhysicsComponent* pComp = GetEntity()->GetComponent<PhysicsComponent>();

	//position of mouse
	Vector2D* curPos = new Vector2D(GetEntity()->transform.getLocalPosition().x, GetEntity()->transform.getLocalPosition().z);
	//vector in front of cat of length = JUMP_DIST
	Vector2D* jumpVec = new Vector2D(GetEntity()->transform.getLocalForward().x * MOUSE_JUMP_DIST, GetEntity()->transform.getLocalForward().z * MOUSE_JUMP_DIST);
	jumpVec = new Vector2D(*curPos + *jumpVec);

	std::set<PhysObjectType::PhysObjectType> types = std::set<PhysObjectType::PhysObjectType>{
		PhysObjectType::PLATFORM
	};

	Vector2D* hitPos = new Vector2D(0, 0);

	PhysicsComponent* jumpTarget = pComp->rayCheck(types, curPos, jumpVec, *hitPos);

	//check if we are in a location we can jump in
	if (jumpTarget != nullptr) {
		//Jump code
		std::cout << "Mouse has jumped." << std::endl;
		GetEntity()->GetComponent<PhysicsComponent>()->jump();

		GetEntity()->GetComponent<SoundComponent>()->ChangeSound(SoundsList::Jump); //set sound to jump
		auto pos = GetEntity()->transform.getLocalPosition(); //get our current position
		GetEntity()->GetComponent<SoundComponent>()->PlaySound(pos.x, pos.y, pos.z); //play sound
		return;
	}
}

void Mouse::OnDeath()
{
	// on death
	downed = true;
	GetEntity()->GetComponent<PlayerComponent>()->SetEnabled(false);
	render->setColor(Color(0, 0, 0));
	_phys->velocity = Vector2D(0, 0);
}

void Mouse::OnRevived()
{
	downed = false;
	GetEntity()->GetComponent<PlayerComponent>()->SetEnabled(true);
	render->setColor(initialColor);
}

void Mouse::addItem(Pickup* item) {

	if (newItem != nullptr) {
		return;
	}

	if (baseItem == nullptr && newItem == nullptr) {
		item->Grab();
		baseItem = item;

		GetEntity()->AddChild(item->GetEntity());
		item->GetEntity()->transform.setLocalPosition(glm::vec3(2, 1, 0));
		std::cout << "Mouse has picked up a " << item << std::endl;
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

void Mouse::dropItem() {
	
	if (baseItem != nullptr) {
		auto dropPos = GetEntity()->t().wPos() + GetEntity()->t().wForward() * 5.0f;	// drop in front of Mouse
		auto e = baseItem->GetEntity();
		e->SetParent(OmegaEngine::Instance().GetRoot(), true);	// forced (instant and unmanaged)	
		e->transform.setLocalPosition(dropPos);
		baseItem->Drop();
		baseItem = nullptr;
	}

	if (newItem != nullptr) {
		Entity* part1;
		Entity* part2;
		auto pos = GetEntity()->transform.getWorldPosition() + GetEntity()->transform.getWorldForward() * 10.0f;
		auto p1 = pos + glm::vec3(-10.0f, 0, 0);
		auto p2 = pos + glm::vec3(10.0f, 0, 0);

		auto type = newItem->type;

		if (type == CONTRAPTIONS::BOMB) 
		{
			part1 = PickupFactory::Instance().Create(PICKUPS::SCREW, p1);
			part2 = PickupFactory::Instance().Create(PICKUPS::BATTERY, p2);
		} 
		else if (type == CONTRAPTIONS::COIL) 
		{
			part1 = PickupFactory::Instance().Create(PICKUPS::BATTERY, p1);
			part2 = PickupFactory::Instance().Create(PICKUPS::SPRING, p2);
		} 
		else if (type == CONTRAPTIONS::GUN) 
		{
			part1 = PickupFactory::Instance().Create(PICKUPS::SCREW, p1);
			part2 = PickupFactory::Instance().Create(PICKUPS::SPRING, p2);
		} 
		else if (type == CONTRAPTIONS::OVERCHARGE) 
		{
			part1 = PickupFactory::Instance().Create(PICKUPS::BATTERY, p1);
			part2 = PickupFactory::Instance().Create(PICKUPS::BATTERY, p2);
		} 
		else if (type == CONTRAPTIONS::SWORDS) 
		{
			part1 = PickupFactory::Instance().Create(PICKUPS::SCREW, p1);
			part2 = PickupFactory::Instance().Create(PICKUPS::SCREW,  p2);
		} 
		else 
		{
			part1 = PickupFactory::Instance().Create(PICKUPS::SPRING, p1);
			part2 = PickupFactory::Instance().Create(PICKUPS::SPRING, p2);
		}

		OmegaEngine::Instance().AddEntity(part1);
		OmegaEngine::Instance().AddEntity(part2);
		newItem->GetEntity()->Destroy();
		newItem = nullptr;
	}
}

void Mouse::use(Contraption* item) {
	
	if (item->use(this))
	{
		// todo: this syntax is kinda weird. should pass reference to pointer or just use newItem directly
		newItem = nullptr;	
	}
}

void Mouse::combine(Pickup *material) {

	Entity* entity;

	if (baseItem == nullptr) {
		return;
	}
	else {
		PhysicsComponent* pComp = GetEntity()->GetComponent<PhysicsComponent>();

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
				std::cout << "Mouse is creating the TRAMPOLINE" << std::endl;
				baseItem->GetEntity()->Destroy();
				baseItem = nullptr;
				entity = ContraptionFactory::Instance().Create(CONTRAPTIONS::TRAMPOLINE, glm::vec3(0, 0, 1));
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

void Mouse::revive(PhysicsComponent* mouse) {
	
	mouse->GetEntity()->GetComponent<HealthComponent>()->SetHealth(1);
}

Component* Mouse::Create(json json)
{
	auto c = ComponentManager<Mouse>::Instance().Create<Mouse>();
	c->speed = json["speed"].get<float>();
	c->downed = json["downed"].get<bool>();
	c->player = json["player"].get<int>();
}

PrefabRegistrar Mouse::reg("Mouse", Mouse::Create);
