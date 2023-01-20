#include <iostream>
#include "raylib.h"
#include "Logic.h"
#include "Item.h"
#include "Block.h"
#include "Recipe.h"
#include "Debug.h"
#include "GUI.h"

//Logic Methods
int GUI::shiftByStation(Recipe dataSource[], int sourceSize, int currentIndex, int stationID, int direction) {
    if (direction == GUI::SHIFT::RIGHT) {
        return GUI::advanceByStation(dataSource, sourceSize, currentIndex, stationID);
    }
    else if (direction == GUI::SHIFT::LEFT) {
        return GUI::retreatByStation(dataSource, sourceSize, currentIndex, stationID);
    }
    return currentIndex;
}

int GUI::advanceByStation(Recipe dataSource[], int sourceSize, int currentIndex, int stationID) {
    for (int i = currentIndex + 1; i < sourceSize; i++) {
        if (dataSource[i].stationID == stationID) {
            return i;
        }
    }
    for (int i = 0; i < currentIndex; i++) {
        if (dataSource[i].stationID == stationID) {
            return i;
        }
    }
    return currentIndex;
}

int GUI::retreatByStation(Recipe dataSource[], int sourceSize, int currentIndex, int stationID) { 
    for (int i = (currentIndex - 1); i > -1; i--) {
        if (dataSource[i].stationID == stationID) {
            return i;
        }
    }
    for (int i = sourceSize; i > currentIndex; i--) {
        if (dataSource[i].stationID == stationID) {
            return i;
        }
    }
    return currentIndex;
}

int GUI::firstInStation(Recipe dataSource[], int sourceSize, int stationID) {
    for (int i = 0; i < sourceSize; i++) {
        if (dataSource[i].stationID == stationID) return i;       
    }
    return -1;
}

bool GUI::isRecipeAvailable(Recipe dataSource[], Item inventory[], int recipeID) {
    bool p1Found, p2Found;
    p1Found = (dataSource[recipeID].part1Data.x == 0 || dataSource[recipeID].part1Data.y == 0);
    p2Found = (dataSource[recipeID].part2Data.x == 0 || dataSource[recipeID].part2Data.y == 0);

    if (!Item::isInventoryEmpty(inventory)) {
        for (int i = 0; i < 30; i++) {
            if (!p1Found) {
                if (dataSource[recipeID].part1Data.x == inventory[i].ID) {
                    if (dataSource[recipeID].part1Data.y <= inventory[i].amount) {
                        p1Found = true;
                    }
                }

            }
            if (!p2Found) {
                if (dataSource[recipeID].part2Data.x == inventory[i].ID) {
                    if (dataSource[recipeID].part2Data.y <= inventory[i].amount) {
                        p2Found = true;
                    }
                }
            }
            if (p1Found && p2Found) {
                break;
            }
        }
    }

    return (p1Found && p2Found);
}

//Display Methods
void GUI::drawStation(Recipe recipeData[], Item itemData[], Item inventory[], int recipeID, bool recipeAvailable, Texture2D available, Texture2D notAvailable) {
    Texture2D* availableTex;
    if (recipeAvailable) {
        availableTex = &available;
    }
    else {
        availableTex = &notAvailable;
    }

    DrawRectangle(250, 250, 300, 300, GRAY);
    recipeData[recipeID].Draw(recipeData, recipeID, itemData, 300, 300);
        
    DrawTexture(*availableTex, 405, 300, RAYWHITE);
}
