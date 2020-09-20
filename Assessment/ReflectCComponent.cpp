#include "ReflectCComponent.h"
#include <cmath>
#include "Mixer.h"

void ReflectCComponent::start()
{
	previousTicks = 0;
	currentReflections = 0;
	reflectionMultiplier = 0;
}

void ReflectCComponent::update(unsigned int ticks, GameObject* container)
{
	ticksPast += ticks - previousTicks;
	previousTicks = ticks;
}

void ReflectCComponent::handleCollision(GameObject* collision, GameObject* container, Vector2D contactPoint)
{

	if (ticksPast > collisionCoolDown)
	{
		currentReflections++;

		if (currentReflections > (BASE_MAX_REFLECTIONS + reflectionMultiplier))
		{
			SDL_Log("Projectile removed from battlefield at x: %f, y: %f", contactPoint.x, contactPoint.y);
			
			container->setActive(false);
			container->setVelocity({ 0, 0 });
			currentReflections = 0;
			return;
		}

		SDL_Log("Projectile reflected at x: %f, y: %f", contactPoint.x, contactPoint.y);
		
		ticksPast = 0;
		
		auto position = Vector2D(contactPoint.x - container->getVelocity()->x, contactPoint.y - container->getVelocity()->y);

		Vector2D normal;

		auto ul = Vector2D(collision->x, collision->y);
		auto ur = Vector2D(collision->x + collision->width, collision->y);
		auto ll = Vector2D(collision->x, collision->y + collision->height);
		auto lr = Vector2D(collision->x + collision->width, collision->y + collision->height);

		double upper = getDistance(position, ul) + getDistance(position, ur);
		double right = getDistance(position, ur) + getDistance(position, lr);
		double lower = getDistance(position, lr) + getDistance(position, ll);
		double left = getDistance(position, ll) + getDistance(position, ul);

		if (upper < right && upper < lower && upper < left)
			normal = { 0, -1 };
		else if (right < lower && right < left)
			normal = { 1, 0 };
		else if (lower < left)
			normal = { 0, 1 };
		else
			normal = { -1 , 0 };
		
		container->setVelocity(*container->getVelocity() - 2 * getDotProduct(*container->getVelocity(), normal) * normal);

		Mixer::playSFX(2);
	}
}

double ReflectCComponent::getDotProduct(Vector2D point1, Vector2D point2)
{
	return point1.x * point2.x + point1.y * point2.y;
}

Vector2D ReflectCComponent::getNormalizedVector(Vector2D vector)
{
	return vector / getMagnitude(vector);
}

double ReflectCComponent::getMagnitude(Vector2D vector)
{	
	return sqrt(vector.x * vector.x + vector.y * vector.y);
}

double ReflectCComponent::getDistance(Vector2D v1, Vector2D v2)
{
	return sqrt((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y));
}
