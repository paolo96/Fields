#include "header.h"
#include <math.h>

bool Charge::isPointInside(int tx, int ty){
    return ( (pow(x-tx, 2) + pow(y-ty, 2)) <= pow(size, 2) );
}

Charge::Charge(std::string shape, int x, int y, int size, double value){
    this->shape = shape;
    this->x = x;
    this->y = y;
    this->size = size;
    this->value = value;
}

Charge::~Charge(){}
