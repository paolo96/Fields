#include "header.h"

CustomCharge::CustomCharge(int x, int y, int size, double value) : Charge(CUSTOM_SHAPE, x, y, size, value){
    shape_map = std::vector< std::vector<bool> >(size, std::vector<bool>(size));
}
