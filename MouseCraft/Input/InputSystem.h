#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include <climits>
#include <glm/glm.hpp>
#include <array>
#include "../Core/System.h"
#include "../Event/EventManager.h"
#include "../Util/CpuProfiler.h"

#define MAX_PLAYERS 4
#define JOYSTICK_DEADZONE 0.1
#define DEBUG_PLAYER 10

// Defines the possible axis this game supports.
// Note: Order matters, should always be Master, Horizontal, Vertical.
enum Axis
{
	LEFT,		
	LEFT_HOR,
	LEFT_VER,
	RIGHT,		
	RIGHT_HOR,
	RIGHT_VER,
};

enum Button
{
	PRIMARY,	// R1				(idx5)
	SECONDARY,	// L1				(idx4)
	AUX1,		// SOUTH BUTTON		(idx0)
    AUX2,		// WEST BUTTON		(idx2)
    OPTION,		// OPTION BUTTON	(idx6)
};

struct AxisEvent
{
	int player;
	Axis axis;
	float value;

	AxisEvent(int p, Axis a, float v)
		: player(p), axis(a), value(v) {}
};

struct Axis2DEvent
{
	int player;
	Axis axis;
	glm::vec2 value;

	Axis2DEvent(int p, Axis a, glm::vec2 v)
		: player(p), axis(a), value(v) {}

	// returns value normalized (length of 1)
	glm::vec2 GetDir()
	{
		return glm::normalize(value);
	}

	// returns value with length clamped between 0-1
	glm::vec2 GetClamped()
	{
		return (glm::length(value) > 1.0f) ? glm::normalize(value) * 1.0f : value;
	}
};

struct ButtonEvent
{
	int player;
	Button button;
	bool isDown;
	
	ButtonEvent(int p, Button b, bool d)
		: player(p), button(b), isDown(d) {}
};

class InputSystem : public System
{
private:
	// Internal class that helps parse 2D axis input for SDL. 
	// This class handles deadzone filtering and determining 
	// if a value has logically changed. 
	// 
	// Instructions: 
	// 1. Optionally call SetX() and/or SetY()
	// 2. Always call Update() once per frame, after all input has been read. 
	// 3. Use HasXXXChanged() to check if input has logically changed
	// 4. Use GetXXX() to retrieve input
	class Axis2DInput
	{
	// variables
	private:
		// shared 
		float deadzone = JOYSTICK_DEADZONE;
		float rawX;					// raw current value
		float rawY;					// raw current value
		bool xSet = false;			// if x-axis change has been raised by SDL
		bool ySet = false;			// if y-axis change has been raised by SDL
		// individual 
		float lastX;
		float lastY;
		bool xChanged = false;		// if x-axis has logically changed
		bool yChanged = false;		// if y-axis has logically changed
		// axis
		glm::vec2 lastAxis;
		bool axisChanged = false;

	// functions
	public:
		void SetX(float x);

		void SetY(float y);

		void Update();

		bool HasXChanged() const
		{
			return xChanged;
		}

		bool HasYChanged() const 
		{
			return yChanged;
		}

		bool HasAxisChanged() const
		{
			return axisChanged;
		}

		float GetX() const
		{
			return lastX;
		}

		float GetY() const
		{
			return lastY;
		}

		glm::vec2 GetAxis() const
		{
			return lastAxis;
		}
	};

// variables 
private:
	std::array<Axis2DInput, MAX_PLAYERS * 2> playerAxes;
	Axis2DInput debugPlayerAxis;	// special axis for keyboard input
	CpuProfiler profiler;

// functions
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
			for (int i = 0; i < numControllers && i < MAX_PLAYERS; ++i)
			{
				std::cout << "Controller[" << i << "]: " << SDL_JoystickNameForIndex(i) << std::endl;
				SDL_JoystickOpen(i);
			}

			if (numControllers > MAX_PLAYERS)
			{
				// Why do you have over 4 controllers? Unsure if this will have performance impact.
				std::cout << "WARNING: Up to 4 controllers can be opened." << std::endl;
			}
		}

		profiler.InitializeTimers(1);
		profiler.LogOutput("Input.log");
	};
	~InputSystem()
	{
	};

	virtual void Update(float dt) override;

private:
	void NotifyAxis(Axis2DInput& axis, Axis which, int player);
};

