#include<iostream>
#include<list>
#include"raylib.h"

#include "Logic.h"
#include "Item.h"
#include "Block.h"
#include "Debug.h"

//Used to create a new item in the world or inventory
Item::Item(Item dataSource[], int x, int y, int id, int state, int amount) {
    this->ID = id;

    this->name = dataSource[ID].name;
    this->amount = amount;

    this->form = { (float)x, (float)y,16,16 };
    this->x = x;
    this->y = y;

    this->spawnTime = (float)GetTime();
    this->state = state;
    this->type = dataSource[ID].type;
    this->toolStrength = dataSource[ID].toolStrength;
    this->stackable = dataSource[ID].stackable;
}

//Used to add an item to the itemData list
Item::Item(const char* name, const char* path, int type, int strength, bool stackable, int blockID) {
    this->name = name;
    this->path = path;
    this->type = type;
    this->state = DATA;
    this->toolStrength = strength;
    this->stackable = stackable;
    this->blockID = blockID;
}

void Item::updateFromArray(Item array[], int newID, int amount, bool setAmount) {
    this->ID = newID;

    this->name = array[ID].name;
    this->amount = amount + this->amount*(!setAmount);

    this->state = state;
    this->type = array[ID].type;
    this->blockID = array[ID].blockID;
}

float Item::bob() {
    return (-4 * cos(4 * (GetTime() - spawnTime)) + 4);
}

bool Item::isInventoryFull(Item inventory[]) {
    for (int i = 0; i < 30; i++) {
        if (inventory[i].ID == 0) {
            return false;
        }
    }
    return true;
}

bool Item::isInventoryEmpty(Item inventory[]) {
    for (int i = 0; i < 30; i++) {
        if (inventory[i].ID != 0) {
            return false;
        }
    }
    return true;
}

void Item::changeAmount(int amount, bool set) {
    if (set) {
        this->amount = amount;
    }
    else {
        this->amount += amount;
    }
}

void Item::setSlotEmpty(Item array[]) {
    updateFromArray(array, 0, 0, true);
}

//finds requested item's index in the inventory
int Item::indexOfItem(Item inventory[], int itemID, int itemAmount, bool checkAmount, bool exactAmount) {
    for (int i = 0; i < 30; i++) {
        //if ID's do not match, skip
        if (inventory[i].ID != itemID) {
            continue;
        }
        
        //if amount is nonspecific, return index
        if (!checkAmount) {
            return i;
        }

        //if exact amount is unneeded, return index of first greater/equal amount
        if (!exactAmount) {
            if (inventory[i].amount >= itemAmount) {
                return i;
            }
        }

        //return index of first equal amounts
        if (inventory[i].amount == itemAmount) {
            return i;
        }
    }
    return -1;
}

void Item::Draw(Item dataSource[], int x, int y, bool dispAmount) {
    DrawTexture(dataSource[this->ID].tex, x, y + (state == GROUND) * bob(), RAYWHITE);
    if (dispAmount && amount != 0) DrawText(TextFormat("%i", amount), x, y + (state == GROUND) * bob(), 10, YELLOW);
}

void Item::DrawID(Item dataSource[], int id, int x, int y) {
    DrawTexture(dataSource[id].tex, x, y, RAYWHITE);
}

void Item::DrawList(std::list<Item> items, Item dataSource[], Rectangle player, int renderDistance) {
    std::list<Item>::iterator itemsDraw;
    for (itemsDraw = items.begin(); itemsDraw != items.end(); itemsDraw++)
    {
        if (Logic::distance(player.x + 25, player.y + 25, itemsDraw->x, itemsDraw->y) > renderDistance) {
            continue;
        }
        itemsDraw->Draw(dataSource, itemsDraw->x, itemsDraw->y, true);
    }
}