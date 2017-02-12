#include "header.h"

Charge::Charge(int x, int y, int size, double value){
	this->x = x;
	this->y = y;
	this->size = size;
	this->value = value;
}


bool Charge::isPointInside(int tx, int ty){
    if( (tx>=(x-size)) && (ty>=(y-size)) && (tx<=(x+size)) && (ty<=(y+size)) )
        return true;
    return false;
}
