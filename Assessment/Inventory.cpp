#include "Inventory.h"
#include "SDL_image.h"
#include "ReflectCComponent.h"
#include "ShootProjectileComponent.h"
#include "MoveTankComponent.h"
#include "RevertMovementCComponent.h"

Inventory::Inventory(){}

Inventory::~Inventory(){}

// This should be called to setup the Inventory data... it will create a few hard-coded test items
// and setup the sprites for rendering
void Inventory::Init(SDL_Renderer* renderer, SDL_Window* window)
{
	this->renderer = renderer;
	
	// setup the inventory defaults, in this case, we're adding six entries (1 row with 6 columns)
	// note: the resize function changes the size of the dynamic array which calls the default constructor for each entry
	mInventoryItems.resize(NumRows * NumCols);

	invTemplate = new Sprite(PositionX - CellSpacing, PositionY - CellSpacing, 700, 600, 1, 1, 0, "../Assessment/Content/inv_template.png", true);
	invTemplate->load(renderer);
	
	// load the sprite texture for the highlighted background cell
	highlightbackground = new Sprite(PositionX, PositionY, CellSize, CellSize, 1, 1, 0, "../Assessment/Content/inv_background_highlighted.png");
	highlightbackground->load(renderer);

	// load the sprite texture for the "empty" background cell
	emptybackground = new Sprite(PositionX, PositionY, CellSize, CellSize, 1, 1, 0, "../Assessment/Content/inv_background_regular.png");
	emptybackground->load(renderer);

	desc = new ScreenText(window, "../Assessment/Content/roboto.ttf");
	desc->setText(" ", PositionX + 358, PositionY + 10, false, 32, 0, 0, 0);

	// hard code some Items, we will replace this with loading from JSON later
	Item one;
	one.ID = 1;
	one.Icon = new Sprite(PositionX, PositionY, CellSize, CellSize, 1, 1, 0, "../Assessment/Content/projectile_icon.png");
	one.Description = "A regular speed projectile that bounces two times.";
	one.Icon->load(renderer);
	mItems.push_back(one);

	Item two;
	two.ID = 2;
	two.Icon = new Sprite(PositionX, PositionY, CellSize, CellSize, 1, 1, 0, "../Assessment/Content/projectile_fast_icon.png");
	two.Description = "A quicker projectile that only bounces once.";
	two.Icon->load(renderer);
	mItems.push_back(two);

	Item three;
	three.ID = 3;
	three.Icon = new Sprite(PositionX, PositionY, CellSize, CellSize, 1, 1, 0, "../Assessment/Content/tracks_fast_icon.png");
	three.Description = "Fast tracks, but you can only take one hit.";
	three.Icon->load(renderer);
	mItems.push_back(three);

	Item four;
	four.ID = 4;
	four.Icon = new Sprite(PositionX, PositionY, CellSize, CellSize, 1, 1, 0, "../Assessment/Content/tracks_strong_icon.png");
	four.Description = "Stronger tracks that are slower but you can take an extra hit.";
	four.Icon->load(renderer);
	mItems.push_back(four);

	trackSlot.Count = 1;
	trackSlot.ItemID = 3;

	turretSlot.Count = 1;
	turretSlot.ItemID = 1;

	AddItemToInventory(0, 2, 1);
	AddItemToInventory(1, 4, 1);

	SDL_Log("Inventory loaded with %i capacity", mInventoryItems.capacity());

}

void Inventory::AddItemToInventory(int slot, int ItemID, int count)
{
	if (slot >= 0 && slot < NumRows * NumCols) // In range
	{
		mInventoryItems[slot].Count = count;
		mInventoryItems[slot].ItemID = ItemID;
	}
}

bool Inventory::AddItemAndCount(int ItemID, int count)
{
	int slot = 0;
	
	for (InventoryItem& item : mInventoryItems)
	{
		if (item.ItemID == 0)
		{
			item.ItemID = ItemID;
			item.Count = count;

			return true;
		}
		
		slot++;
	}
	
	return false;
}

Item* Inventory::LookupItemByID(int ID)
{
	for (Item& i : mItems)
	{
		if (i.ID == ID)
			return &i;
	}
	
	return nullptr;
}

