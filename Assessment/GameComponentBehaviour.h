#pragma once

#include "Vector2D.h"

class GameObject;

class GameComponentBehaviour
{
	public:
		virtual ~GameComponentBehaviour() = default;
		virtual void start() = 0;
		virtual void update(unsigned int ticks, GameObject* container) = 0;
		virtual void handleCollision(GameObject* collision, GameObject* container, Vector2D contactPoint);
};
