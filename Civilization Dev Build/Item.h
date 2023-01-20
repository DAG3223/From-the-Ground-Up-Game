#ifndef ITEM_h
#define ITEM_h
#pragma once

#include<iostream>
#include<list>
#include"raylib.h"

#include "Logic.h"
#include "Block.h"

class Item {
public:
    //Enums
    enum STATES {
        DATA,
        DROPPING,
        GROUND,
        INVENTORY
    };
    enum TYPES {
        NONE,
        MATERIAL,
        WEAPON,
        TOOL,
        ARMOR,
        BLOCK
    };
    enum TOOL_STRENGTH {
        MAKESHIFT,
        IRON,
        BRONZE,
        STEEL
    };
    enum NAMES {
        EMPTY,
        LOG,
        STONE,
        SAND,
        SILICON,
        COPPER_WIRE,
        LIGHTBULB,
        CIRCUIT_BOARD,
        URANIUM_FUEL,
        ORE_COAL,
        ORE_COPPER,
        ORE_TIN,
        ORE_IRON,
        ORE_TUNGSTEN,
        ORE_URANIUM,
        WOOD_BLOCK,
        STONE_BLOCK,
        BRICKS,
        GLASS,
        FLOWER,
        STONE_TOOL,
        IRON_TOOL,
        BRONZE_TOOL,
        STEEL_TOOL,
        WORKBENCH,
        FURNACE,
        ANVIL,
        SIGNALLER,
        BAR_COPPER,
        BAR_TIN,
        BAR_IRON,
        BAR_TUNGSTEN,
        ALLOY_BRONZE,
        ALLOY_FeTung,
        ALLOY_STEEL,
        ACORN
    };
    enum STACK {
        NO,
        YES
    };

    //Display values
    const char* path;
    Texture2D tex;
    const char* name;
    int amount = 0;

    //Locational values
    float x = 0;
    float y = 0;

    //Interaction values
    Rectangle form = { x,y,16,16 };
    int state = DATA;
    int type = NONE;
    float spawnTime = 0.0f;
    int blockID = -1;
    int toolStrength;
    bool stackable;

    //Index in itemData[]
    int ID = 0;

    //Used to create a new item in the world or inventory
    Item(Item dataSource[], int x, int y, int id = 0, int state = DATA, int amount = 1);

    //Used to add an item to the itemData list
    Item(const char* name, const char* path, int type = NONE, int strength = NONE, bool stackable = false, int blockID = -1);

    void updateFromArray(Item array[], int newID, int amount, bool setAmount = false);

    float bob();

    static bool isInventoryFull(Item inventory[]);

    static bool isInventoryEmpty(Item inventory[]);

    static int indexOfItem(Item inventory[], int itemID, int itemAmount = 0, bool checkAmount = true, bool exactAmount = false);

    void changeAmount(int amount, bool set = false);

    void setSlotEmpty(Item array[]);

    void Draw(Item dataSource[], int x, int y, bool dispAmount = true);

    static void DrawID(Item dataSource[], int id, int x, int y);

    static void DrawList(std::list<Item> items, Item dataSource[], Rectangle player, int renderDistance);
};

#endif