void Inventory::EquipItem()
{
	int slot = HighlightRow * NumCols + HighlightCol;
	InventoryItem& item = mInventoryItems[slot];

	
	if (item.ItemID != 0)
	{
		if (item.ItemID == 1 || item.ItemID == 2)
		{
			auto pivot = projectiles[0]->sprite->pivot;
			
			if (item.ItemID == 1)
			{
				for (auto projectile : projectiles)
				{
					if (!projectile->active)
					{
						projectile->loadSprite(renderer, 0, 0, "../Assessment/Content/projectile.png");
						projectile->sprite->pivot = pivot;

						projectile->getComponent<ReflectCComponent>().reflectionMultiplier = 0;
						projectile->getComponent<ReflectCComponent>().reflectionMultiplier = 0;
					}
					else
						projectilesToStillChange.emplace_back(projectile);
				}

				turret->getComponent<ShootProjectileComponent>().speedMultiplier = 0;
			}
			else
			{
				for (auto projectile : projectiles)
				{
					if (!projectile->active)
					{
						projectile->loadSprite(renderer, 0, 0, "../Assessment/Content/projectile_fast.png");
						projectile->sprite->pivot = pivot;

						projectile->getComponent<ReflectCComponent>().reflectionMultiplier = -1;
					}
					else
						projectilesToStillChange.emplace_back(projectile);
				}

				turret->getComponent<ShootProjectileComponent>().speedMultiplier = 2;
			}
			
			auto tempInvId = item.ItemID;
			item.ItemID = turretSlot.ItemID;
			turretSlot.ItemID = tempInvId;
		}
		else
		{
			if (item.ItemID == 3)
			{
				tankBody->getComponent<MoveTankComponent>().speedMultiplier = 0;
				tankBody->getComponent<RevertMovementCComponent>().shotsCanTake = 1;
			}
			else
			{
				tankBody->getComponent<MoveTankComponent>().speedMultiplier = -1;
				tankBody->getComponent<RevertMovementCComponent>().shotsCanTake = 2;
			}
			
			auto tempInvId = item.ItemID;
			item.ItemID = trackSlot.ItemID;
			trackSlot.ItemID = tempInvId;
		}
	}
}

void Inventory::MoveFocus(int key, bool isPressed)
{
	switch (key)
	{
		case SDLK_a: {MOVE_LEFT = isPressed; break; }
		case SDLK_d: {MOVE_RIGHT = isPressed; break; }
		case SDLK_w: {MOVE_UP = isPressed; break; }
		case SDLK_s: {MOVE_DOWN = isPressed; break; }
		case SDLK_RETURN: {RETURN = isPressed; break; }
	}
}

void Inventory::Render(SDL_Renderer* renderer)
{
	invTemplate->render(renderer);

	emptybackground->x = PositionX + 454; // Upper track slot
	emptybackground->y = PositionY + 361;
	emptybackground->render(renderer);

	auto item = LookupItemByID(trackSlot.ItemID);
	item->Icon->x = PositionX + 454;
	item->Icon->y = PositionY + 361;

	item->Icon->render(renderer);
	
	emptybackground->y = PositionY + 511; // Lower track slot
	emptybackground->render(renderer);

	item->Icon->y = PositionY + 511;

	item->Icon->render(renderer);

	emptybackground->x = PositionX + 311; // Turret slot
	emptybackground->y = PositionY + 438;
	emptybackground->render(renderer);

	item = LookupItemByID(turretSlot.ItemID);
	item->Icon->x = PositionX + 311;
	item->Icon->y = PositionY + 438;

	item->Icon->render(renderer);
	
	for (int row = 0; row < NumRows; ++row)
	{
		for (int column = 0; column < NumCols; ++column)
		{
			int slot = row * NumCols + column;

			int x = PositionX + (CellSize + CellSpacing) * column;
			int y = PositionY + (CellSize + CellSpacing) * row;

			if (HighlightRow == row && HighlightCol == column)
			{
				highlightbackground->x = x;
				highlightbackground->y = y;

				highlightbackground->render(renderer);

				if (mInventoryItems[slot].ItemID != 0)
					desc->setText(LookupItemByID(mInventoryItems[slot].ItemID)->Description);
				else
					desc->setText(" ");
			}
			else
			{
				emptybackground->x = x;
				emptybackground->y = y;

				emptybackground->render(renderer);
			}

			if (mInventoryItems[slot].ItemID != 0)
			{
				auto item = LookupItemByID(mInventoryItems[slot].ItemID);
				item->Icon->x = x;
				item->Icon->y = y;

				item->Icon->render(renderer);
			}
		}
	}

	desc->render();
}

void Inventory::Update(bool updateContents)
{
	if (updateContents)
	{
		if (MOVE_RIGHT && HighlightCol < NumCols - 1)
			HighlightCol++;
		else if (MOVE_LEFT && HighlightCol > 0)
			HighlightCol--;
		else if (MOVE_UP && HighlightRow > 0)
			HighlightRow--;
		else if (MOVE_DOWN && HighlightRow < NumRows - 1)
			HighlightRow++;
		else if (RETURN)
			EquipItem();

		MOVE_RIGHT = false;
		MOVE_LEFT = false;
		MOVE_UP = false;
		MOVE_DOWN = false;
		RETURN = false;
	}

	if (!projectilesToStillChange.empty())
	{
		auto pivot = projectiles[0]->sprite->pivot;
		
		for (int p = 0; p < projectilesToStillChange.size(); ++p)
		{
			if (!projectilesToStillChange[p]->active)
			{
				if (turretSlot.ItemID == 1)
				{
					projectilesToStillChange[p]->loadSprite(renderer, 0, 0, "../Assessment/Content/projectile.png");
					projectilesToStillChange[p]->sprite->pivot = pivot;

					projectilesToStillChange[p]->getComponent<ReflectCComponent>().reflectionMultiplier = 0;
				}
				else
				{
					projectilesToStillChange[p]->loadSprite(renderer, 0, 0, "../Assessment/Content/projectile_fast.png");
					projectilesToStillChange[p]->sprite->pivot = pivot;

					projectilesToStillChange[p]->getComponent<ReflectCComponent>().reflectionMultiplier = -1;
				}

				projectilesToStillChange.erase(projectilesToStillChange.begin() + p);
				p--;
			}
		}
	}
}
