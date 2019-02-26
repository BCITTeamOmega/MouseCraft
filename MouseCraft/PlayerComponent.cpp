#include "PlayerComponent.h"
#include "Input/InputSystem.h"

void PlayerComponent::OnInitialized()
{
	EventManager::Subscribe(EventName::INPUT_AXIS_2D, this);
	_entity = GetEntity();
	_health = _entity->GetComponent<HealthComponent>();
}

void PlayerComponent::Notify(EventName eventName, Param* params)
{
	UpdatableComponent::Notify(eventName, params);

	if (eventName == EventName::INPUT_AXIS_2D)
	{
		auto data = static_cast<TypeParam<Axis2DEvent>*>(params)->Param;
		
		if (data.player != _playerID)
			return;
		
		if (data.axis == Axis::LEFT)
		{
			_move = data.GetClamped();
		}
		else if (data.axis == Axis::RIGHT)
		{
			_aim = data.GetClamped();
		}
		else
		{
			// do nothing
		}
	}
}

void PlayerComponent::Update(float deltaTime)
{
	// movement 
	_entity->transform.translate(glm::vec3(_move.x * deltaTime * _speed, 0, _move.y * deltaTime * _speed));
	// rotation
	if (_move.x != 0 && _move.y != 0)
	{
		auto rotation = glm::atan(_move.x / _move.y);
		if (_move.y > 0)
			rotation -= M_PI;
		_entity->transform.setLocalRotation(glm::vec3(0, rotation, 0));
	}
}
