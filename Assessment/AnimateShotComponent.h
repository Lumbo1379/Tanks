#pragma once

#include "GameComponentBehaviour.h"
#include "GameObject.h"

class AnimateShotComponent : public GameComponentBehaviour
{
	public:
		GameObject* projectileEffect;
		bool fired;
	
	private:
		void start() override;
		void update(unsigned int ticks, GameObject* container) override;

		int ticksCount;
		int previousTicksCount;
};
