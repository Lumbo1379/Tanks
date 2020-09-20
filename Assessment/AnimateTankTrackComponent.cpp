#include "AnimateTankTrackComponent.h"

void AnimateTankTrackComponent::start() {}

void AnimateTankTrackComponent::update(unsigned int ticks, GameObject* container)
{
	if (container->sprite->numberOfFrames > 1)
		container->sprite->update(ticks, container->MOVE_LEFT || container->MOVE_RIGHT || container->MOVE_UP || container->MOVE_DOWN, container->MOVE_DOWN);
}
