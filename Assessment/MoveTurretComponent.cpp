#include "MoveTurretComponent.h"

void MoveTurretComponent::start() {}

void MoveTurretComponent::update(unsigned int ticks, GameObject* container)
{
	if (container->MOVE_ARROW_LEFT)
		container->sprite->rotate(-SPEED);

	if (container->MOVE_ARROW_RIGHT)
		container->sprite->rotate(SPEED);
}
