#ifndef LOGIC_h
#define LOGIC_h
#pragma once
#include<iostream>
#include"raylib.h"
#include "Item.h"

class Logic {
public:
    static bool numberInRangeI(int argument, int min, int max);
    
    static float snapGridPoint(float plane, float tileSize);

    //checks collision between two rectangles
    static bool recCollide(Rectangle rec1, Rectangle rec2);

    //checks if mouse is inside a rectangle
    static bool clickRec(int mX, int mY, Rectangle rec);

    static bool XOR(bool a, bool b);

    static void drawWorld(Color bg, Color major, Color minor);

    static int Random(int start, int range);

    static int randomRangeI(int min, int max);

    static float distance(int x1, int y1, int x2, int y2);
};

#endif