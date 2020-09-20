#include "MoveTowardsComponent.h"
#include <cmath>
#include "RotateTowardsComponent.h"

#define PI 3.14159265

void MoveTowardsComponent::setTarget(Vector2D target)
{
	this->target = target;
}

Vector2D MoveTowardsComponent::getTarget() const
{
	return target;
}

void MoveTowardsComponent::start()
{
	truncExtraX = 0;
	truncExtraY = 0;
	proceed = false;
	xReached = false;
	yReached = false;
	recalculated = false;
}

void MoveTowardsComponent::update(unsigned int ticks, GameObject* container)
{
	int pivotX = container->x + container->sprite->pivot.x;
	int pivotY = container->y + container->sprite->pivot.y;

	int transX = container->x - pivotX; // Rotate at origin
	int transY = container->y - pivotY;

	double rotation = toRadians(container->sprite->rotation);

	int width = container->width * container->scale;

	int xTop = (transX + width / 2) * cos(rotation) - transY * sin(rotation) + pivotX;
	int yTop = (transX + width / 2) * sin(rotation) + transY * cos(rotation) + pivotY;

	
	if ((!xReached || !yReached) && proceed)
	{
		if (abs(xTop - target.x) < TOLERANCE)
		{
			//SDL_Log("X Reached");
			xReached = true;
		}

		if (abs(yTop - target.y) < TOLERANCE)
		{
			//SDL_Log("Y Reached");
			yReached = true;
		}

		if ((xReached || yReached) && !recalculated)// || (abs(xTop - target.x) > 80 && abs(yTop - target.y) > 80))
		{
			//SDL_Log("Recalculate");
			container->getComponent<RotateTowardsComponent>().setTarget(target, Vector2D(xTop, yTop), container->sprite->rotation);
			container->getComponent<RotateTowardsComponent>().look = true;
			proceed = false;
			recalculated = true;
			container->setVelocity({ 0, 0 });
			turret->setVelocity({ 0, 0 });
			
			return;
		}
		
		atTarget = false;
		
		double x = cos(toRadians(container->sprite->rotation - 90)) * SPEED;
		double y = sin(toRadians(container->sprite->rotation - 90)) * SPEED;

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

		container->setVelocity({ x, y });
		turret->setVelocity({ x, y });
	}
	else if (proceed)
	{
		xReached = false;
		yReached = false;
		atTarget = true;
		proceed = false;

		container->setVelocity({ 0, 0 });
		turret->setVelocity({ 0, 0 });
	}
}

double MoveTowardsComponent::toRadians(double degrees)
{
	return  degrees * PI / 180;
}
