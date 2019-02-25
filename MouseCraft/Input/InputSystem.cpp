#include "InputSystem.h"

void InputSystem::Axis2DInput::SetX(float x)
{
	xSet = true;
	rawX = x;
}

void InputSystem::Axis2DInput::SetY(float y)
{
	ySet = true;
	rawY = y;
}

void InputSystem::Axis2DInput::Update()
{
	xChanged = false;
	yChanged = false;
	axisChanged = false;

	// parse indiv - x
	if (xSet)
	{
		if (lastX == 0)
		{
			// dead, check if alive 
			if (abs(rawX) > deadzone)
			{
				xChanged = true;
				lastX = rawX;
			}
			// else ignore 
		}
		else
		{
			// active, check if died
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
	}

	// parase indiv - y
	if (ySet)
	{
		if (lastY == 0)
		{
			// dead, check if alive 
			if (abs(rawY) > deadzone)
			{
				yChanged = true;
				lastY = rawY;
			}
			// else ignore 
		}
		else
		{
			// active, check if died
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
	}

	// parse axis 
	// note: possible for x and y not to change but for axis to change. (x=0.9, y=0.9, axis>1)
	if (xSet || ySet)
	{
		auto length = sqrt(rawX*rawX + rawY*rawY);
		if (lastAxis == glm::vec2())
		{
			// dead, check if alive
			if (length > deadzone)
			{
				axisChanged = true;
				lastAxis = glm::vec2(rawX, rawY);
			}
		}
		else
		{
			// active, check if died
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
	
	// cleanup
	xSet = false;
	ySet = false;
}
