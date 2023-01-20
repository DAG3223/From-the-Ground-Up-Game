#ifndef GUI_h
#define GUI_h
#pragma once
#include <iostream>
#include "raylib.h"
#include "Item.h"

class GUI {
public:
    enum NAMES {
        NONE,
        WORKBENCH,
        FURNACE,
        ANVIL,
        SIGNALLER
    };

    enum SHIFT {
        RIGHT,
        LEFT
    };

    //Logic Methods
    static int shiftByStation(Recipe dataSource[], int sourceSize, int currentIndex, int stationID, int direction);
    
    static int advanceByStation(Recipe dataSource[], int sourceSize, int currentIndex, int stationID);

    static int retreatByStation(Recipe dataSource[], int sourceSize, int currentIndex, int stationID);

    static int firstInStation(Recipe dataSource[], int sourceSize, int stationID);

    static bool isRecipeAvailable(Recipe dataSource[], Item inventory[], int recipeID);

    //Display Methods
    static void drawStation(Recipe recipeData[], Item itemData[], Item inventory[], int recipeID, bool recipeAvailable, Texture2D available, Texture2D notAvailable);
};
#endif