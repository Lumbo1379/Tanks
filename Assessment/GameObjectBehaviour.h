#pragma once
#include <vector>
#include <SDL.h>
#include "Vector2D.h"

enum class Tag;
class GameComponentBehaviour;

using namespace  std;

class GameObjectBehaviour
{
	public:
		GameObjectBehaviour();

	protected:
		void updateChildren(unsigned int ticks, void* parent);
		void renderChildren(SDL_Renderer* renderer);
		void assignChild(void* child, void* parent);
		void addComponentToChildren(GameComponentBehaviour* component);
		void setChildrenScale(double scale, void* parent);
		void setChildrenVelocity(Vector2D velocity, void* parent);
		void setChildrenTag(Tag tag);
		void setParentActive(void* obj, bool active);
		void revertChildrenLastMovement();
	
		bool lastMovedReverted;
		void* parent;
	
	private:
		vector<void*> children; // not of type GameObject to avoid a circular dependency
		void setChildrenActive(void* parent, bool active);
};
