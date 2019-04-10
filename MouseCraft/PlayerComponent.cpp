#include "PlayerComponent.h"
#include "Input/InputSystem.h"

PlayerComponent::PlayerComponent() :
	handleStop(this, &PlayerComponent::StopMoving),
	handleResume(this, &PlayerComponent::ResumeMoving)
{
	
}

void PlayerComponent::OnInitialized()
{
	EventManager::Subscribe(EventName::INPUT_AXIS_2D, this);
	_entity = GetEntity();
	_health = _entity->GetComponent<HealthComponent>();
	_physicsComponent = _entity->GetComponent<PhysicsComponent>();

	handleStop.Observe(_physicsComponent->stopMoving);
	handleResume.Observe(_physicsComponent->resumeMoving);
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

void PlayerComponent::StopMoving()
{

}

void PlayerComponent::ResumeMoving()
{

}

void PlayerComponent::Update(float deltaTime)
{
	if (_isDisabled)
		return;
	// backwards compatibility for now 
	if (_physicsComponent == nullptr)
	{
		_entity->transform.translate(glm::vec3(_move.x * deltaTime * _speed, 0, _move.y * deltaTime * _speed));
	}
	else
	{
		_physicsComponent->velocity = _move * _speed;
	}
	// rotation
	if (_move.x != 0 || _move.y != 0)
	{
		_entity->transform.face2D(_move);
	}
}

Component * PlayerComponent::Create(json json)
{
	auto c = ComponentManager<UpdatableComponent>::Instance().Create<PlayerComponent>();
	
	c->_speed = json["speed"].get<float>();
	
	auto team = json["team"].get<std::string>();
	if (team == "mouse")
		c->_teamID = Team::MOUSE;
	else if (team == "cat")
		c->_teamID = Team::CAT;
	else
		SDL_assert(false && "UNKNOWN TEAM");

	if (json.find("id") != json.end())
		c->_playerID = json["id"].get<int>();

	return c;
}

PrefabRegistrar PlayerComponent::reg("Player", &PlayerComponent::Create);