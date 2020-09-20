#pragma once

#include "GameComponentBehaviour.h"
#include "GameObject.h"
#include "AnimateTankExplosionComponent.h"

class RevertMovementCComponent : public GameComponentBehaviour
{
	public:
		AnimateTankExplosionComponent* animateTankExplosionComponent;
		int shotsCanTake;
	
	private:
		void start() override;
		void update(unsigned int ticks, GameObject* container) override;
		void handleCollision(GameObject* collision, GameObject* container, Vector2D contactPoint) override;

		int currentShotsTaken;
};
