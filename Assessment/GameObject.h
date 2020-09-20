#pragma once

#include <SDL.h>
#include "Vector2D.h"
#include "Sprite.h"
#include "GameObjectBehaviour.h"
#include "GameComponentBehaviour.h"

enum class Tag
{
	Default,
	Tank,
	Projectile
};

class GameObject : public GameObjectBehaviour
{
	public:
		GameObject();
		GameObject(int x, int y, int width, int height, Vector2D maxVelocity = Vector2D(0, 0), Vector2D minVelocity = Vector2D(0, 0));
		~GameObject();
		void loadSprite(SDL_Renderer* renderer, unsigned int numberOfFrames, int offset, const char* pathToSprite, bool useColourKey = false, vector<unsigned int> frameSpeeds = vector<unsigned int>());
		void update(unsigned int ticks);
		void input(int key, bool isPressed);
		void render(SDL_Renderer* renderer);
		void addChild(GameObject* child);
		void addComponent(GameComponentBehaviour* component);
		void addCollisionComponent(GameComponentBehaviour* collisionComponent);
		void setScale(double scale);
		void setActive(bool active);
		void setVelocity(Vector2D velocity);
		void handleCollision(GameObject* collisionObj, Vector2D contactPoint);
		void setTag(Tag tag);
		Vector2D* getVelocity();
		void revertLastMove();
		template <class T> T& getComponent();
	
		Tag tag;
		int x, y, width, height;
		bool MOVE_LEFT, MOVE_RIGHT, MOVE_UP, MOVE_DOWN, MOVE_ARROW_LEFT, MOVE_ARROW_RIGHT, SPACE;
		Sprite* sprite;
		double scale;
		bool active;

	private:	
		SDL_Rect rect;
		vector<GameComponentBehaviour*> components;
		vector<GameComponentBehaviour*> collisionComponents;
		Vector2D velocity;
};

template<class T>
T& GameObject::getComponent()
{
	for (auto component : components)
	{
		auto checkComponent = dynamic_cast<T*> (component);
		
		if (checkComponent != nullptr)
			return *checkComponent;
	}

	for (auto component : collisionComponents)
	{
		auto checkComponent = dynamic_cast<T*> (component);

		if (checkComponent != nullptr)
			return *checkComponent;
	}
}
