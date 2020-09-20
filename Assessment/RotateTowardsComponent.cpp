#include "RotateTowardsComponent.h"
#include <cmath>

#define PI 3.14159265

void RotateTowardsComponent::setTarget(Vector2D target, Vector2D currentPosition, double currentRotation)
{
	auto direction = getNormalizedVector(target - currentPosition);
	//auto radians = acos(getDotProduct(currentPosition, direction) / (getMagnitude(currentPosition) * getMagnitude(target)));
	auto radians = atan2(direction.y, direction.x);
	
	targetRotation = toDegrees(radians) + 90;

	targetRotation = targetRotation - currentRotation;

	if (targetRotation > 180)
		targetRotation = -(360 - targetRotation);
	else if (targetRotation < -180)
		targetRotation = 360 + targetRotation;
}

void RotateTowardsComponent::start()
{
	look = false;
}

void RotateTowardsComponent::update(unsigned int ticks, GameObject* container)
{
	if (look)
	{
		if (abs(targetRotation) > TOLERANCE)
		{
			if (targetRotation < 0)
			{
				targetRotation += SPEED;
				container->sprite->rotate(-SPEED);
			}
			else
			{
				targetRotation -= SPEED;
				container->sprite->rotate(SPEED);
			}
		}
		else
		{
			if (moveTowardsComponent != nullptr)
			{
				moveTowardsComponent->proceed = true;
				look = false;
			}
		}
	}
}

double RotateTowardsComponent::toDegrees(double radians)
{
	return  radians * (180 / PI);
}

double RotateTowardsComponent::getMagnitude(Vector2D vector)
{
	return sqrt(vector.x * vector.x + vector.y * vector.y);
}

double RotateTowardsComponent::getDotProduct(Vector2D point1, Vector2D point2)
{
	return point1.x * point2.x + point1.y * point2.y;
}

Vector2D RotateTowardsComponent::getNormalizedVector(Vector2D vector)
{
	return vector / getMagnitude(vector);
}