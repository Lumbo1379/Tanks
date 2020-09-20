#include "GameObjectBehaviour.h"
#include "GameObject.h"

GameObjectBehaviour::GameObjectBehaviour() = default;

void GameObjectBehaviour::updateChildren(unsigned int ticks, void* parent)
{
	auto parentPtr{ static_cast<GameObject*>(parent) };
	
	for (auto child : children)
	{
		auto childPtr{ static_cast<GameObject*>(child) };

		if (parentPtr->sprite != nullptr)
		{
			childPtr->sprite->rotation = parentPtr->sprite->rotation;
			childPtr->sprite->recentSpriteRotation = parentPtr->sprite->recentSpriteRotation;
		}

		childPtr->MOVE_LEFT = parentPtr->MOVE_LEFT;
		childPtr->MOVE_RIGHT = parentPtr->MOVE_RIGHT;
		childPtr->MOVE_UP = parentPtr->MOVE_UP;
		childPtr->MOVE_DOWN = parentPtr->MOVE_DOWN;
		childPtr->MOVE_ARROW_LEFT = parentPtr->MOVE_ARROW_LEFT;
		childPtr->MOVE_ARROW_RIGHT = parentPtr->MOVE_ARROW_RIGHT;
		childPtr->SPACE = parentPtr->SPACE;

		childPtr->update(ticks);
	}
}

void GameObjectBehaviour::renderChildren(SDL_Renderer* renderer)
{
	for (auto child : children)
		static_cast<GameObject*>(child)->render(renderer);
}

void GameObjectBehaviour::assignChild(void* child, void* parent)
{
	auto childPtr{ static_cast<GameObject*>(child) };
	auto parentPtr{ static_cast<GameObject*>(parent) };
	
	childPtr->x = parentPtr->scale * childPtr->x + parentPtr->x;
	childPtr->y = parentPtr->scale * childPtr->y + parentPtr->y;
	
	childPtr->setScale(childPtr->scale * parentPtr->scale);

	childPtr->parent = parent;

	children.emplace_back(child);
}

void GameObjectBehaviour::addComponentToChildren(GameComponentBehaviour* component)
{
	for (auto child : children)
		static_cast<GameObject*>(child)->addComponent(component);
}

void GameObjectBehaviour::setChildrenScale(double scale, void* parent)
{
	auto parentPtr{ static_cast<GameObject*>(parent) };

	for (auto child : children)
	{
		auto childPtr = static_cast<GameObject*>(child);
		childPtr->setScale(childPtr->scale * parentPtr->scale);
	}
}

void GameObjectBehaviour::setChildrenVelocity(Vector2D velocity, void* parent)
{
	auto parentPtr{ static_cast<GameObject*>(parent) };

	for (auto child : children)
	{
		auto childPtr = static_cast<GameObject*>(child);
		childPtr->setVelocity(velocity);
	}
}

void GameObjectBehaviour::setChildrenTag(Tag tag)
{
	for (auto child : children)
	{
		auto childPtr = static_cast<GameObject*>(child);
		childPtr->setTag(tag);
	}
}

void GameObjectBehaviour::setParentActive(void* obj, bool active)
{
	if (parent != nullptr)
	{
		GameObject* parentPtr = { static_cast<GameObject*>(parent) };
		GameObject* previousParentPtr = parentPtr;

		while (parentPtr != nullptr)
		{
			previousParentPtr = parentPtr;
			parentPtr = { static_cast<GameObject*>(previousParentPtr->parent) };
		}

		previousParentPtr->active = active;

		setChildrenActive(previousParentPtr, active);
	}
	else
	{
		auto objPtr = static_cast<GameObject*>(obj);
		objPtr->active = active;

		setChildrenActive(objPtr, active);
	}
}

void GameObjectBehaviour::revertChildrenLastMovement()
{
	lastMovedReverted = true;
	
	for (auto child : children)
	{
		auto childPtr = static_cast<GameObject*>(child);

		if (!childPtr->lastMovedReverted)
			childPtr->revertLastMove();
	}

	if (parent != nullptr)
	{
		auto parentPtr{ static_cast<GameObject*>(parent) };

		if (!parentPtr->lastMovedReverted)
			parentPtr->revertLastMove();
	}
}

void GameObjectBehaviour::setChildrenActive(void* parent, bool active)
{
	auto parentPtr = static_cast<GameObject*>(parent);
	
	for (auto child : parentPtr->children)
	{
		setChildrenActive(child, active);
		
		auto childPtr = static_cast<GameObject*>(child);
		childPtr->active = active;
	}
}
