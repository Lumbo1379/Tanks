#include "AnimateTankExplosionComponent.h"

void AnimateTankExplosionComponent::start()
{
	explode = false;
	ticksCount = 0;
	previousTicksCount = 0;
}

void AnimateTankExplosionComponent::update(unsigned int ticks, GameObject* container)
{
	if (explode)
	{
		container->x = position.x - (128 / 2 * container->sprite->scale);
		container->y = position.y - (128 / 2 * container->sprite->scale);
		container->sprite->visible = true;
		
		ticksCount += (ticks - previousTicksCount);

		if (ticksCount >= (int)(container->sprite->frameSpeeds[0] * container->sprite->numberOfFrames))
		{
			container->sprite->visible = false;
			explode = false;
			ticksCount = 0;
			previousTicksCount = 0;
		}
		else
			container->sprite->update(ticksCount, true, false);
	}

	previousTicksCount = ticks;
}
