#include "Raycast.h"
#include <cmath>

bool Raycast::cast(Vector2D target, Vector2D position, Vector2D direction)
{
	bool xReached = false;
	bool yReached = false;
	
	int x = position.x;
	int y = position.y;
	
	int row = y / 80;
	int column = x / 80;

	while ((abs(x - target.x) > 80 && !xReached) || (abs(y - target.y) > 80 && !yReached))
	{
		if (abs(x - target.x) < 80)
			xReached = true;

		if (abs(y - target.y) < 80)
			yReached = true;
		
		x += direction.x;
		y += direction.y;

		int tempRow = y / 80;
		int tempColumn = x / 80;

		if (tempRow != row || tempColumn != column)
		{
			row = tempRow;
			column = tempColumn;

			if (pathGrid[row][column]->isClosed && !pathGrid[row][column]->toClose)
				return true;
		}
	}
	
	return false;
}
