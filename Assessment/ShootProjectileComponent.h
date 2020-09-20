#pragma once

#include "GameComponentBehaviour.h"
#include  "GameObject.h"
#include "Vector2D.h"
#include "AnimateShotComponent.h"

class ShootProjectileComponent : public GameComponentBehaviour
{
	public:
		AnimateShotComponent* animateShotComponent;
		GameObject* projectiles[3];
		const int MAX_PROJECTILES_BASE = 3;
		int projectileCount;
		int previousTickCount;
		int speedMultiplier;

	private:
		void start() override;
		void update(unsigned int ticks, GameObject* container) override;
		double toRadians(double degrees);

		const int SPEED_BASE = 5;
		const int delayBetweenShots = 500;
};