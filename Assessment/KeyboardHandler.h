#pragma once

#include <map>
#include <string>
#include "SDL.h"

using namespace std;

class KeyboardHandler
{
	public:
		void handleKeyboardEvent(SDL_Event event);
		bool isPressed(SDL_Keycode keyCode);
		bool isReleased(SDL_Keycode keyCode);

	private:
		map<int, int> keyStates;
	
};
