#ifndef RECIPE_h
#define RECIPE_h
#pragma once
#include <iostream>
#include "raylib.h"
#include "Item.h"

class Recipe {
public:
    enum RECIPES {
        WORKBENCH_BLOCK_WOOD,
        WORKBENCH_BLOCK_STONE,
        WORKBENCH_BLOCK_BRICK,
        WORKBENCH_FURNACE,
        WORKBENCH_ANVIL,
        WORKBENCH_LIGHT,
        WORKBENCH_CIRCUIT,

        FURNACE_GLASS,
        FURNACE_SILICON,
        FURNACE_BAR_COPPER,
        FURNACE_BAR_TIN,
        FURNACE_BAR_IRON,
        FURNACE_BAR_TUNGSTEN,
        FURNACE_ALLOY_BRONZE,
        FURNACE_ALLOY_FERROTUNGSTEN,
        FURNACE_ALLOY_STEEL,

        ANVIL_SAND,
        ANVIL_WIRE,
        ANVIL_TOOL_IRON,
        ANVIL_TOOL_BRONZE,
        ANVIL_TOOL_STEEL
    };
    
    //Logic Variables
    const char* name;

    Vector2 part1Data;
    Vector2 part2Data;
    Vector2 outputData;

    /*int part1ID, part1Amount;
    int part2ID, part2Amount;
    int outputID, outputAmount;*/

    int stationID;

    //Constructor
    Recipe(const char* name, Vector2 outputData, Vector2 part1Data, Vector2 part2Data, int station);

    //Display Method
    void Draw(Recipe recipeData[], int recipeID, Item itemDataSource[], int x, int y);
};
#endif