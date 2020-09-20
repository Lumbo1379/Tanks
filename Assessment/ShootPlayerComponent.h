#pragma once
#include "GameComponentBehaviour.h"
#include "GameObject.h"
#include "RotateTowardsComponent.h"
#include "AnimateShotComponent.h"
#include "Raycast.h"

class ShootPlayerComponent : public GameComponentBehaviour
{
	public:
		RotateTowardsComponent* rotateTowardsComponent;
		GameObject* player;
		AnimateShotComponent* animateShotComponent;
		GameObject* projectiles[3];
		Raycast* raycast;
		const int MAX_PROJECTILES = 3;
		int projectileCount;
		int previousTickCount;
	
	private:
		void start() override;
		void update(unsigned int ticks, GameObject* container) override;
		double toRadians(double degrees);
		double getDistance(Vector2D v1, Vector2D v2);

		const int SPEED = 5;
		const int MIN_DISTANCE_TO_SHOOT = 1000;
		const int delayBetweenShots = 500;
};
