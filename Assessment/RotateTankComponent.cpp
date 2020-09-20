#include "RotateTankComponent.h"

void RotateTankComponent::start() {}

void RotateTankComponent::update(unsigned int ticks, GameObject* container)
{
	if (container->MOVE_LEFT)
		container->sprite->rotate(-SPEED);

	if (container->MOVE_RIGHT)
		container->sprite->rotate(SPEED);
}

