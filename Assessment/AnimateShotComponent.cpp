#include "AnimateShotComponent.h"

void AnimateShotComponent::start()
{
	fired = false;
	ticksCount = 0;
	previousTicksCount = 0;
}

void AnimateShotComponent::update(unsigned int ticks, GameObject* container)
{
	if (fired)
	{
		ticksCount += (ticks - previousTicksCount);

		if (ticksCount >= (int)(projectileEffect->sprite->frameSpeeds[0] * projectileEffect->sprite->numberOfFrames))
		{
			projectileEffect->sprite->visible = false;
			fired = false;
			ticksCount = 0;
			previousTicksCount = 0;
		}
		else
			projectileEffect->sprite->update(ticksCount, true, false);
	}

	previousTicksCount = ticks;
}
