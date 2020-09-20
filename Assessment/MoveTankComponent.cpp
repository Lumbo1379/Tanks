#include "MoveTankComponent.h"
#include <cmath>

#define PI 3.14159265

MoveTankComponent::MoveTankComponent() {}

MoveTankComponent::MoveTankComponent(int minX, int maxX, int minY, int maxY)
{
	this->minX = minX;
	this->maxX = maxX;
	this->minY = minY;
	this->maxY = maxY;
}

void MoveTankComponent::start()
{
	truncExtraX = 0;
	truncExtraY = 0;
	speedMultiplier = 0;
}

void MoveTankComponent::update(unsigned int ticks, GameObject* container)
{
	if (!container->MOVE_UP && !container->MOVE_DOWN)
	{
		container->setVelocity({ 0, 0 });
		turret->setVelocity({ 0, 0 });
	}
	else if (container->MOVE_UP || container->MOVE_DOWN)
	{
		if (*score > 0)
			*score = *score - 1;
		
		double rotationOffset;

		if (container->MOVE_UP)
			rotationOffset = -90;
		else if (container->MOVE_DOWN)
			rotationOffset = -270;


		double x = cos(toRadians(container->sprite->rotation + rotationOffset)) * (SPEED_BASE + speedMultiplier);
		double y = sin(toRadians(container->sprite->rotation + rotationOffset)) * (SPEED_BASE + speedMultiplier);

		truncExtraX += modf(x, &x);
		truncExtraY += modf(y, &y);

		double temp;
		
		if (truncExtraX >= 1 || truncExtraX <= -1)
		{
			truncExtraX = modf(truncExtraX, &temp);
			x += temp;
		}

		if (truncExtraY >= 1 || truncExtraY <= -1)
		{
			truncExtraY = modf(truncExtraY, &temp);
			y += temp;
		}

		if (container->x + x  < minX || container->x + x > maxX)
			x = 0;

		if (container->y + y  < minY || container->y + y > maxY)
			y = 0;

		container->setVelocity({ x, y });
		turret->setVelocity({ x, y });
	}
}

double MoveTankComponent::toRadians(double degrees)
{
	return  degrees * PI / 180;
}
