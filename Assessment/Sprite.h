#pragma once

#include <SDL.h>

class Sprite
{
	public:
		Sprite();
		Sprite(int x, int y, int width, int height, unsigned int numberOfFrames, int offset, const char* pathToSprite, bool useColourKey = false);
		void render(SDL_Renderer* renderer);
		void load(SDL_Renderer* renderer);

		int x, y, width, height, offset;

	private:

		SDL_Texture* texture;
		const char* path;
		unsigned int numberOfFrames, animationSpeed, frameIndex;
};