#include "ShootProjectileComponent.h"
#include "Mixer.h"
#include <cmath>

#define PI 3.14159265

void ShootProjectileComponent::start()
{
	projectileCount = 0;
	previousTickCount = 0;
	speedMultiplier = 0;
}

void ShootProjectileComponent::update(unsigned int ticks, GameObject* container)
{
	if (container->SPACE && !projectiles[projectileCount]->active)
	{
		if (ticks - previousTickCount < delayBetweenShots)
			return;
		
		previousTickCount = ticks;

		double x = cos(toRadians(container->sprite->rotation - 90)) * (SPEED_BASE + speedMultiplier);
		double y = sin(toRadians(container->sprite->rotation - 90)) * (SPEED_BASE + speedMultiplier);

		auto projectile = projectiles[projectileCount];
		projectile->x = container->x + 11;
		projectile->y = container->y - 16;
		projectile->sprite->rotation = container->sprite->rotation;
		projectile->setVelocity({ x, y });
		projectile->setActive(true);

		if (projectileCount == MAX_PROJECTILES_BASE - 1)
			projectileCount = 0;
		else
			projectileCount++;

		animateShotComponent->fired = true;
		animateShotComponent->projectileEffect->sprite->visible = true;
		Mixer::playSFX(1);
	}
}

double ShootProjectileComponent::toRadians(double degrees)
{
	return  degrees * PI / 180;
}
