#pragma once

#include <iostream>
#include <SDL.h>

class Timer
{
private:
	int startTicks; // SDL time when the timer started
	int frameTicks; // Time since last frame

public:
	Timer();
	void resetFrameTicksTimer();
	int getFrameTicks();
	int getAnimationTicks();
};


