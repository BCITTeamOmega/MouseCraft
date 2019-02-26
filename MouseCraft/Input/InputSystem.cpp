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

void InputSystem::Update(float dt)
{
	SDL_Event e;
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

			if (player >= MAX_PLAYERS)
				continue;

			// left analog horizontal
			if (e.jaxis.axis == 0)
			{
				playerAxes[player * 2].SetX(value);
			}
			// left analog vertical 
			else if (e.jaxis.axis == 1)
			{
				playerAxes[player * 2].SetY(value);
			}
			// right analog horizontal 
			else if (e.jaxis.axis == 3)
			{
				playerAxes[player * 2 + 1].SetX(value);
			}
			// right analog vertical
			else if (e.jaxis.axis == 4)
			{
				playerAxes[player * 2 + 1].SetY(value);
			}
			else
			{
				continue;
			}
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
		else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
		{
			// handling both states here to minimize LINES
			int player = DEBUG_PLAYER;
			bool isDown = e.type == SDL_KEYDOWN;

			switch (e.key.keysym.sym)
			{
			case SDLK_w:
				debugPlayerAxis.SetY(isDown ? -1 : 0);
				break;
			case SDLK_a:
				debugPlayerAxis.SetX(isDown ? -1 : 0);
				break;
			case SDLK_s:
				debugPlayerAxis.SetY(isDown ? 1 : 0);
				break;
			case SDLK_d:
				debugPlayerAxis.SetX(isDown ? 1 : 0);
				break;
			case SDLK_j:
				EventManager::Notify(EventName::INPUT_BUTTON,
					new TypeParam<ButtonEvent>(ButtonEvent(player, Button::PRIMARY, isDown)));
				break;
			case SDLK_k:
				EventManager::Notify(EventName::INPUT_BUTTON,
					new TypeParam<ButtonEvent>(ButtonEvent(player, Button::SECONDARY, isDown)));
				break;
			case SDLK_l:
				EventManager::Notify(EventName::INPUT_BUTTON,
					new TypeParam<ButtonEvent>(ButtonEvent(player, Button::AUX1, isDown)));
				break;
			case SDLK_SEMICOLON:
				EventManager::Notify(EventName::INPUT_BUTTON,
					new TypeParam<ButtonEvent>(ButtonEvent(player, Button::AUX2, isDown)));
				break;
			}
		}
	} // end while 


	// notify joystick movement 
	for (int player = 0; player < MAX_PLAYERS; ++player)
	{
		Axis2DInput& leftStick = playerAxes[player * 2];
		Axis2DInput& rightStick = playerAxes[player * 2 + 1];

		// update 
		leftStick.Update();
		rightStick.Update();

		// notify 
		NotifyAxis(leftStick, player);
		NotifyAxis(rightStick, player);
	}

	// notify debug keyboard player 
	debugPlayerAxis.Update();
	NotifyAxis(debugPlayerAxis, DEBUG_PLAYER);
}

void InputSystem::NotifyAxis(Axis2DInput& axis, int player)
{
	if (axis.HasXChanged())
	{
		EventManager::Notify(EventName::INPUT_AXIS,
			new TypeParam<AxisEvent>(AxisEvent(player, Axis::LEFT_HOR, axis.GetX())));
	}
	if (axis.HasYChanged())
	{
		EventManager::Notify(EventName::INPUT_AXIS,
			new TypeParam<AxisEvent>(AxisEvent(player, Axis::LEFT_VER, axis.GetY())));
	}
	if (axis.HasAxisChanged())
	{
		EventManager::Notify(EventName::INPUT_AXIS_2D,
			new TypeParam<Axis2DEvent>(Axis2DEvent(player, Axis::LEFT, axis.GetAxis())));
	}
}
