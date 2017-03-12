#include "header.h"

SquareCharge::SquareCharge(int x, int y, int size, double value) : Charge(SQUARE_SHAPE, x, y, size, value){}

bool SquareCharge::isPointInside(int tx, int ty){
	return (tx>=(x)) && (ty>=(y)) && (tx<=(x+size)) && (ty<=(y+size));
}
