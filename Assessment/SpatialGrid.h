#pragma once

#include "GameObject.h";

class SpatialGrid {
	public :
		void tryAddObjectToSegment(int row, int column, GameObject* obj);
		void removeObjectFromSegment(int row, int column, GameObject* obj);
		void checkForCollisions();

	private:
		void compareObjects(vector<GameObject*> gameObjects);
		bool compareForCollision(GameObject* obj1, GameObject* obj2);
		vector<Vector2D> getRectanglePoints(GameObject* obj);
		int getX(int x, int y, double rotation) const;
		int getY(int x, int y, double rotation) const;
		int getDotProduct(Vector2D point1, Vector2D point2);
		Vector2D getDirectionVector(Vector2D point1, Vector2D point2);
		double toRadians(double degrees);
	
		vector<GameObject*> spatialGrid[7][9]; // [rows][columns]
		int rows = 7;
		int columns = 9;
};