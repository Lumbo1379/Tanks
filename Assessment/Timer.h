#pragma once

#include <iostream>
#include <SDL.h>

class Timer
{
private:
	int startTicks; // SDL time when the timer started

public:
	Timer();
	void resetTicksTimer();
	int getTicks();
};


