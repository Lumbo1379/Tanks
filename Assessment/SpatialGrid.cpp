#include "SpatialGrid.h"
#include <algorithm>
#include <cmath>

#define PI 3.14159265

void SpatialGrid::tryAddObjectToSegment(int row, int column, GameObject* obj)
{
	if (row < 0 || row >= rows || column < 0 || column >= columns) return;
	
	if (find(spatialGrid[row][column].begin(), spatialGrid[row][column].end(), obj) == spatialGrid[row][column].end() || spatialGrid[row][column].empty()) // If not found
		spatialGrid[row][column].emplace_back(obj);
}

void SpatialGrid::removeObjectFromSegment(int row, int column, GameObject* obj)
{
	if (row < 0 || row >= rows || column < 0 || column >= columns) return;
	
	spatialGrid[row][column].erase(remove(spatialGrid[row][column].begin(), spatialGrid[row][column].end(), obj), spatialGrid[row][column].end());
}

void SpatialGrid::checkForCollisions()
{
	for (int row = 0; row < rows; ++row)
	{
		for (int column = 0; column < columns; ++column)
		{
			if (spatialGrid[row][column].size() > 1)
				compareObjects(spatialGrid[row][column]);
		}
	}
}

void SpatialGrid::compareObjects(vector<GameObject*> gameObjects)
{
	int objIndexToCompare = 0;

	while (objIndexToCompare < gameObjects.size())
	{
		for (int obj = 0; obj < gameObjects.size(); ++obj)
		{
			if (obj == objIndexToCompare) continue;
			if (!gameObjects[objIndexToCompare]->active || !gameObjects[obj]->active) continue;
			
			compareForCollision(gameObjects[objIndexToCompare], gameObjects[obj]);
		}

		objIndexToCompare++;
	}
}

bool SpatialGrid::compareForCollision(GameObject* obj1, GameObject* obj2)
{
	vector<Vector2D> obj1Points = getRectanglePoints(obj1); // Check if obj1 inside of obj2, M points
	vector<Vector2D> obj2Points = getRectanglePoints(obj2); // A, B, C, D points

	if (obj1->x <= 0 && obj1->y <= 0 || obj2->x <= 0 && obj2->y <= 0)
		auto foo = 1;
	
	for (auto point : obj1Points)
	{
		auto AM = getDirectionVector(obj2Points[0], point);
		auto AB = getDirectionVector(obj2Points[0], obj2Points[1]);
		auto AD = getDirectionVector(obj2Points[0], obj2Points[2]);

		if (0 < getDotProduct(AM, AB) && getDotProduct(AM, AB) < getDotProduct(AB, AB) && 0 < getDotProduct(AM, AD) && getDotProduct(AM, AD) < getDotProduct(AD, AD))
		{
			//SDL_Log("Collision");
			obj1->handleCollision(obj2, point);
			obj2->handleCollision(obj1, point);
			
			return  true;
		}
	}

	return  false;
}

vector<Vector2D> SpatialGrid::getRectanglePoints(GameObject* obj)
{
	vector<Vector2D> points;
	
	int pivotX = obj->x + obj->sprite->pivot.x;
	int pivotY = obj->y + obj->sprite->pivot.y;

	int transX = obj->x - pivotX; // Rotate at origin
	int transY = obj->y - pivotY;

	double rotation = toRadians(obj->sprite->rotation);

	int width = obj->width * obj->scale;
	int height = obj->height * obj->scale;

	points.emplace_back(Vector2D(getX(transX, transY, rotation) + pivotX, getY(transX, transY, rotation) + pivotY)); // Upper left
	points.emplace_back(Vector2D(getX(transX + width, transY, rotation) + pivotX, getY(transX + width, transY, rotation) + pivotY)); // Upper right
	points.emplace_back(Vector2D(getX(transX, transY + height, rotation) + pivotX, getY(transX, transY + height, rotation) + pivotY)); // Lower left
	points.emplace_back(Vector2D(getX(transX + width, transY + height, rotation) + pivotX, getY(transX + width, transY + height, rotation) + pivotY)); // Lower right

	return points;
}

int SpatialGrid::getX(int x, int y, double rotation) const
{
	return x * cos(rotation) - y * sin(rotation);
}

int SpatialGrid::getY(int x, int y, double rotation) const
{
	return x * sin(rotation) + y * cos(rotation);
}

int SpatialGrid::getDotProduct(Vector2D point1, Vector2D point2)
{
	return point1.x * point2.x + point1.y * point2.y;
}

Vector2D SpatialGrid::getDirectionVector(Vector2D point1, Vector2D point2)
{
	return {point2.x - point1.x, point2.y - point1.y};
}

double SpatialGrid::toRadians(double degrees)
{
	return  degrees * PI / 180;
}