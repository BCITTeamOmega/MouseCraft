#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include <climits>
#include <glm/glm.hpp>
#include "../Core/System.h"
#include "../Event/EventManager.h"

enum Axis
{
	LEFT_HOR,
	LEFT_VER,
	RIGHT_HOR,
	RIGHT_VER,
	LEFT,		// tentative
	RIGHT,		// tentative
};

enum Button
{
	PRIMARY,	// R1				(idx5)
	SECONDARY,	// L1				(idx4)
	AUX1,		// SOUTH BUTTON		(idx0)
	AUX2,		// WEST BUTTON		(idx2)
};

struct AxisEvent
{
	int player;
	Axis axis;
	float value;

	AxisEvent(int p, Axis a, float v)
		: player(p), axis(a), value(v) {}
};

struct ButtonEvent
{
	int player;
	Button button;
	bool isDown;
	
	ButtonEvent(int p, Button b, bool d)
		: player(p), button(b), isDown(d) {}
};

/*
struct AnalogEvent
{
	float value;
	bool isVertical;
	AnalogEvent(float v, bool isVer) : value(v), isVertical(isVer)
	{
	}
	AnalogEvent(Sint16 v, bool isVer) 
		: value((float)v / (float)INT16_MAX), isVertical(isVer)
	{
	}
};
*/

class InputSystem : public System
{
	// class that helps parse 2d input for SDL.
	// Optionally call SetX() and/or SetY()
	// Always call Update() once per frame, after all input has been read. 
	// Use Has...Changed()
	class Axis2DInput
	{
	public:
		// shared 
		float deadzone = 0.1f;
		float rawX;					// raw current value
		float rawY;					// raw current value
		// individual 
		float lastX;
		float lastY;
		bool xChanged = false;
		bool yChanged = false;
		// axis
		glm::vec2 lastAxis;
		bool axisChanged = false;


		void SetX(float x)
		{
			rawX = x;
		};

		void SetY(float y)
		{
			rawY = y;
		};

		void Update()
		{
			xChanged = false;
			yChanged = false;
			axisChanged = false;

			// parse indiv - x
			if (lastX == 0)
			{
				// dead 
				if (abs(rawX) > deadzone)
				{
					xChanged = true;
					lastX = rawX;
				}
				// else ignore 
			}
			else
			{
				// active 
				if (abs(rawX) < deadzone)
				{
					lastX = 0;
				}
				else
				{
					lastX = rawX;
				}
				xChanged = true;
			}

			// parase indiv - y
			if (lastY == 0)
			{
				// dead 
				if (abs(rawY) > deadzone)
				{
					yChanged = true;
					lastY = rawY;
				}
				// else ignore 
			}
			else
			{
				// active 
				if (abs(rawY) < deadzone)
				{
					lastY = 0;
				}
				else
				{
					lastY = rawY;
				}
				yChanged = true;
			}

			// parse axis 
			// note: possible for x and y not to change but for axis to change. (x=0.9, y=0.9, axis>1)
			auto length = sqrt(rawX*rawX + rawY * rawY);
			if (lastAxis == glm::vec2())
			{
				// dead 
				if (length > deadzone)
				{
					axisChanged = true;
					lastAxis = glm::vec2(rawX, rawY);
				}
			}
			else
			{
				// active 
				if (length < deadzone)
				{
					lastAxis = glm::vec2();
				}
				else
				{
					lastAxis = glm::vec2(rawX, rawY);
				}
				axisChanged = true;
			}
		}

		bool HasXChanged()
		{
			return xChanged;
		}

		bool HasYChanged()
		{
			return yChanged;
		}

		bool HasAxisChanged()
		{
			return axisChanged;
		}

		float GetX()
		{
			return lastX;
		}

		float GetY()
		{
			return lastY;
		}

