#pragma once

#include "GameComponentBehaviour.h"
#include "GameObject.h"
#include "MoveTowardsComponent.h"

class RotateTowardsComponent : public GameComponentBehaviour
{
	public:
		void setTarget(Vector2D target, Vector2D currentPosition, double currentRotation);

		bool look;
		MoveTowardsComponent* moveTowardsComponent;

	private:
		void start() override;
		void update(unsigned int ticks, GameObject* container) override;
		double toDegrees(double radians);
		double getMagnitude(Vector2D vector);
		double getDotProduct(Vector2D point1, Vector2D point2);
		Vector2D getNormalizedVector(Vector2D vector);

		const int SPEED = 2;
		const int TOLERANCE = 3;
		int targetRotation;
};
