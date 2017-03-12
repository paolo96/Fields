#include "header.h"

Charge::Charge(std::string shape, int x, int y, int size, double value){
    this->shape = shape;
    this->x = x;
    this->y = y;
    this->size = size;
    this->value = value;
}

bool Charge::isPointInside(int tx, int ty){}
Charge::~Charge(){}
