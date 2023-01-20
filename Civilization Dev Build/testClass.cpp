#include<iostream>
#include"testClass.h"

testClass::testClass(int setValue) {
    this->testValue = setValue;
}

int testClass::myValue() {
    return testValue;
}