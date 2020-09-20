#include "Item.h"

unsigned int Item::IDValue = 1;	// static value starts at 1.. 	

Item::Item()
{
	ID = Item::IDValue;
	// note that if we are loading Item's from a data file then this ID value will not be useful
	// as we'll load the ID values from the files data
	Item::IDValue++;	// increment the static counter so that the next instance gets a unique ID value
}


Item::~Item()
{
}

// copy constructor
Item::Item(const Item& copyfrom)
{
	this->ID = copyfrom.ID;
	this->CanDrop = copyfrom.CanDrop;
	this->CategoryFlags = copyfrom.CategoryFlags;
	this->Cost = copyfrom.Cost;
	this->Icon = copyfrom.Icon;
	this->InventorySlot = copyfrom.InventorySlot;
	this->IsStackable = copyfrom.IsStackable;
	this->MaxStackSize = copyfrom.MaxStackSize;
	this->Weight = copyfrom.Weight;
	this->Description = copyfrom.Description;
}

// assignment operator
Item& Item::operator=(const Item& copyfrom)
{
	this->ID = copyfrom.ID;
	this->CanDrop = copyfrom.CanDrop;
	this->CategoryFlags = copyfrom.CategoryFlags;
	this->Cost = copyfrom.Cost;
	this->Icon = copyfrom.Icon;
	this->InventorySlot = copyfrom.InventorySlot;
	this->IsStackable = copyfrom.IsStackable;
	this->MaxStackSize = copyfrom.MaxStackSize;
	this->Weight = copyfrom.Weight;
	this->Description = copyfrom.Description;
	return *this;
}