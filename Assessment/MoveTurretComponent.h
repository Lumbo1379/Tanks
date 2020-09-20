#pragma once
#include "GameComponentBehaviour.h"
#include  "GameObject.h"

class MoveTurretComponent : public GameComponentBehaviour
{
	public:
		double SPEED = 2;
	
	private:
		void start() override;
		void update(unsigned int ticks, GameObject* container) override;
};