		glm::vec2 GetAxis()
		{
			return lastAxis;
		}
	};


public:
	InputSystem()
	{
		SDL_InitSubSystem(SDL_INIT_JOYSTICK);
		
		// display some input info
		auto numControllers = SDL_NumJoysticks();
		if (numControllers <= 0)
		{
			std::cout << "WARNING: There are no controllers plugged in!" << std::endl;
		}
		else
		{
			std::cout << "Detected " << numControllers << " controllers." << std::endl;
			for (int i = 0; i < numControllers; ++i)
			{
				std::cout << "Controller[" << i << "]: " << SDL_JoystickNameForIndex(i) << std::endl;
				SDL_JoystickOpen(i);
			}

			if (numControllers > 4)
			{
				// Why do you have over 4 controllers? Unsure if this will have performance impact.
				std::cout << "INFO: Over 4 controllers have been opened." << std::endl;
			}
		}
	};
	~InputSystem()
	{
	};

	virtual void Update(float dt) override
	{
		SDL_Event e;
		int count = 0;


		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				// todo: omegaengine quit. 
				SDL_Quit();
			}
			else if (e.type == SDL_JOYAXISMOTION)
			{
				/*
				std::cout << "joy event" << std::endl;
				std::cout << (unsigned)e.jaxis.axis << std::endl;
				std::cout << (int)e.jaxis.value << std::endl;
				std::cout << (unsigned)e.jbutton.button << std::endl;

				std::cout << "\rjoy event: "
					<< (unsigned)e.jaxis.axis << "\t"
					<< (unsigned)e.jaxis.value << "\t"
					<< (unsigned)e.jbutton.button
					<< std::flush;
				*/

				int player = e.jaxis.which;
				Axis axis;
				float value = (float)e.jaxis.value / (float)INT16_MAX;
				
				// left analog horizontal
				if (e.jaxis.axis == 0)
				{
					axis = Axis::LEFT_HOR;
				}
				// left analog vertical 
				else if (e.jaxis.axis == 1)
				{
					axis = Axis::LEFT_VER;
				}
				// right analog horizontal 
				else if (e.jaxis.axis == 3)
				{
					axis = Axis::RIGHT_HOR;
				}
				// right analog vertical
				else if (e.jaxis.axis == 4)
				{
					axis = Axis::RIGHT_VER;
				}
				else
				{
					continue;
				}

				// notify 
				EventManager::Notify(
					EventName::INPUT_AXIS,
					new TypeParam<AxisEvent>(AxisEvent(player, axis, value)));
			}
			else if (e.type == SDL_JOYBUTTONDOWN)
			{
				int player = e.jbutton.which;
				Button b;

				switch (e.jbutton.button)
				{
				case 5:
					b = Button::PRIMARY;
					break;
				case 4:
					b = Button::SECONDARY;
					break;
				case 0:
					b = Button::AUX1;
					break;
				case 1:
					b = Button::AUX2;
					break;
				default:
					continue;
				}

				// notify
				EventManager::Notify(EventName::INPUT_BUTTON,
					new TypeParam<ButtonEvent>(ButtonEvent(player, b, true)));
			}
			else if (e.type == SDL_JOYBUTTONUP)
			{
				int player = e.jbutton.which;
				Button b;

				switch (e.jbutton.button)
				{
				case 5:
					b = Button::PRIMARY;
					break;
				case 4:
					b = Button::SECONDARY;
					break;
				case 0:
					b = Button::AUX1;
					break;
				case 1:
					b = Button::AUX2;
					break;
				default:
					continue;
				}

				// notify
				EventManager::Notify(EventName::INPUT_BUTTON,
					new TypeParam<ButtonEvent>(ButtonEvent(player, b, false)));
			}
		}
	}

// variables 
public:
	const float JOYSTICK_DEAD_ZONE = 0.1f;

private:
	bool p1move_idle = true;
	bool p1aim_idle = true;
	glm::i16vec2 p1move;
	glm::i16vec2 p1aim;
};

