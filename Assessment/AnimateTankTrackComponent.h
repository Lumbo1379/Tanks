#pragma once
#include "GameComponentBehaviour.h"
#include  "GameObject.h"

class AnimateTankTrackComponent : public GameComponentBehaviour
{
	private:
		void start() override;
		void update(unsigned int ticks, GameObject* container) override;
};
