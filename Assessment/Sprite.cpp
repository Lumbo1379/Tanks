#include "Sprite.h"
#include "SDL_image.h"
#include <cmath>

Sprite::Sprite() {}

Sprite::Sprite(int x, int y, int width, int height, double scale, unsigned int numberOfFrames, int offset, const char* pathToSprite, bool useColourKey , vector<unsigned int> frameSpeeds)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->scale = scale;
	this->numberOfFrames = numberOfFrames;
	this->offset = offset;
	this->frameSpeeds = frameSpeeds;
	this->useColourKey = useColourKey;
	path = pathToSprite;
	animationSpeed = 100;
	frameIndex = 0;
	texture = nullptr;
	rotation = 0;
	flipped = SDL_FLIP_NONE;
	visible = true;
}

Sprite::~Sprite()
{
	if (texture != nullptr)
		SDL_DestroyTexture(texture);
}

void Sprite::render(SDL_Renderer* renderer) const
{
	if (visible)
	{
		auto calcX = (width + offset) * frameIndex;

		SDL_Rect src = { calcX, 0, width, height };
		SDL_Rect dest = { x, y, width * scale, height * scale };

		SDL_RenderCopyEx(renderer, texture, &src, &dest, rotation, &pivot, flipped);
	}
}

void Sprite::load(SDL_Renderer* renderer)
{
	IMG_Init(SDL_INIT_EVERYTHING);

	SDL_Surface* surface = IMG_Load(path);

	if (surface == nullptr)
		SDL_Log("Sprite not loaded!");

	/*if (useColourKey)
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 193, 193, 193));*/
	
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	this->texture = texture;

	if (useColourKey) // Not actually using colour key
	{
		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
		SDL_SetTextureAlphaMod(texture, 200);
	}
	
	SDL_FreeSurface(surface);

	IMG_Quit();

	setPivot({ width / 2, height / 2 });
}

void Sprite::update(unsigned int ticks, bool play, bool reverse)
{	
	auto tempFrameIndex = frameIndex;
	
	if (numberOfFrames > 1 && play)
		frameIndex = (ticks / frameSpeeds[frameIndex]) % numberOfFrames;

	if (reverse)
	{
		if (std::abs((int)tempFrameIndex - (int)frameIndex) == 1 
			|| tempFrameIndex == numberOfFrames - 1 && frameIndex == 0 
			|| frameIndex == numberOfFrames - 1 && tempFrameIndex == 0) // To avoid reverse playing much faster
		{
			if (tempFrameIndex > 0)
				frameIndex = tempFrameIndex - 1;
			else
				frameIndex = numberOfFrames - 1;
		}
		else
			frameIndex = tempFrameIndex;
	}
}

void Sprite::rotate(double rotation)
{
	recentSpriteRotation = rotation;
	
	this->rotation += rotation;

	if (this->rotation < 0)
		this->rotation += 360;
	else if (this->rotation > 360)
		this->rotation -= 360;
}

void Sprite::flip(bool horizontal)
{
	if (flipped == SDL_FLIP_NONE && horizontal)
		flipped = SDL_FLIP_HORIZONTAL;
	else if (flipped != SDL_FLIP_NONE)
		flipped = SDL_FLIP_VERTICAL;
	else
		flipped = SDL_FLIP_NONE;
}

void Sprite::setPivot(SDL_Point point)
{
	pivot = { (int)(point.x * scale), (int)(point.y * scale) };
}

void Sprite::revertLastRotation()
{
	rotate(-recentSpriteRotation);
}
