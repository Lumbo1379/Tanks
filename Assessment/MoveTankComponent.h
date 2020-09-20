#pragma once

#include "GameComponentBehaviour.h"
#include  "GameObject.h"

class MoveTankComponent : public GameComponentBehaviour
{
	public:
		MoveTankComponent();
		MoveTankComponent(int minX, int maxX, int minY, int maxY);

		GameObject* turret;
		int speedMultiplier;
		int* score;
		double SPEED_BASE = 3;

	private:
		void start() override;
		void update(unsigned int ticks, GameObject* container) override;
		double toRadians(double degrees);
	
		double truncExtraX, truncExtraY;
		int minX, maxX, minY, maxY;
};