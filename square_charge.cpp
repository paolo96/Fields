#include "header.h"
#include <iostream>

SquareCharge::SquareCharge(int x, int y, int size, double value) : Charge(SQUARE_SHAPE, x, y, size, value){}

bool SquareCharge::isPointInside(int tx, int ty){
    if( (tx>=(x)) && (ty>=(y)) && (tx<=(x+size)) && (ty<=(y+size)) )
        return true;
    return false;
}
