#pragma once
#include <string>
#include "Sprite.h"

// you can define more bits here up to max of your long integer bit size, depends on platform, but could be 64 bits or 16 (0-15) etc
// its usually good to define a "terminal" bit that allows for you to know what the current maximum is

enum Item_Category
{
	Category_None = 0,
	Category_Food = 1 << 0,
	Category_Item = 1 << 1,
	Category_Clothing = 1 << 2,
	Category_Prop = 1 << 3,
	Category_Resource = 1 << 4,
	Category_Consumable = 1 << 5,
	Category_Structure = 1 << 6,
	Category_Last_Do_Not_Use = 1 << 15
};

// "slots" might be used to denote special limitations for items in a particular part of an inventory
// for instance if you have an RPG, you might use a slot restriction to only allow some items to be equipped in specific
// parts of the Inventory
enum Inventory_Slot
{
	Slot_None = 0,
	Slot_Head,
	Slot_Chest,
	Slot_Hands,
	Slot_Feet,
	Slot_Pants,
	Slot_Face,
	Slot_Last
};


/// Item Class
// simple class that holds data values for an individual "Item". This is essentially a data-only class (i.e. no code), so it could have also
// been created as a struct. Note: You can basically add whatever data you need to this class. Things like which image to use, cost, weight etc.
// I've added a bunch of things I've found useful in the past, but you should delete or add whatever you need for your own inventory here

class Item
{
public:
	Item();
	~Item();
	Item(const Item& copyfrom);	// copy constructor
	Item& operator=(const Item& copyfrom); // assignment operator

	unsigned int ID = 0;	// lets assume that 0 = "NOTHING"
	unsigned short int CategoryFlags = 0;	// 16 bit-flags we can use to filter what type of object this item is (Tool, Clothing, Prop etc)
	unsigned int InventorySlot = 0;			// what "slot" should this item usually go in, for a slot-based inventory (head, chest, arms, feet etc)
	std::string Description = "";				// proper description
	unsigned int Cost = 0;					// base cost of the item
	float Weight = 1.0f;					// weight
	bool IsStackable = false;				// stackable? if so.. well stacking in inventories requires logic :)
	unsigned int MaxStackSize = 1;			// if it is stackable, different items might have different stack sizes
	bool CanDrop = true;					// can it be dropped? (might want to add an vector offset for where it gets dropped relative to character)
	Sprite* Icon;					// path or image name for use as an icon in an inventory

	// need some way of describing what item gets created if we drop this thing into the world..?
	// etc... whatever data you want

private:
	static unsigned int IDValue; // static variable we increment to ensure unique ID value
};
