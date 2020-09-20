#pragma once

#include "GameComponentBehaviour.h"
#include "SpatialGrid.h"
#include  "GameObject.h"

class LogInSpatialGridComponent : public GameComponentBehaviour
{
	public:
		LogInSpatialGridComponent();
		LogInSpatialGridComponent(int segmentWidth, int segmentHeight);
		SpatialGrid* spatialGrid;

	private:
		void start() override;
		void update(unsigned int ticks, GameObject* container) override;
		void addSegment(int x, int y, GameObject* container);
		int getX(int x, int y, double rotation) const;
		int getY(int x, int y, double rotation) const;
		double toRadians(double degrees);
		void releaseSegmentsLeft(GameObject* container);

		int segmentWidth, segmentHeight;
		vector<Vector2D> currentSegmentsHeld;
		vector<Vector2D> tempSegmentsHeld;
};