#pragma once

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif // !GLM_ENABLE_EXPERIMENTAL

#include <iomanip>
#include <glm/gtx/string_cast.hpp>
#include "Event/EventManager.h"
#include "Core/UpdatableComponent.h"
#include "Core/Entity.h"
#include "InputSystem.h"

// Test component for mouse movement

class MouseMovement : public UpdatableComponent
{
public:
	MouseMovement()
	{
		std::cout << std::setprecision(2);
		EventManager::Subscribe(EventName::INPUT_AXIS, this);
		EventManager::Subscribe(EventName::INPUT_BUTTON, this);
	};

	~MouseMovement()
	{
		EventManager::Unsubscribe(EventName::INPUT_AXIS, this);
		EventManager::Unsubscribe(EventName::INPUT_BUTTON, this);
	};

	virtual void Update(float deltaTime)
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

		// std::cout << "\rMouse moving: " << move.x << "," << move.y << std::flush;
	}

	virtual void Notify(EventName eventName, Param *params) override
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
		}
	}


public:
	int player = 0;
	float speed = 10.0f;

private: 
	float moveX; 
	float moveY;
	float aimX;
	float aimY;
	bool shoot;
};

