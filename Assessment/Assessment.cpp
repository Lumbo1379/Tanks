#include <iostream>
#include <fstream>
#include <string>
#include <SDL_mixer.h>
#include "SDL.h"
#include "Timer.h"
#include "KeyboardHandler.h"
#include "vector"
#include "GameObject.h"
#include "MoveTurretComponent.h"
#include "AnimateTankTrackComponent.h"
#include "MoveTankComponent.h"
#include "RotateTankComponent.h"
#include "ShootProjectileComponent.h"
#include "LogInSpatialGridComponent.h"
#include "SpatialGrid.h"
#include "RevertMovementCComponent.h"
#include "ReflectCComponent.h"
#include "ScreenText.h"
#include "Inventory.h"
#include "PathTile.h"
#include "MoveTowardsComponent.h"
#include "RotateTowardsComponent.h"
#include "AStarPathComponent.h"
#include "AnimateShotComponent.h"
#include "AnimateTankExplosionComponent.h"
#include "ShootPlayerComponent.h"
#include "Raycast.h"
#include "Mixer.h"
#include "Logger.h"

void init(SDL_Window* window);
void input(SDL_Window* window);
void destroyRendererWindow(SDL_Renderer* renderer, SDL_Window* window);
void render(SDL_Renderer* renderer);
void update();
GameObject* createProjectile(SDL_Renderer* renderer, GameObject* turret);
void createTank(SDL_Renderer* renderer, bool isPlayer, int x, int y);
void initialiseMap(SDL_Renderer* renderer);
bool checkNeighbours(int row, int column);

const float DELTA_TIME = 1000 / 60;
const int windowWidth = 1280;
const int windowHeight = 960;
const int segmentWidth = 150;
const int segmentHeight = 150;
Timer timer;
SDL_Event event;
KeyboardHandler keyboardHandler;
SpatialGrid spatialGrid;
vector<GameObject*> gameObjects;
ScreenText* score;
ScreenText* time;
int timerStartTicks;
int pauseTimerTicks;
int* scoreValue;
Inventory* inventory;
bool done;
bool isFullScreen;
bool showInventory;
bool showLoopLog;
bool pause;
bool gameOver;
bool crossClicked;
PathTile* pathGrid[12][16]; // [rows][columns]
GameObject* player;
GameObject* playerTurret;
vector<GameObject*> enemiesBody;
vector<GameObject*> enemiesTurret;
Mix_Music* SOUND_backgroundMusic;
Mix_Chunk* SOUND_shotSFX;
Mix_Chunk* SOUND_explosionSFX;
Logger* logger;

