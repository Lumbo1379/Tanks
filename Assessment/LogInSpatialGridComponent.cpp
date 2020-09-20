#include "LogInSpatialGridComponent.h"
#include <cmath>
#include <algorithm>

#define PI 3.14159265

LogInSpatialGridComponent::LogInSpatialGridComponent() {}

LogInSpatialGridComponent::LogInSpatialGridComponent(int segmentWidth, int segmentHeight)
{
	this->segmentWidth = segmentWidth;
	this->segmentHeight = segmentHeight;
}

void LogInSpatialGridComponent::start() {}

void LogInSpatialGridComponent::update(unsigned int ticks, GameObject* container)
{
	tempSegmentsHeld.clear();
	
	int pivotX = container->x + container->sprite->pivot.x;
	int pivotY = container->y + container->sprite->pivot.y;
	
	int transX = container->x - pivotX; // Rotate at origin
	int transY = container->y - pivotY;

	double rotation = toRadians(container->sprite->rotation);

	int width = container->width * container->scale;
	int height = container->height * container->scale;

	addSegment(getX(transX, transY, rotation) + pivotX, getY(transX, transY, rotation) + pivotY, container); // Upper left
	addSegment(getX(transX + width, transY, rotation) + pivotX, getY(transX + width, transY, rotation) + pivotY, container); // Upper right
	addSegment(getX(transX, transY + height, rotation) + pivotX, getY(transX, transY + height, rotation) + pivotY, container); // Lower left
	addSegment(getX(transX + width, transY + height, rotation) + pivotX, getY(transX + width, transY + height, rotation) + pivotY, container); // Lower right

	releaseSegmentsLeft(container);
}

void LogInSpatialGridComponent::addSegment(int x, int y, GameObject* container)
{
	int column = x / segmentWidth;
	int row = y / segmentHeight;

	spatialGrid->tryAddObjectToSegment(row, column, container);
	
	tempSegmentsHeld.emplace_back(Vector2D(row, column));
}

int LogInSpatialGridComponent::getX(int x, int y, double rotation) const
{
	return x * cos(rotation) - y * sin(rotation);
}

int LogInSpatialGridComponent::getY(int x, int y, double rotation) const
{	
	return x * sin(rotation) + y * cos(rotation);
}

double LogInSpatialGridComponent::toRadians(double degrees)
{
	return  degrees * PI / 180;
}

void LogInSpatialGridComponent::releaseSegmentsLeft(GameObject* container)
{
	vector<Vector2D> segmentsToRelease;
	
	if (currentSegmentsHeld.empty())
		currentSegmentsHeld = tempSegmentsHeld;
	else
	{
		for (auto segment : currentSegmentsHeld)
		{
			for (auto tempSegment : tempSegmentsHeld)
			{
				if (find_if(tempSegmentsHeld.begin(), tempSegmentsHeld.end(), [&](const Vector2D v) -> bool { return v == segment; }) == tempSegmentsHeld.end())
					segmentsToRelease.emplace_back(segment);
			}
		}

		for (auto segmentToRelease : segmentsToRelease)		
			spatialGrid->removeObjectFromSegment((int)segmentToRelease.x, (int)segmentToRelease.y, container);

		currentSegmentsHeld = tempSegmentsHeld;
	}
}
