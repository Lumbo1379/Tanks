#pragma once

#include "GameComponentBehaviour.h"
#include  "GameObject.h"

class ReflectCComponent : public GameComponentBehaviour
{
	public:
		void start() override;
		void update(unsigned int ticks, GameObject* container) override;
		void handleCollision(GameObject* collision, GameObject* container, Vector2D contactPoint) override;

		int reflectionMultiplier;

	private:
		double getDotProduct(Vector2D point1, Vector2D point2);
		Vector2D getNormalizedVector(Vector2D vector);
		double getMagnitude(Vector2D vector);
		double getDistance(Vector2D v1, Vector2D v2);

		const int collisionCoolDown = 75;
		const int BASE_MAX_REFLECTIONS = 2;
		int ticksPast = 0;
		int previousTicks;
		int currentReflections;
};