#pragma once

#include "Vector2D.h"
#include "PathTile.h"

class Raycast
{
	public:
		bool cast(Vector2D target, Vector2D position, Vector2D direction);

		PathTile* pathGrid[12][16];
};
