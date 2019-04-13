#include "InputSystem.h"
#include "../Core/OmegaEngine.h"

#pragma region Class::Axis2DInput

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
			// active, check if died, else normal input.
			lastAxis = (length < deadzone) ? glm::vec2() : glm::vec2(rawX, rawY);
			axisChanged = true;
		}
	}
	
	// cleanup
	xSet = false;
	ySet = false;
}

#pragma endregion

#pragma region Class::InputSystem

void InputSystem::Update(float dt)
{
	profiler.StartTimer(0);

	// Note: hot pile of trash for handle SDL2 hot pile of keyboard input trash.
	dkUpLast = dkUp;
	dkDownLast = dkDown;
	dkLeftLast = dkLeft;
	dkRightLast = dkRight;

	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			OmegaEngine::Instance().Stop();
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
		else if (e.type == SDL_JOYBUTTONDOWN || e.type == SDL_JOYBUTTONUP)
		{
			int player = e.jbutton.which;
			bool isDown = e.jbutton.state == SDL_PRESSED;
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
            case 6:
                b = Button::OPTION;
                break;
			default:
				continue;
			}

			// notify
			EventManager::Notify(EventName::INPUT_BUTTON,
				new TypeParam<ButtonEvent>(ButtonEvent{ player, b, isDown }));
		}
		else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
		{
			// handling both states here to minimize LINES
			int player = DEBUG_PLAYER;
			bool isDown = e.type == SDL_KEYDOWN;
			float diff;

			switch (e.key.keysym.sym)
			{
			case SDLK_w:
				dkUp = (isDown) ? -1 : 0;
				break;
			case SDLK_a:
				dkLeft = (isDown) ? -1 : 0;
				break;
			case SDLK_s:
				dkDown = (isDown) ? 1 : 0;
				break;
			case SDLK_d:
				dkRight = (isDown) ? 1 : 0;
				break;
			case SDLK_j:
				EventManager::Notify(EventName::INPUT_BUTTON,
					new TypeParam<ButtonEvent>(ButtonEvent{ player, Button::PRIMARY, isDown }));
				break;
			case SDLK_k:
				EventManager::Notify(EventName::INPUT_BUTTON,
					new TypeParam<ButtonEvent>(ButtonEvent{ player, Button::SECONDARY, isDown }));
				break;
			case SDLK_l:
				EventManager::Notify(EventName::INPUT_BUTTON,
					new TypeParam<ButtonEvent>(ButtonEvent{ player, Button::AUX1, isDown }));
				break;
			case SDLK_SEMICOLON:
				EventManager::Notify(EventName::INPUT_BUTTON,
					new TypeParam<ButtonEvent>(ButtonEvent{ player, Button::AUX2, isDown }));
				break;
            case SDLK_RETURN:
                EventManager::Notify(EventName::INPUT_BUTTON,
					new TypeParam<ButtonEvent>(ButtonEvent{ player, Button::OPTION, isDown }));
                break;
            }
		}
		else if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
		{
			bool isDown = e.button.state == SDL_PRESSED;
			bool isRight;
			glm::ivec2 pos;

			if (e.button.button == SDL_BUTTON_LEFT)
			{
				isRight = false;
			}
			else if (e.button.button == SDL_BUTTON_RIGHT)
			{
				isRight = true;
			}
			else
			{
				// some other button - ignore. 
				continue;
			}

			SDL_GetMouseState(&pos.x, &pos.y);

			// notify 
			EventManager::Notify(EventName::INPUT_MOUSE_CLICK,
				new TypeParam<MouseButtonEvent>(MouseButtonEvent{ pos, isRight, isDown }));
		}
		else if (e.type == SDL_MOUSEMOTION)
		{
			EventManager::Notify(EventName::INPUT_MOUSE_MOVE,
				new TypeParam<glm::ivec2>(glm::ivec2(e.motion.x, e.motion.y)));
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
		NotifyAxis(leftStick, Axis::LEFT, player);
		NotifyAxis(rightStick, Axis::RIGHT, player);
	}

	// notify debug keyboard player 
	// note: we update every frame because SDL2 is stupid and key_down/up event/pressed/release 
	// is already called every frame on hold (instead on change).
	bool dkUpdated = (dkLeft != dkLeftLast) || (dkRight != dkRightLast) || (dkUp != dkUpLast) || (dkDown != dkDownLast);
	if (dkUpdated)
	{
		debugPlayerAxis.SetX(dkLeft + dkRight);
		debugPlayerAxis.SetY(dkUp + dkDown);
	}
	debugPlayerAxis.Update();
	NotifyAxis(debugPlayerAxis, Axis::LEFT, DEBUG_PLAYER);

	profiler.StopTimer(0);
	profiler.FrameFinish();
}

void InputSystem::NotifyAxis(Axis2DInput& axis, Axis which, int player)
{
	if (axis.HasAxisChanged())
	{
		EventManager::Notify(EventName::INPUT_AXIS_2D,
			new TypeParam<Axis2DEvent>(Axis2DEvent{ player, which, axis.GetAxis() }));
	}
	if (axis.HasXChanged())
	{
		EventManager::Notify(EventName::INPUT_AXIS,
			new TypeParam<AxisEvent>(AxisEvent{ player, static_cast<Axis>(which + 1), axis.GetX() }));
	}
	if (axis.HasYChanged())
	{
		EventManager::Notify(EventName::INPUT_AXIS,
			new TypeParam<AxisEvent>(AxisEvent{ player, static_cast<Axis>(which + 2), axis.GetY() }));
	}
}

#pragma endregion