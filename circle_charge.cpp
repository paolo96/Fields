#include "header.h"
#include <math.h>

CircleCharge::CircleCharge(int x, int y, int size, double value) : Charge(CIRCLE_SHAPE, x, y, size, value){}

bool CircleCharge::isPointInside(int tx, int ty){
    return ( (pow(x-tx, 2) + pow(y-ty, 2)) <= pow(size, 2) );
}
