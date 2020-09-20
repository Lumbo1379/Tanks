#pragma once

#include <SDL.h>
#include <vector>

using namespace std;

class Sprite
{
	public:
		Sprite();
		Sprite(int x, int y, int width, int height, double scale, unsigned int numberOfFrames, int offset, const char* pathToSprite, bool useColourKey = false, vector<unsigned int> frameSpeeds = vector<unsigned int>());
		~Sprite();
		void render(SDL_Renderer* renderer) const;
		void load(SDL_Renderer* renderer);
		void update(unsigned int ticks, bool play, bool reverse);
		void rotate(double rotation);
		void flip(bool horizontal);
		void setPivot(SDL_Point point);
		void revertLastRotation();
	
		int x, y, width, height, offset;
		unsigned int numberOfFrames;
		vector<unsigned int> frameSpeeds;
		SDL_Point pivot;
		double rotation, scale; // Scale will always be same in x and y
		int recentSpriteRotation;
		unsigned int frameIndex;
		bool visible;

	private:
		SDL_Texture* texture;
		SDL_RendererFlip flipped;
		const char* path;
		unsigned int animationSpeed;
		bool useColourKey;
};
