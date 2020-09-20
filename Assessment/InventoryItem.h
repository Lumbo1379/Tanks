#pragma once

// simple struct that holds a count and an identifier of what item it represents (ItemID), which can then be looked-up in the ItemDatabase to get
// info on it for display, cost etc.. Note: this class is header only as it only stores data and is used in another data structure + container class
struct InventoryItem
{
	unsigned int Count = 0;
	unsigned int ItemID = 0; // note: lets use the convention that 0 == NO ITEM DEFINED!
};
