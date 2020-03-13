#include <iostream>
#include "SDL.h"
#include "Timer.h"
#include "KeyboardHandler.h"
#include "Sprite.h"

void init(SDL_Window* window);
void input(SDL_Window* window);
void destroyRendererWindow(SDL_Renderer* renderer, SDL_Window* window);
void render(SDL_Renderer* renderer);
void update();

const float DELTA_TIME = 1000 / 60;
const int windowWidth = 800;
const int windowHeight = 600;
Timer timer;
SDL_Event event;
KeyboardHandler keyboardHandler;
Sprite sprite;
bool done;
bool isFullScreen;
//17x18
int main(int argc, char* argv[])
{
    done = false;
	isFullScreen = false;

    // Try to initialise, if we can't, then exit this program

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        return 1;

    // Create a window

    SDL_Window* window = SDL_CreateWindow("Callum Thompson CGP2015M: Tanks",
        250,
        250,
        windowWidth,
        windowHeight,
        SDL_WINDOW_RESIZABLE);

    init(window);

    // Create a renderer which draws to the window surface

    SDL_Renderer* renderer = SDL_CreateRenderer(window,
        -1,
        SDL_RENDERER_ACCELERATED);

    sprite = Sprite(0, 0, 17, 18, 4, 0, "/Content/marshmellow_man.png");
    sprite.load(renderer);

    while (!done)
    {
        // Resets a frame timer to zero

        timer.resetTicksTimer();

        input(window);
        update();
        render(renderer);

        // If less time has passed than allocated block, wait difference

        if (timer.getTicks() < DELTA_TIME)
            SDL_Delay(DELTA_TIME - timer.getTicks());
    }

    return 0;
}

void update()
{

}

void init(SDL_Window* window)
{
    // Get resolution

    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    auto screenWidth = DM.w;
    auto screenHeight = DM.h;

    SDL_SetWindowPosition(window, screenWidth / 2 - (windowWidth / 2), screenHeight / 2 - (windowHeight / 2));
}

void render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 20, SDL_ALPHA_OPAQUE);

    // Clear everything on the window, paint over with last known colour  

    SDL_RenderClear(renderer);

    //gameContainerConwaySquare.render(renderer, 0, 0, 255, 255);
    sprite.render(renderer);

    // Tell SDL we have finished drawing, SDL will actually show our changes

    SDL_RenderPresent(renderer);
}

void destroyRendererWindow(SDL_Renderer* renderer, SDL_Window* window)
{
    if (renderer)
        SDL_DestroyRenderer(renderer);

    if (window)
        SDL_DestroyWindow(window);
}

void input(SDL_Window* window)
{
    // While there are events

    while (SDL_PollEvent(&event))
    {
        keyboardHandler.handleKeyboardEvent(event);

        if (event.type == SDL_QUIT)
            done = true;

		if (event.type == SDL_KEYDOWN)
		{
			if (keyboardHandler.isPressed(SDLK_ESCAPE) && keyboardHandler.isPressed(SDLK_LSHIFT))
				done = true;

			if (keyboardHandler.isPressed(SDLK_f))
			{
				if (!isFullScreen)
				{
					SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
					isFullScreen = true;
				}
				else
				{
					SDL_SetWindowFullscreen(window, 0);
					isFullScreen = false;
				}
			}
		}
    }
}
