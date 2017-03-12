#include "header.h"

Button::Button(std::string name, int x, int y, int width, int height, bool checked){
    this->name=name;
    this->x=x;
    this->y=y;
    this->width=width;
    this->height=height;
    this->checked=checked;
}

bool Button::isPointInside(int tx, int ty){
    if( (tx>x) && (ty>y) && (tx<(x+width)) && (ty<(y+height)) )
        return true;
    return false;
}
