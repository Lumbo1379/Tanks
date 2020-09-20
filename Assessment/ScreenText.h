/**
* @author Benjamin Williams <bwilliams@lincoln.ac.uk> modified by Phil Carlisle to use SDL_ttf instead of sttbimage
*
* @brief  A file which renders text to the screen.
* @notes  Consult the lecture slides for the usage of this class.
*/
#pragma once

#define _CRT_SECURE_NO_WARNINGS 1

#include <string>
#include <cstdarg>
#include "SDL.h"
#include "SDL_ttf.h"

class ScreenText
{
	public:
		ScreenText();
		ScreenText(SDL_Window* window, const std::string& fontPath, int fontsize = 32);
		~ScreenText(void);
		void render(void);
		void move(int x, int y);
		void setColor(unsigned int color);
		void setColor(unsigned char r, unsigned char g, unsigned char b);
		void setColor(const SDL_Color color);
		void setText(const std::string& text);
		void setText(const std::string& text, const int posX, const int posY, const bool bCenterText = false, const int fontSize = 48, const int r = 255, const int g = 255, const int b = 255);
		void setFontSize(const int fontSize);
		void updateSurface(void);

	private:
		bool loadFontData(const std::string& fontPath);
		void createTextSurface(void);

		SDL_Renderer* renderer = NULL;
		SDL_Window* window = NULL;
		int width = 0, height = 0;
		int x = 0, y = 0;
		int fontScale = 32;
		SDL_Color color = { 255,255,255,255 };
		SDL_Color backgroundcolour = { 0,0,0,0 };
		std::string currenttext;
		std::string fontpath;
		TTF_Font* fontptr = NULL;
		SDL_Surface* textSurface = NULL;
		SDL_Texture* textTexture = NULL;
};