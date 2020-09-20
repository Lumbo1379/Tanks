#include "RevertMovementCComponent.h"
#include "Mixer.h"

void RevertMovementCComponent::start()
{
	shotsCanTake = 1;
	currentShotsTaken = 0;
}

void RevertMovementCComponent::update(unsigned int ticks, GameObject* container) {}

void RevertMovementCComponent::handleCollision(GameObject* collision, GameObject* container, Vector2D contactPoint)
{
	SDL_Log("Collision at x: %f, y: %f between Tag: %i and Tag: %i", contactPoint.x, contactPoint.y, collision->tag, container->tag);
	
	if (collision->tag == Tag::Projectile && container->tag == Tag::Tank)
	{
		collision->setActive(false);

		currentShotsTaken++;

		if (currentShotsTaken >= shotsCanTake)
		{
			animateTankExplosionComponent->position.x = container->x;
			animateTankExplosionComponent->position.y = container->y;
			animateTankExplosionComponent->explode = true;

			container->setActive(false);
			Mixer::playSFX(0);
			
			SDL_Log("Tank exploded by projectile at x: %f, y: %f", contactPoint.x, contactPoint.y);
		}
		else
			SDL_Log("Tank hit but not exploded by projectile at x: %f, y: %f", contactPoint.x, contactPoint.y);
	}
	else if (collision->tag == Tag::Tank && container->tag == Tag::Tank)
	{
		animateTankExplosionComponent->position.x = container->x;
		animateTankExplosionComponent->position.y = container->y;
		animateTankExplosionComponent->explode = true;

		container->setActive(false);
		Mixer::playSFX(0);

		SDL_Log("Tank exploded by tank at x: %f, y: %f", contactPoint.x, contactPoint.y);
	}
	else
	{
		SDL_Log("Container last move reverted at x: %f, y: %f", contactPoint.x, contactPoint.y);
		
		container->revertLastMove();
	}
}
