#include "header.h"

Point::Point(int x, int  y){
    this->x=x;
    this->y=y;
}

Point::Point(int x, int  y, double value){
	this->x=x;
	this->y=y;
	this->value=value;
}

//Used by std::sort in Field::findEquipotentialLines()
bool Point::operator<(const Point& temp) const{
	return (value < temp.value);
}

bool Point::operator==(const Point& temp) const{
        return ((x == temp.x) && (y == temp.y));
}
