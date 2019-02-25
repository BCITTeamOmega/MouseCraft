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
		addItem(other->GetEntity()->GetComponent<Pickup>());
	}
}

void Mice::addItem(Pickup* item) {

	if (baseItem == nullptr && newItem == nullptr) {
		item->Grab();
		baseItem = item;

		GetEntity()->AddChild(item->GetEntity());
		item->GetEntity()->transform.setLocalPosition(glm::vec3(2, 1, 0));
	}
	else if (baseItem != nullptr && newItem == nullptr) {
		combine(item);
	}
	else if (newItem != nullptr) {
		use(newItem);
	}
	else return;
}

void Mice::dropItem() {
	if (baseItem != nullptr || newItem != nullptr) {
		auto e = baseItem->GetEntity();
		e->SetParent(OmegaEngine::Instance().GetRoot());
		e->transform.setLocalPosition(e->transform.getWorldPosition());

		baseItem->Drop();
		baseItem = nullptr;
	}
}

void Mice::use(Contraption* item) {
	item->use();
	newItem = nullptr;
}

void Mice::combine(Pickup *material) {

	Entity* entity;
	Contraption* c = new Contraption();
	GetEntity()->AddChild(c->GetEntity());
	c->GetEntity()->transform.setLocalPosition(glm::vec3(2, 1, 0));

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
				baseItem = nullptr;
				c->type = CONTRAPTIONS::OVERCHARGE;
				entity = ContraptionFactory::Instance().Create(CONTRAPTIONS::OVERCHARGE, c->GetEntity()->transform.getLocalPosition());
				OmegaEngine::Instance().AddEntity(entity);
				newItem = c;
				break;

			case SCREW:
				baseItem = nullptr;
				c->type = CONTRAPTIONS::BOMB;
				entity = ContraptionFactory::Instance().Create(CONTRAPTIONS::OVERCHARGE, c->GetEntity()->transform.getLocalPosition());
				OmegaEngine::Instance().AddEntity(entity);
				newItem = c;
				break;

			case SPRING:
				baseItem = nullptr;
				c->type = CONTRAPTIONS::COIL;
				entity = ContraptionFactory::Instance().Create(CONTRAPTIONS::OVERCHARGE, c->GetEntity()->transform.getLocalPosition());
				OmegaEngine::Instance().AddEntity(entity);
				newItem = c;
				break;

			default:
				break;
			}
			break;

		case SCREW:
			switch (material->type)
			{
			case BATTERY:
				baseItem = nullptr;
				c->type = CONTRAPTIONS::BOMB;
				entity = ContraptionFactory::Instance().Create(CONTRAPTIONS::OVERCHARGE, c->GetEntity()->transform.getLocalPosition());
				OmegaEngine::Instance().AddEntity(entity);
				newItem = c;
				break;

			case SCREW:
				baseItem = nullptr;
				c->type = CONTRAPTIONS::SWORDS;
				entity = ContraptionFactory::Instance().Create(CONTRAPTIONS::OVERCHARGE, c->GetEntity()->transform.getLocalPosition());
				OmegaEngine::Instance().AddEntity(entity);
				newItem = c;
				break;

			case SPRING:
				baseItem = nullptr;
				c->type = CONTRAPTIONS::GUN;
				entity = ContraptionFactory::Instance().Create(CONTRAPTIONS::OVERCHARGE, c->GetEntity()->transform.getLocalPosition());
				OmegaEngine::Instance().AddEntity(entity);
				newItem = c;
				break;

			default:
				break;
			}
			break;

		case SPRING:
			switch (material->type)
			{
			case BATTERY:
				baseItem = nullptr;
				c->type = CONTRAPTIONS::COIL;
				entity = ContraptionFactory::Instance().Create(CONTRAPTIONS::OVERCHARGE, c->GetEntity()->transform.getLocalPosition());
				OmegaEngine::Instance().AddEntity(entity);
				newItem = c;
				break;

			case SCREW:
				baseItem = nullptr;
				c->type = CONTRAPTIONS::GUN;
				entity = ContraptionFactory::Instance().Create(CONTRAPTIONS::OVERCHARGE, c->GetEntity()->transform.getLocalPosition());
				OmegaEngine::Instance().AddEntity(entity);
				newItem = c;
				break;

			case SPRING:
				baseItem = nullptr;
				c->type = CONTRAPTIONS::PLATFORM;
				entity = ContraptionFactory::Instance().Create(CONTRAPTIONS::OVERCHARGE, c->GetEntity()->transform.getLocalPosition());
				OmegaEngine::Instance().AddEntity(entity);
				newItem = c;
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