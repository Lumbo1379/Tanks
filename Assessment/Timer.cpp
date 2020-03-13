#include "Timer.h"

Timer::Timer()
{
	startTicks = 0;
}

void Timer::resetTicksTimer()
{
	startTicks = SDL_GetTicks(); // Number of millisconds since start of SDL program
}

int Timer::getTicks()
{
	return (SDL_GetTicks() - startTicks); // Return the current time minus the start time
}
