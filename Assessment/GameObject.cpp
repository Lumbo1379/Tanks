#include "GameObject.h"
#include <iostream>

GameObject::GameObject() {}

GameObject::GameObject(int x, int y, int width, int height, Vector2D maxVelocity, Vector2D minVelocity)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	MOVE_LEFT = false;
	MOVE_RIGHT = false;
	MOVE_UP = false;
	MOVE_DOWN = false;
	MOVE_ARROW_LEFT = false;
	MOVE_ARROW_RIGHT = false;
	SPACE = false;
	velocity.x = 0;
	velocity.y = 0;
	scale = 1;
	sprite = nullptr;
	active = true;
	tag = Tag::Default;
}

GameObject::~GameObject()
{
	delete sprite; // No need to check if null as deleting null pointer has no effect
}

void GameObject::loadSprite(SDL_Renderer* renderer, unsigned int numberOfFrames, int offset, const char* pathToSprite, bool useColourKey, vector<unsigned int> frameSpeeds)
{
	sprite = new Sprite(x, y, width, height, scale, numberOfFrames, offset, pathToSprite, useColourKey, frameSpeeds);
	sprite->load(renderer);
}

void GameObject::update(unsigned int ticks)
{
	if (!active) return;
	
	lastMovedReverted = false;
	
	for (auto component : components)
		component->update(ticks, this);

	for (auto collisionComponent : collisionComponents)
		collisionComponent->update(ticks, this);

	x += velocity.x;
	y += velocity.y;

	rect.x = x;
	rect.y = y;
	rect.h = height;
	rect.w = width;

	if (sprite != nullptr)
	{
		sprite->x = x;
		sprite->y = y;
	}

	updateChildren(ticks, this);
}

void GameObject::input(int key, bool isPressed)
{
	switch (key)
	{
		case SDLK_a: {MOVE_LEFT = isPressed; break; }
		case SDLK_d: {MOVE_RIGHT = isPressed; break; }
		case SDLK_w: {MOVE_UP = isPressed; break; }
		case SDLK_s: {MOVE_DOWN = isPressed; break; }
		case SDLK_SPACE: {SPACE = isPressed; break; }
		case SDL_SCANCODE_LEFT: {MOVE_ARROW_LEFT = isPressed; break; }
		case SDL_SCANCODE_RIGHT: {MOVE_ARROW_RIGHT = isPressed; break; }
	}
}

void GameObject::render(SDL_Renderer* renderer)
{
	renderChildren(renderer); // Render children first so they are behind the parent
	
	if (sprite != nullptr && active)
		sprite->render(renderer);
}

void GameObject::addChild(GameObject* child)
{
	assignChild(child, this);
}

void GameObject::addComponent(GameComponentBehaviour* component)
{
	SDL_Log("Game component %s added to object with Tag: %i at x: %i, y: %i", typeid(*component).name(), tag, x, y);
	
	components.emplace_back(component);
	
	component->start();
}

void GameObject::addCollisionComponent(GameComponentBehaviour* collisionComponent)
{
	SDL_Log("Game collision component %s added to object with Tag: %i at x: %i, y: %i", typeid(*collisionComponent).name(), tag, x, y);
	
	collisionComponents.emplace_back(collisionComponent);

	collisionComponent->start();
}

void GameObject::setScale(double scale)
{
	this->scale = scale;

	if (sprite != nullptr)
		sprite->scale = scale;

	setChildrenScale(scale, this);
}

void GameObject::setActive(bool active)
{
	setParentActive(this, active);
}

void GameObject::setVelocity(Vector2D velocity)
{
	this->velocity = velocity;

	setChildrenVelocity(velocity, this);
}

void GameObject::handleCollision(GameObject* collisionObj, Vector2D contactPoint)
{
	for (auto collisionComponent : collisionComponents)
		collisionComponent->handleCollision(collisionObj, this, contactPoint);
}

void GameObject::setTag(Tag tag)
{
	this->tag = tag;

	setChildrenTag(tag);
}

Vector2D* GameObject::getVelocity()
{
	return &velocity;
}

void GameObject::revertLastMove()
{	
	x -= velocity.x;
	y -= velocity.y;

	velocity.x = 0;
	velocity.y = 0;

	rect.x = x;
	rect.y = y;
	rect.h = height;
	rect.w = width;

	if (sprite != nullptr)
	{
		sprite->x = x;
		sprite->y = y;

		sprite->revertLastRotation();
	}

	revertChildrenLastMovement();
}
