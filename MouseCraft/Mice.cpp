#include "Mice.h"

Mice::Mice()
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
	GetEntity()->GetComponent<DebugColliderComponent>()->OnCollide.Attach(this);
}

void Mice::Update(float deltaTime) 
{
	auto move = glm::vec2(moveX, moveY);
	// normalize if over 1.0 (eg. corner)
	if (glm::length(move) > 1.0f)
		move = glm::normalize(move);

	GetEntity()->transform.translate(
		glm::vec3(move.x * deltaTime * speed, 0, move.y * deltaTime * speed));

	if (shoot)
	{
		std::cout << std::endl << "Mouse[" << player << "] - Pew pew!" << std::endl;
		shoot = false;
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

	if (eventName == EventName::INPUT_AXIS)
	{
		auto data = static_cast<TypeParam<AxisEvent>*>(params)->Param;

		if (data.player != player)
			return;

		switch (data.axis)
		{
		case Axis::LEFT_HOR:
			moveX = data.value;
			break;
		case Axis::LEFT_VER:
			moveY = data.value;
			break;
		case Axis::RIGHT_HOR:
			aimX = data.value;
			break;
		case Axis::RIGHT_VER:
			aimY = data.value;
			break;
		default:
			break;
		}
	}
	else if (eventName == EventName::INPUT_BUTTON)
	{
		auto data = static_cast<TypeParam<ButtonEvent>*>(params)->Param;

		if (data.player != player)
			return;

		if (data.button == Button::PRIMARY && data.isDown)
			shoot = true;	// or do it right away, no post processing required.

		if (data.button == Button::AUX1 && data.isDown)
			std::cout << glm::to_string(GetEntity()->transform.getWorldPosition()) << std::endl;

		if (data.button == Button::AUX2)
			drop = data.isDown;
	}
}

void Mice::Publish(DebugColliderComponent* me, DebugColliderComponent* other)
{
	if (other->tag == "pickup")
	{
		std::cout << "PICKUP???" << std::endl;
		addItem(other->GetEntity()->GetComponent<Pickup>());
	}
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
	}
	else return;
}

void Mice::dropItem() {
	
	if (baseItem != nullptr) {
		auto e = baseItem->GetEntity();
		e->SetParent(OmegaEngine::Instance().GetRoot());
		e->transform.setLocalPosition(e->transform.getWorldPosition());

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
	item->use();
	newItem = nullptr;
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