#pragma once

#include "GameComponentBehaviour.h"
#include "GameObject.h"

class MoveTowardsComponent : public GameComponentBehaviour
{
	public:
		void setTarget(Vector2D target);
		Vector2D getTarget() const;
		
		GameObject* turret;
		bool atTarget;
		bool proceed;
		bool recalculated;

	private:
		void start() override;
		void update(unsigned int ticks, GameObject* container) override;
		double toRadians(double degrees);

		const int SPEED = 3;
		const int TOLERANCE = 30;
		double truncExtraX, truncExtraY;
		Vector2D target;
		Vector2D startPos;
		bool xReached, yReached;
};
