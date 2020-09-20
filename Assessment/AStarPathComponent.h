#pragma once

#include "GameComponentBehaviour.h"
#include "GameObject.h"
#include "PathTile.h"
#include "MoveTowardsComponent.h"
#include "RotateTowardsComponent.h"
#include "TargetTile.h"

class AStarPathComponent : public GameComponentBehaviour
{
	public:
		GameObject* player;
		MoveTowardsComponent* moveTowardsComponent;
		RotateTowardsComponent* rotateTowardsComponent;
		PathTile* pathGridOriginal[12][16];

	private:
		void start() override;
		void update(unsigned int ticks, GameObject* container) override;
		void build(int x, int y);
		void calculateCost(int x, int y, float newCost, int parentX, int parentY);

		vector<TargetTile> targetTiles;
		PathTile pathGridWorking[12][16];
		int linkX[12][16];
		int linkY[12][16];
		bool built, pathingError;
};
