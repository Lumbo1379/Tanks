#pragma once

#include "GameComponentBehaviour.h"
#include "GameObject.h"

class RotateTankComponent : public GameComponentBehaviour
{
	private:
		void start() override;
		void update(unsigned int ticks, GameObject* container) override;

		const int SPEED = 2;
};