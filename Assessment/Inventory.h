#pragma once
#include "SDL.h"
#include <vector>
#include "Sprite.h"
#include "Item.h"
#include "InventoryItem.h"
#include "ScreenText.h"
#include "GameObject.h"


class Inventory
{
public:
	Inventory();
	~Inventory();

	void Init(SDL_Renderer* renderer, SDL_Window* window);
	void AddItemToInventory(int slot, int ItemID, int count);
	bool AddItemAndCount(int ItemID, int count);
	Item* LookupItemByID(int ID);
	void EquipItem();
	void MoveFocus(int key, bool isPressed);

	// render the inventory.. you might want to also render item counts on top! Also consider how to show text for items
	// perhaps when a highlight is applied you render a screentext to display the name of the item somewhere?
	void Render(SDL_Renderer* renderer);
	void Update(bool updateContents);

	int NumRows = 3;			// how many rows in the grid?
	int NumCols = 3;			// how many columns in the grid?
	int HighlightRow = 0;		// which Row contains the highlight?
	int HighlightCol = 0;		// which Column contains the highlight?
	int PositionX = 100;		// where does the grid begin from in X axis
	int PositionY = 100;		// where does the grid begin from in Y axis
	int CellSpacing = 10;		// how much space between Cells?
	int CellSize = 64;			// how big should the individual Cell size be?
	Sprite* highlightbackground;	// sprite used to draw a highlighted Cell background
	Sprite* emptybackground;		// sprite used to draw an empty Cell
	Sprite* invTemplate;
	ScreenText* desc;
	bool MOVE_LEFT, MOVE_RIGHT, MOVE_UP, MOVE_DOWN, RETURN;

	std::vector<InventoryItem> mInventoryItems;		// container for inventory items (our Cell data Item ID + count)
	std::vector<Item> mItems;						// container for Item data (basically our item database)
	InventoryItem trackSlot;
	InventoryItem turretSlot;
	vector<GameObject*> projectiles;
	GameObject* turret;
	GameObject* tankBody;

private:
	SDL_Renderer* renderer;
	vector<GameObject*> projectilesToStillChange;

};
