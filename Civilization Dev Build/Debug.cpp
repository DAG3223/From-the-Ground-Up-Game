#include<iostream>
#include "raylib.h"
#include "Logic.h"
#include "Item.h"
#include "Debug.h"

void Debug::startEvent(const char* name, bool perform) {
    if (perform) {
        std::cout << std::endl;
        std::cout << "*------------------------------------------*" << std::endl;
        std::cout << "       " << name << " EVENT START" << std::endl;
        std::cout << "*------------------------------------------*" << std::endl;
        std::cout << std::endl;
    }  
}

void Debug::endEvent(const char* name, bool perform) {
    if (perform) {
        std::cout << std::endl;
        std::cout << "*------------------------------------------*" << std::endl;
        std::cout << "       " << name << " EVENT END" << std::endl;
        std::cout << "*------------------------------------------*" << std::endl;
        std::cout << std::endl;
    }
}

void Debug::Log(const char* text, bool perform) {
    if (perform) {
        std::cout << "[DEBUG]: " << text << std::endl;
    }
}

void Debug::displayInt(const char* text, int value, bool perform) {
    if (perform) {
        std::cout << "[DEBUG]: " << text << value << "" << std::endl;
    }
}

void Debug::displayFloat(const char* text, float value, bool perform) {
    if (perform) {
        std::cout << "[DEBUG]: " << text << value << "" << std::endl;
    }
}

void Debug::displayChar(const char* text, const char* value, bool perform) {
    if (perform) {
        std::cout << "[DEBUG]: " << text << value << "" << std::endl;
    }
}

void Debug::displayBool(const char* text, bool value, bool perform) {
    if (perform) {
        if (value) {
            std::cout << "[DEBUG]: " << text << "true" << "" << std::endl;
        }
        else {
            std::cout << "[DEBUG]: " << text << "false" << "" << std::endl;
        }
    }
}

void Debug::blankLine(bool perform) {
    if (perform) {
        std::cout << std::endl;
    }
}