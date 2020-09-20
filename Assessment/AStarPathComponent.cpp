#include "AStarPathComponent.h"
#include <cmath>

#define PI 3.14159265

void AStarPathComponent::start()
{
	built = false;
	pathingError = false;
}

void AStarPathComponent::update(unsigned int ticks, GameObject* container)
{
	if (built)
	{
		if (moveTowardsComponent->atTarget)
		{
			if (!targetTiles.empty())
			{
				TargetTile tile = targetTiles.back();
				targetTiles.pop_back();

				moveTowardsComponent->setTarget(Vector2D(tile.y * 80 + 40, tile.x * 80 + 40));
				moveTowardsComponent->atTarget = false;
				moveTowardsComponent->recalculated = false;

				int pivotX = container->x + container->sprite->pivot.x;
				int pivotY = container->y + container->sprite->pivot.y;

				int transX = container->x - pivotX; // Rotate at origin
				int transY = container->y - pivotY;

				double rotation = container->sprite->rotation * (180 / PI);;

				int width = container->width * container->scale;

				int xTop = (transX + width / 2) * cos(rotation) - transY * sin(rotation) + pivotX;
				int yTop = (transX + width / 2) * sin(rotation) + transY * cos(rotation) + pivotY;
				
				rotateTowardsComponent->setTarget(Vector2D(tile.y * 80 + 40, tile.x * 80 + 40), Vector2D(xTop, yTop), container->sprite->rotation);
				rotateTowardsComponent->look = true;
			}
			else
				build(container->y / 80, container->x / 80);
		}
	}
	else if (!pathingError)
		build(container->y / 80, container->x / 80);
}

void AStarPathComponent::build(int x, int y)
{
	int startTicks = SDL_GetTicks();
	
	targetTiles.clear();
	moveTowardsComponent->atTarget = true;
	int neighbourReach = 1;

	while (true)
	{
		if (pathGridOriginal[x][y]->isClosed)
		{
			if (x + neighbourReach < 12)
			{
				if (!pathGridOriginal[x + neighbourReach][y]->isClosed)
				{
					x += neighbourReach;
					break;
				}
			}
			else if (x - neighbourReach >= 0)
			{
				if (!pathGridOriginal[x - neighbourReach][y]->isClosed)
				{
					x -= neighbourReach;
					break;
				}
			}
			else if (y + neighbourReach < 16)
			{
				if (!pathGridOriginal[x][y + neighbourReach]->isClosed)
				{
					y += neighbourReach;
					break;
				}
			}
			else if (y - neighbourReach >= 0)
			{
				if (!pathGridOriginal[x][y - neighbourReach]->isClosed)
				{
					y -= neighbourReach;
					break;
				}
			}
		}
		else
			break;

		neighbourReach++;
	}

	for (int row = 0; row < 12; ++row)
	{
		for (int column = 0; column < 16; ++column)
		{
			pathGridWorking[row][column] = *pathGridOriginal[row][column];
			linkX[row][column] = -1;
			linkY[row][column] = -1;
		}
	}

	pathGridWorking[x][y].cost = 0;

	int nextX = -1;
	int nextY = -1;

	int targetRow = player->y / 80;
	int targetColumn = player->x / 80;

	if (pathGridOriginal[targetRow][targetColumn]->isClosed)
		return;

	while (true)
	{
		float lowestCost = 10000.0f;

		for (int row = 0; row < 12; ++row)
		{
			for (int column = 0; column < 16; ++column)
			{
				float heuristic = fabs(targetRow - row) + fabs(targetColumn - column);

				if (pathGridWorking[row][column].cost + heuristic < lowestCost && !pathGridWorking[row][column].isClosed)
				{
					nextX = row;
					nextY = column;

					lowestCost = pathGridWorking[nextX][nextY].cost + heuristic;
				}
			}
		}

		pathGridWorking[nextX][nextY].isClosed = true;

		if (pathGridWorking[targetRow][targetColumn].isClosed)
			break;

		float parentCost = pathGridWorking[nextX][nextY].cost;
		calculateCost(nextX - 1, nextY, parentCost + 1.0f, nextX, nextY); // Left
		calculateCost(nextX + 1, nextY, parentCost + 1.0f, nextX, nextY); // Right
		calculateCost(nextX, nextY - 1, parentCost + 1.0f, nextX, nextY); // Up
		calculateCost(nextX, nextY + 1, parentCost + 1.0f, nextX, nextY); // Down
		calculateCost(nextX - 1, nextY - 1, parentCost + 1.4f, nextX, nextY); // Left-up
		calculateCost(nextX + 1, nextY - 1, parentCost + 1.4f, nextX, nextY); // Right-up
		calculateCost(nextX - 1, nextY + 1, parentCost + 1.4f, nextX, nextY); // Left-down
		calculateCost(nextX + 1, nextY + 1, parentCost + 1.4f, nextX, nextY); // Right-down
	}

	bool done = false;
	int nextClosedX = targetRow;
	int nextClosedY = targetColumn;

	int i = 0;

	while (!done)
	{
		if (nextClosedX == -1 || nextClosedY == -1)
		{
			pathingError = true;
			return;
		}
		
		targetTiles.emplace_back(TargetTile(nextClosedX, nextClosedY));
		int tmpX = nextClosedX;
		int tmpY = nextClosedY;
		nextClosedX = linkX[tmpX][tmpY];
		nextClosedY = linkY[tmpX][tmpY];

		if ((nextClosedX == x) && (nextClosedY == y))
			done = true;
	}

	built = true;

	SDL_Log("Path built in %i ticks", SDL_GetTicks() - startTicks);
}

void AStarPathComponent::calculateCost(int x, int y, float newCost, int parentX, int parentY)
{
	if (newCost < pathGridWorking[x][y].cost)
	{
		pathGridWorking[x][y].cost = newCost;
		linkX[x][y] = parentX;
		linkY[x][y] = parentY;
	}
}
