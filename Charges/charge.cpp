#include "header.h"
#include <iostream>

bool Charge::isPointInside(int tx, int ty){
    if(shape==CIRCLE_SHAPE){
        if( (tx>=(x-size)) && (ty>=(y-size)) && (tx<=(x+size)) && (ty<=(y+size)) )
            return true;
        return false;
    } else if(shape==SQUARE_SHAPE){
        if( (tx>=(x)) && (ty>=(y)) && (tx<=(x+size)) && (ty<=(y+size)) )
            return true;
        return false;
    }
}

Charge::Charge(std::string shape, int x, int y, int size, double value){
    this->shape = shape;
    this->x = x;
    this->y = y;
    this->size = size;
    this->value = value;
}
