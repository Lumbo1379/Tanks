#include "Timer.h"

Timer::Timer()
{
	startTicks = 0;
}

void Timer::resetFrameTicksTimer()
{
	frameTicks = SDL_GetTicks(); // Number of millisconds since start of SDL program
}

int Timer::getFrameTicks()
{
	return (SDL_GetTicks() - frameTicks); // Return the current time minus the start time
}

int Timer::getAnimationTicks()
{
	return (SDL_GetTicks() - startTicks); // Return the current time minus the start time
}
