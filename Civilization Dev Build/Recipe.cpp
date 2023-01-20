#include <iostream>
#include "raylib.h"
#include "Logic.h"
#include "Item.h"
#include "Block.h"
#include "Recipe.h"

//Constructor
Recipe::Recipe(const char* name, Vector2 outputData, Vector2 part1Data, Vector2 part2Data, int station) {
    this->name = name;
    
    this->outputData = outputData;
    this->part1Data = part1Data;
    this->part2Data = part2Data;

    this->stationID = station;
}

//Display Methods
void Recipe::Draw(Recipe recipeData[], int recipeID, Item itemDataSource[], int x, int y) {
    Vector2* itemData = &outputData;
    int xOffset;

    DrawText(recipeData[recipeID].name, x, y - 15, 10, BLACK);
    DrawRectangle(x, y, 100, 25, DARKGRAY);
    
    //Draw Ingredients
    for (int i = 0; i < 3; i++) {
        switch (i) {
        case 0:
            itemData = &outputData;
            break;
        case 1:
            itemData = &part1Data;
            break;
        case 2:
            itemData = &part2Data;
            break;
        }
        
        if (itemData->x == Item::NAMES::EMPTY) {
            continue;
        }
        if (itemData->y < 1) {
            continue;
        }
        xOffset = (2 * (i * i)) + (18 * i) + 4;

        Item::DrawID(itemDataSource, itemData->x, x + xOffset, y + 4);
        DrawText(TextFormat("%i", (int)itemData->y), x + xOffset, y + 4, 10, YELLOW);
        
        DrawText("Needs:", x, y+25, 15, BLACK);
        if (i == 1) {
            Item::DrawID(itemDataSource, itemData->x, x, y + 40);
            DrawText(itemDataSource[(int)itemData->x].name, x + 20, y + 40, 10, BLACK);
        }
        else if (i == 2) {
            Item::DrawID(itemDataSource, itemData->x, x, y + 60);
            DrawText(itemDataSource[(int)itemData->x].name, x + 20, y + 60, 10, BLACK);
        }
        
    }

    //Write out Recipe
}