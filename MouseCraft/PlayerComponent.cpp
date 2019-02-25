#include "PlayerComponent.h"
#include "Input/InputSystem.h"

unsigned int PlayerComponent::_curID = 0;

void PlayerComponent::OnInitialized()
{
	EventManager::Subscribe(EventName::INPUT_AXIS, this);
	_entity = GetEntity();
	_health = _entity->GetComponent<HealthComponent>();
	_playerID = _curID++;
}

void PlayerComponent::Notify(EventName eventName, Param* params)
{
	UpdatableComponent::Notify(eventName, params);

	if (eventName == EventName::INPUT_AXIS)
	{
		auto data = static_cast<TypeParam<AxisEvent>*>(params)->Param;

		if (data.player != _playerID)
			return;

		switch (data.axis)
		{
		case Axis::LEFT_HOR:
			_moveX = data.value;
			break;
		case Axis::LEFT_VER:
			_moveY = data.value;
			break;
		case Axis::RIGHT_HOR:
			_aimX = data.value;
			break;
		case Axis::RIGHT_VER:
			_aimY = data.value;
			break;
		default:
			break;
		}
	}
}

void PlayerComponent::Update(float deltaTime)
{
	auto move = glm::vec2(_moveX, _moveY);
	// normalize if over 1.0 (eg. corner)
	if (glm::length(move) > 1.0f)
		move = glm::normalize(move);

	_entity->transform.translate(glm::vec3(move.x * deltaTime * _speed, 0, move.y * deltaTime * _speed));
}
