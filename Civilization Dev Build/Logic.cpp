#include<iostream>
#include"raylib.h"
#include"Logic.h"
#include "Item.h"

bool Logic::numberInRangeI(int argument, int min, int max) {
    return (argument >= min && argument <= max);
}

float Logic::snapGridPoint(float plane, float tileSize) {
    return roundf((plane) / tileSize) * tileSize;
}

//checks collision between two rectangles
bool Logic::recCollide(Rectangle rec1, Rectangle rec2) {
    return rec1.x < rec2.x + rec2.width && rec1.x + rec1.width > rec2.x && rec1.y < rec2.y + rec2.height && rec1.y + rec1.height > rec2.y;
}

//checks if mouse is inside a rectangle
bool Logic::clickRec(int mX, int mY, Rectangle rec) {
    return ((mX >= rec.x && mX <= rec.x + rec.width) && (mY >= rec.y && mY <= rec.y + rec.height));
}

bool Logic::XOR(bool a, bool b) {
    return (((!a) && b) || ((!b) && a));
}

void Logic::drawWorld(Color bg, Color major, Color minor) {
    ClearBackground(bg);
    DrawRectangle(-800, -800, 2400, 2400, major);
    DrawRectangle(0, -800, 800, 800, minor);
    DrawRectangle(-800, 0, 800, 800, minor);
    DrawRectangle(800, 0, 800, 800, minor);
    DrawRectangle(0, 800, 800, 800, minor);
}

int Logic::Random(int start, int range) {
    return rand() % start + range;
}

int Logic::randomRangeI(int min, int max) {
    return rand() % min + (max-min+1);
}

float Logic::distance(int x1, int y1, int x2, int y2) {
    return (float)sqrt(((x2 - x1)*(x2 - x1)) + ((y2 - y1) * (y2 - y1)));
}

