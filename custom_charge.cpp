#include "header.h"

CustomCharge::CustomCharge(int x, int y, int size, double value) : Charge(CUSTOM_SHAPE, x, y, size, value){}

bool CustomCharge::isPointInside(int tx, int ty){
    for(int i=0; i<shape_map.size(); i++)
        if(((shape_map[i].x+x) == tx) && ((shape_map[i].y+y) == ty))
            return true;
    return false;
}
