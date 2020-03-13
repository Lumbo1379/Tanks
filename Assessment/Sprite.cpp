#include "Sprite.h"
#include "SDL_image.h"

Sprite::Sprite() {}

Sprite::Sprite(int x, int y, int width, int height, unsigned int numberOfFrames, int offset, const char* pathToSprite, bool useColourKey)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->numberOfFrames = numberOfFrames;
	this->offset = offset;
	path = pathToSprite;
	animationSpeed = 100;
	frameIndex = 0;
	texture = nullptr;
}

void Sprite::render(SDL_Renderer* renderer)
{
	auto calcX = (width + offset) * frameIndex;

	SDL_Rect src = { calcX, 0, width, height };
	SDL_Rect dest = { x, y, width, height };

	SDL_RenderCopy(renderer, texture, &src, &dest);
}

void Sprite::load(SDL_Renderer* renderer)
{
	IMG_Init(SDL_INIT_EVERYTHING);

	SDL_Surface* surface = IMG_Load(path);

	texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	IMG_Quit();
}
