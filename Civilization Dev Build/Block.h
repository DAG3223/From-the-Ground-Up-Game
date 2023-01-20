#ifndef BLOCK_h
#define BLOCK_h
#pragma once
#include <iostream>
#include "raylib.h"
#include "Item.h"

class Block {
public:
    enum Collisions {
        UNSOLID,
        SOLID
    };

    enum Toughness {
        ALL,
        IRON,
        BRONZE,
        STEEL
    };

    enum NAMES {
        NONE,
        WOOD,
        STONE,
        BRICKS,
        GLASS,
        FLOWER,
        TREE,
        NODE_COAL,
        NODE_COPPER,
        NODE_TIN,
        NODE_IRON,
        NODE_TUNGSTEN,
        NODE_URANIUM,
        WORKBENCH,
        FURNACE,
        ANVIL,
        SIGNALLER
    };

    const char* name;
    const char* path;

    //locational data
    float x = 0, y = 0;
    Rectangle form = { x,y,25,25 };

    //texture data
    Texture2D tex;

    //state data
    bool solid = false;
    int blockID = 0;
    int guiID = 0;

    int itemID = 0;
    int dropMin = 0;
    int dropMax = 0;
    
    int item2ID = 0;
    int drop2Min = 0;
    int drop2Max = 0;
    
    Vector2 sounds;
    int toughness;
    
    //constructor
    Block(Block dataSource[], float x, float y, int blockID = 0);

    Block(const char* refName, const char* path, Vector2 sounds, bool solid, int toughness, int guiID, Vector3 itemInfo, Vector3 item2Info = {0,0,0});

    static void DrawList(std::list<Block> list, Block dataSource[], Rectangle player, int renderDistance);

    //Draws the block
    void Draw(Block dataSource[]);
};

#endif