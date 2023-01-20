#ifndef DEBUG_h
#define DEBUG_h
#pragma once
#include<iostream>
#include"raylib.h"
#include "Item.h"

class Debug {
public:
    static void startEvent(const char* name, bool perform = true);

    static void endEvent(const char* name, bool perform = true);

    static void Log(const char* text, bool perform = true);

    static void displayInt(const char* text, int value, bool perform = true);

    static void displayFloat(const char* text, float value, bool perform = true);

    static void displayChar(const char* text, const char* value, bool perform = true);

    static void displayBool(const char* text, bool value, bool perform = true);

    static void blankLine(bool perform = true);
};

#endif
