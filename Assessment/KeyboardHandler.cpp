#include "KeyboardHandler.h"

void KeyboardHandler::handleKeyboardEvent(SDL_Event event)
{
	if (!event.key.repeat)
		keyStates[event.key.keysym.sym] = event.type;
}

bool KeyboardHandler::isPressed(SDL_Keycode keyCode)
{
	return keyStates[keyCode] == SDL_KEYDOWN;
}

bool KeyboardHandler::isReleased(SDL_Keycode keyCode)
{
	return keyStates[keyCode] == SDL_KEYUP;
}
