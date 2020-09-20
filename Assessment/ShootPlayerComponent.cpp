#include "ShootPlayerComponent.h"
#include <cmath>
#include "Mixer.h"

#define PI 3.14159265

void ShootPlayerComponent::start()
{
	projectileCount = 0;
	previousTickCount = 0;
}

void ShootPlayerComponent::update(unsigned int ticks, GameObject* container)
{
	rotateTowardsComponent->setTarget(Vector2D(player->x, player->y), Vector2D(container->x, container->y), container->sprite->rotation);

	if (getDistance(Vector2D(container->x, container->y), Vector2D(player->x, player->y)) <= MIN_DISTANCE_TO_SHOOT && !projectiles[projectileCount]->active)
	{
		if (ticks - previousTickCount < delayBetweenShots)
			return;

		previousTickCount = ticks;

		double x = cos(toRadians(container->sprite->rotation - 90)) * SPEED;
		double y = sin(toRadians(container->sprite->rotation - 90)) * SPEED;

		if (raycast->cast(Vector2D(player->x, player->y), Vector2D(container->x, container->y), Vector2D(x, y)))
			return;

		auto projectile = projectiles[projectileCount];
		projectile->x = container->x + 11;
		projectile->y = container->y - 16;
		projectile->sprite->rotation = container->sprite->rotation;
		projectile->setVelocity({ x, y });
		projectile->setActive(true);

		if (projectileCount == MAX_PROJECTILES - 1)
			projectileCount = 0;
		else
			projectileCount++;

		animateShotComponent->fired = true;
		animateShotComponent->projectileEffect->sprite->visible = true;
		Mixer::playSFX(1);
	}
}

double ShootPlayerComponent::toRadians(double degrees)
{
	return  degrees * PI / 180;
}

double ShootPlayerComponent::getDistance(Vector2D v1, Vector2D v2)
{
	return sqrt((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y));
}
