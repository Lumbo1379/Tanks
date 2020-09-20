#pragma once

#include "GameComponentBehaviour.h"
#include "GameObject.h"

class AnimateTankExplosionComponent : public GameComponentBehaviour
{
	public:
		Vector2D position;
		bool explode;

	private:
		void start() override;
		void update(unsigned int ticks, GameObject* container) override;

		int ticksCount;
		int previousTicksCount;
};