int main(int argc, char* argv[])
{
    done = false;
	isFullScreen = false;
	showInventory = false;
	showLoopLog = false;
	scoreValue = new int;
	pause = false;
	gameOver = false;
	crossClicked = false;
	pauseTimerTicks = 0;
	*scoreValue = 5000;
	
    // Try to initialise, if we can't, then exit this program

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        return 1;

    // Create a window

    SDL_Window* window = SDL_CreateWindow("Callum Thompson THO16647670 CGP2015M: Tanks",
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

	// Set reference width and height to scale from

	SDL_RenderSetLogicalSize(renderer, windowWidth, windowHeight);

	logger = new Logger("../Assessment/Content/log.txt");

	if (Mixer::init())
	{
		Mixer::addMusic("../Assessment/Content/background_music.wav");
		Mixer::addSFX("../Assessment/Content/explosion_sfx.wav"); // Id = 0
		Mixer::addSFX("../Assessment/Content/shot_sfx.wav"); // Id = 1
		Mixer::addSFX("../Assessment/Content/bounce_sfx.wav"); // Id = 2
	}

	inventory = new Inventory();
	inventory->Init(renderer, window);
	
	score = new ScreenText(window, "../Assessment/Content/roboto.ttf");
	score->setText("Score: 5000", 100, 100, false, 25, 255, 255, 255);

	time = new ScreenText(window, "../Assessment/Content/roboto.ttf");
	time->setText("Time Left: 60", 1000, 100, false, 25, 255, 255, 255);
	
	auto background = new GameObject(0, 0, 1280, 960);
	background->loadSprite(renderer, 1, 0, "../Assessment/Content/background.png");

	auto splashScreen = new GameObject(0, 0, 1280, 960);
	splashScreen->loadSprite(renderer, 1, 0, "../Assessment/Content/splash_screen.png");

	gameObjects.emplace_back(background);
	initialiseMap(renderer);

	auto raycast = new Raycast();
	std::copy(&pathGrid[0][0], &pathGrid[0][0] + 12 * 16, &raycast->pathGrid[0][0]); // Use copy instead of memcpy

    for (auto& enemy : enemiesBody)
    {
		std::copy(&pathGrid[0][0], &pathGrid[0][0] + 12 * 16, &enemy->getComponent<AStarPathComponent>().pathGridOriginal[0][0]); // Use copy instead of memcpy
		enemy->getComponent<AStarPathComponent>().player = player;
    }

	for (auto& enemy : enemiesTurret)
	{
		enemy->getComponent<ShootPlayerComponent>().player = player;
		enemy->getComponent<ShootPlayerComponent>().raycast = raycast;
	}

	SDL_Log("World loaded in %i ticks", SDL_GetTicks());

	int splashTicks = SDL_GetTicks();

	while (SDL_GetTicks() - splashTicks < 10000 && !crossClicked)
	{
		input(window);

		splashScreen->render(renderer);
		SDL_RenderPresent(renderer);
	}

	timerStartTicks = SDL_GetTicks();
	
    while (!done)
    {
        // Resets a frame timer to zero	

        timer.resetFrameTicksTimer();

        input(window);
        update();
        render(renderer);

        // If less time has passed than allocated block, wait difference

		if (timer.getFrameTicks() < DELTA_TIME)
			SDL_Delay(DELTA_TIME - timer.getFrameTicks());
    }

	int checkPlayerDoesNotDieTicks = SDL_GetTicks();
	
	while (SDL_GetTicks() - checkPlayerDoesNotDieTicks < 3500 && !crossClicked && 60 - (SDL_GetTicks() - timerStartTicks) / 1000 > 0) // Check player not hit by projectile immediately after killed enemy
	{
		// Resets a frame timer to zero	

		timer.resetFrameTicksTimer();

		input(window);
		update();
		render(renderer);

		// If less time has passed than allocated block, wait difference

		if (timer.getFrameTicks() < DELTA_TIME)
			SDL_Delay(DELTA_TIME - timer.getFrameTicks());
	}

	if (gameOver)
	{
		int gameOverTicks = SDL_GetTicks();
		auto endScreen = new GameObject(0, 0, 1280, 960);
		endScreen->loadSprite(renderer, 1, 0, "../Assessment/Content/black_screen.png");

		auto endText = new ScreenText(window, "../Assessment/Content/roboto.ttf");
		
		if (!player->active)
			endText->setText("You Lost! :(", 640, 480, true, 52, 255, 255, 255);
		else if (enemiesBody.empty())
			endText->setText("You Won!\nWith a score of " + std::to_string(*scoreValue), 640, 480, true, 52, 255, 255, 255);

		while (SDL_GetTicks() - gameOverTicks < 5000 && !crossClicked)
		{
			input(window);

			endScreen->render(renderer);
			endText->render();
			SDL_RenderPresent(renderer);
		}
	}

	SDL_Log("Game closed");
	
	Mixer::onClose();
	destroyRendererWindow(renderer, window);
	SDL_Quit();
	
    return 0;
}

void update()
{
	if (!pause)
	{
		int startTicks = SDL_GetTicks();

		Mixer::playMusic();

		if (!showInventory)
		{
			for (auto gameObject : gameObjects)
				gameObject->update(timer.getAnimationTicks());

			if (showLoopLog)
				SDL_Log("Gameobjects updated | frame loop time: %i", timer.getFrameTicks());

			spatialGrid.checkForCollisions();

			if (showLoopLog)
				SDL_Log("Collisions updated | frame loop time: %i", timer.getFrameTicks());

			time->setText("Time Left: " + std::to_string(60 - (SDL_GetTicks() - timerStartTicks) / 1000));
		}
		else
		{
			timerStartTicks += (SDL_GetTicks() - pauseTimerTicks);
			pauseTimerTicks = SDL_GetTicks();
		}

		inventory->Update(showInventory);

		for (int b = 0; b < enemiesBody.size(); ++b)
		{
			if (!enemiesBody[b]->active)
			{
				enemiesBody.erase(enemiesBody.begin() + b);
				b--;
				*scoreValue = *scoreValue + 1000;

				SDL_Log("Enemy tank destroyed!");
			}
		}

		score->setText("Score: " + std::to_string(*scoreValue));

		if (showLoopLog)
			SDL_Log("Update took %i ticks to complete", SDL_GetTicks() - startTicks);

		pauseTimerTicks = SDL_GetTicks();

		if (60 - (SDL_GetTicks() - timerStartTicks) / 1000 <= 0)
		{
			done = true;
			gameOver = true;
		}
		else if (enemiesBody.empty() || !player->active)
		{
			done = true;
			gameOver = true;
		}
	}
	else
	{
		timerStartTicks += (SDL_GetTicks() - pauseTimerTicks);
		pauseTimerTicks = SDL_GetTicks();
	}
}

void init(SDL_Window* window)
{
    // Get resolution

    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    auto screenWidth = DM.w;
    auto screenHeight = DM.h;

    SDL_SetWindowPosition(window, screenWidth / 2 - (windowWidth / 2), screenHeight / 2 - (windowHeight / 2));

	SDL_Log("Window initialized");
}

void render(SDL_Renderer* renderer)
{
	if (!pause)
	{
		int startTicks = SDL_GetTicks();

		SDL_SetRenderDrawColor(renderer, 0, 0, 20, SDL_ALPHA_OPAQUE);

		// Clear everything on the window, paint over with last known colour  

		SDL_RenderClear(renderer);

		for (auto gameObject : gameObjects)
			gameObject->render(renderer);

		score->render();
		time->render();

		if (showInventory)
			inventory->Render(renderer);

		// Tell SDL we have finished drawing, SDL will actually show our changes

		SDL_RenderPresent(renderer);

		if (showLoopLog)
			SDL_Log("Render took %i ticks to complete", SDL_GetTicks() - startTicks);
	}
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
	int startTicks = SDL_GetTicks();
	
    // While there are events

    while (SDL_PollEvent(&event))
    {
        keyboardHandler.handleKeyboardEvent(event); // Useful when capturing multiple keys pressed/held and then used for other single presses for consistency

		if (event.type == SDL_QUIT)
		{
			crossClicked = true;
			done = true;
		}
		else if (event.type == SDL_KEYDOWN)
		{
			if (!event.key.repeat)
				SDL_Log("[KEY DOWN] time %d; code %d; char %s;", event.key.timestamp, event.key.keysym.sym, SDL_GetKeyName(event.key.keysym.sym));
			
			if (keyboardHandler.isPressed(SDLK_ESCAPE) && keyboardHandler.isPressed(SDLK_LSHIFT))
			{
				crossClicked = true;
				done = true;
			}

			if (keyboardHandler.isPressed(SDLK_w) ||keyboardHandler.isPressed(SDLK_s) || keyboardHandler.isPressed(SDLK_a) || keyboardHandler.isPressed(SDLK_d) || keyboardHandler.isPressed(SDLK_SPACE) || keyboardHandler.isPressed(SDLK_RETURN))
			{
				if (!showInventory)
				{
					for (auto obj : gameObjects)
						obj->input(event.key.keysym.sym, true);
				}
				else
					inventory->MoveFocus(event.key.keysym.sym, true);
			}

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

			if (keyboardHandler.isPressed(SDLK_TAB))
			{
				if (showInventory)
					showInventory = false;
				else
					showInventory = true;
			}

			if (keyboardHandler.isPressed(SDLK_ESCAPE))
			{
				if (showInventory)
					showInventory = false;
			}

			if (keyboardHandler.isPressed(SDLK_m))
				Mixer::mute();

			if (keyboardHandler.isPressed(SDLK_EQUALS))
				Mixer::volumeUp();

			if (keyboardHandler.isPressed(SDLK_MINUS))
				Mixer::volumeDown();

			if (keyboardHandler.isPressed(SDLK_1))
			{
				auto& comp = player->getComponent<MoveTankComponent>();
				
				if (comp.SPEED_BASE > 0)
					comp.SPEED_BASE -= 0.1;

				SDL_Log("Player speed decreased to %f", comp.SPEED_BASE);
			}

			if (keyboardHandler.isPressed(SDLK_2))
			{
				auto& comp = player->getComponent<MoveTankComponent>();
			
				comp.SPEED_BASE += 0.1;

				SDL_Log("Player speed increased to %f", comp.SPEED_BASE);
			}

			if (keyboardHandler.isPressed(SDLK_3))
			{
				auto& comp = playerTurret->getComponent<MoveTurretComponent>();

				if (comp.SPEED > 0)
					comp.SPEED -= 0.1;

				SDL_Log("Player turret speed decreased to %f", comp.SPEED);
			}

			if (keyboardHandler.isPressed(SDLK_4))
			{
				auto& comp = playerTurret->getComponent<MoveTurretComponent>();

				comp.SPEED += 0.1;

				SDL_Log("Player turret speed increased to %f", comp.SPEED);
			}

			if (keyboardHandler.isPressed(SDLK_v))
			{
				if (showLoopLog)
					showLoopLog = false;
				else
					showLoopLog = true;
			}

			if (keyboardHandler.isPressed(SDLK_p))
			{
				if (pause)
				{
					SDL_Log("Game un-paused");
					pause = false;
				}
				else
				{
					SDL_Log("Game paused");
					pause = true;
				}
			}
		}
		else if (event.type == SDL_KEYUP)
		{
			if (!event.key.repeat)
				SDL_Log("[KEY UP] time %d; code %d; char %s;", event.key.timestamp, event.key.keysym.sym, SDL_GetKeyName(event.key.keysym.sym));
			
			if (keyboardHandler.isReleased(SDLK_w) || keyboardHandler.isReleased(SDLK_s) || keyboardHandler.isReleased(SDLK_a) || keyboardHandler.isReleased(SDLK_d) || keyboardHandler.isReleased(SDLK_SPACE))
			{
				if (!showInventory)
				{
					for (auto obj : gameObjects)
						obj->input(event.key.keysym.sym, false);
				}
			}
		}
    }

	auto keyStates = SDL_GetKeyboardState(nullptr); // To avoid delay when capturing arrow keys

	for (auto obj : gameObjects)
	{
		obj->input(SDL_SCANCODE_LEFT, keyStates[SDL_SCANCODE_LEFT]);
		obj->input(SDL_SCANCODE_RIGHT, keyStates[SDL_SCANCODE_RIGHT]);
	}

	if (showLoopLog)
		SDL_Log("Input took %i ticks to complete", SDL_GetTicks() - startTicks);
}

GameObject* createProjectile(SDL_Renderer* renderer, GameObject* turret)
{
	auto projectile = new GameObject(0, 0, 32, 32);
	projectile->loadSprite(renderer, 0, 0, "../Assessment/Content/projectile.png");
	projectile->sprite->setPivot({ turret->sprite->pivot.x - 11, turret->sprite->pivot.y + 16});
	projectile->setActive(false);
	projectile->setScale(0.3);

	projectile->setTag(Tag::Projectile);

	projectile->addComponent(new LogInSpatialGridComponent(segmentWidth, segmentHeight));
	projectile->getComponent<LogInSpatialGridComponent>().spatialGrid = &spatialGrid;
	projectile->addCollisionComponent(new ReflectCComponent);

	return projectile;
}

void createTank(SDL_Renderer* renderer, bool isPlayer, int x, int y)
{
	const char* bodyPath;
	const char* turretPath;

	if (isPlayer)
	{
		bodyPath = "../Assessment/Content/player_tank.png";
		turretPath = "../Assessment/Content/player_turret.png";
	}
	else
	{
		bodyPath = "../Assessment/Content/enemy_tank.png";
		turretPath = "../Assessment/Content/enemy_turret.png";
	}
	
	auto tank = new GameObject(x, y, 94, 212);
	tank->setScale(0.35);

	auto tankExplosionEffect = new GameObject(0, 0, 256, 256);
	tankExplosionEffect->loadSprite(renderer, 8, 0, "../Assessment/Content/tank_explosion.png", false, vector<unsigned int>(8, 50));
	tankExplosionEffect->setScale(0.6);
	tankExplosionEffect->addComponent(new AnimateTankExplosionComponent());
	tankExplosionEffect->sprite->visible = false;

	auto tankBody = new GameObject(0, 0, 170, 252);
	tank->addChild(tankBody);
	tankBody->loadSprite(renderer, 1, 0, bodyPath);

	auto leftTrack = new GameObject(1, 0, 42, 246);
	tankBody->addChild(leftTrack);
	leftTrack->loadSprite(renderer, 4, 0, "../Assessment/Content/tracks.png", false, vector<unsigned int>(4, 100)); // Load sprite after setting parent so uses parents x and y
	leftTrack->sprite->setPivot({ 84, 126 });

	auto rightTrack = new GameObject(126, 0, 42, 246);
	tankBody->addChild(rightTrack);
	rightTrack->loadSprite(renderer, 4, 0, "../Assessment/Content/tracks.png", false, vector<unsigned int>(4, 100));
	rightTrack->sprite->setPivot({ -42, 126 });

	auto turret = new GameObject(38, -30, 94, 212);
	tank->addChild(turret);
	turret->loadSprite(renderer, 1, 0, turretPath);
	turret->sprite->setPivot({ 47, 162 });

	auto projectileEffect = new GameObject(-15, -75, 128, 128);
	turret->addChild(projectileEffect);
	projectileEffect->loadSprite(renderer, 3, 0, "../Assessment/Content/projectile_shoot_effect.png", false, vector<unsigned int>(3, 100));
	projectileEffect->sprite->pivot.y += 57;
	projectileEffect->sprite->visible = false;

	tank->setTag(Tag::Tank);

	// Add components after children assigned

	turret->addComponent(new AnimateShotComponent());
	turret->getComponent<AnimateShotComponent>().projectileEffect = projectileEffect;
	
	if (isPlayer)
	{
		player = tankBody;
		playerTurret = turret;

		turret->addComponent(new MoveTurretComponent());
		turret->addComponent(new ShootProjectileComponent());
		turret->getComponent<ShootProjectileComponent>().animateShotComponent = &turret->getComponent<AnimateShotComponent>();
		leftTrack->addComponent(new AnimateTankTrackComponent());
		rightTrack->addComponent(new AnimateTankTrackComponent());
		tankBody->addComponent(new RotateTankComponent());
		tankBody->addComponent(new MoveTankComponent(0, windowWidth - tankBody->width * tankBody->scale, 0, windowHeight - tankBody->height * tankBody->scale));
		tankBody->getComponent<MoveTankComponent>().turret = turret;
		tankBody->getComponent<MoveTankComponent>().score = scoreValue;

		auto& shootProjectile = turret->getComponent<ShootProjectileComponent>();

		for (auto p = 0; p < shootProjectile.MAX_PROJECTILES_BASE; p++)
		{
			shootProjectile.projectiles[p] = createProjectile(renderer, turret);
			gameObjects.emplace_back(shootProjectile.projectiles[p]);
			inventory->projectiles.emplace_back(shootProjectile.projectiles[p]);
		}

		inventory->turret = turret;
		inventory->tankBody = tankBody;
	}
	else
	{
		enemiesBody.emplace_back(tankBody); // To add player reference at later stage
		enemiesTurret.emplace_back(turret);
		
		tankBody->addComponent(new MoveTowardsComponent());
		tankBody->getComponent<MoveTowardsComponent>().turret = turret;
		tankBody->getComponent<MoveTowardsComponent>().setTarget(Vector2D(tank->x, tank->y));
		tankBody->addComponent(new RotateTowardsComponent());
		tankBody->getComponent<RotateTowardsComponent>().moveTowardsComponent = &tankBody->getComponent<MoveTowardsComponent>();
		tankBody->addComponent(new AStarPathComponent());
		tankBody->getComponent<AStarPathComponent>().moveTowardsComponent = &tankBody->getComponent<MoveTowardsComponent>();
		tankBody->getComponent<AStarPathComponent>().rotateTowardsComponent = &tankBody->getComponent<RotateTowardsComponent>();
		turret->addComponent(new RotateTowardsComponent());
		turret->addComponent(new ShootPlayerComponent());
		turret->getComponent<ShootPlayerComponent>().rotateTowardsComponent = &turret->getComponent<RotateTowardsComponent>();
		turret->getComponent<ShootPlayerComponent>().animateShotComponent = &turret->getComponent<AnimateShotComponent>();
		turret->getComponent<RotateTowardsComponent>().look = true;

		auto& shootProjectile = turret->getComponent<ShootPlayerComponent>();

		for (auto p = 0; p < shootProjectile.MAX_PROJECTILES; p++)
		{
			shootProjectile.projectiles[p] = createProjectile(renderer, turret);
			gameObjects.emplace_back(shootProjectile.projectiles[p]);
		}
	}
	
	tankBody->addComponent(new LogInSpatialGridComponent(segmentWidth, segmentHeight));
	tankBody->getComponent<LogInSpatialGridComponent>().spatialGrid = &spatialGrid;
	tankBody->addCollisionComponent(new RevertMovementCComponent());
	tankBody->getComponent<RevertMovementCComponent>().animateTankExplosionComponent = &tankExplosionEffect->getComponent<AnimateTankExplosionComponent>();
	
	gameObjects.emplace_back(tank); // Only the parent is emplaced, the parent updates and renders all the children
	gameObjects.emplace_back(tankExplosionEffect);
}

void initialiseMap(SDL_Renderer* renderer)
{	
	string line;
	int lineNum = -1;
	int lineColumnNum;
	int startTicks = SDL_GetTicks();
	
	ifstream file;
	file.open("../Assessment/Content/map.txt");

	if (file.is_open())
	{
		while (getline(file, line))
		{
			lineNum++;
			lineColumnNum = -1;
			
			for (auto character : line)
			{	
				lineColumnNum++;

				pathGrid[lineNum][lineColumnNum] = new PathTile();
				auto& tile = pathGrid[lineNum][lineColumnNum];

				tile->cost = 1000.0f;
				
				switch (character)
				{
					case 'C':
					{
						tile->isClosed = false;
							
						break;
					}
					case 'W':
					{
						tile->isClosed = true;
							
						auto wall = new GameObject(lineColumnNum * 80, lineNum * 80, 80, 80);
						wall->loadSprite(renderer, 1, 0, "../Assessment/Content/wall.png");
							
						wall->addComponent(new LogInSpatialGridComponent(segmentWidth, segmentHeight));
						wall->getComponent<LogInSpatialGridComponent>().spatialGrid = &spatialGrid;
						gameObjects.emplace_back(wall);

						SDL_Log("Wall created at row %i, column %i", lineNum, lineColumnNum);
							
						break;
					}
					case 'P':
					{
						tile->isClosed = false;
							
						createTank(renderer, true, lineColumnNum * 80, lineNum * 80);

						SDL_Log("Player created at row %i, column %i", lineNum, lineColumnNum);
							
						break;
					}
					case 'E':
					{
						tile->isClosed = false;
							
						createTank(renderer, false, lineColumnNum * 80, lineNum * 80);

						SDL_Log("Enemy created at row %i, column %i", lineNum, lineColumnNum);

						break;
					}
					default:
						break;
				}
			}
		}

		// Remove tight spaces from possible route
		
		for (int row = 0; row < 12; ++row)
		{
			for (int column = 0; column < 16; ++column)
			{
				if (!pathGrid[row][column]->isClosed)
					pathGrid[row][column]->toClose = checkNeighbours(row, column);
			}
		}

		for (int row = 0; row < 12; ++row)
		{
			for (int column = 0; column < 16; ++column)
			{
				if (pathGrid[row][column]->toClose)
					pathGrid[row][column]->isClosed = true;
			}
		}
	}

	file.close();

	SDL_Log("Map created in %i ticks", SDL_GetTicks() - startTicks);
}

bool checkNeighbours(int row, int column)
{
	bool closedAbove = false;
	bool closedBelow = false;
	bool closedLeft = false;
	bool closedRight = false;

	if (row == 0)
		closedAbove = true;
	else if (pathGrid[row - 1][column]->isClosed)
		closedAbove = true;

	if (row == 11)
		closedBelow = true;
	else if (pathGrid[row + 1][column]->isClosed)
		closedBelow = true;

	if (column == 0)
		closedLeft = true;
	else if (pathGrid[row][column - 1]->isClosed)
		closedLeft = true;

	if (column == 15)
		closedRight = true;
	else if (pathGrid[row][column + 1]->isClosed)
		closedRight = true;

	return closedAbove || closedBelow || closedLeft || closedRight;
}
