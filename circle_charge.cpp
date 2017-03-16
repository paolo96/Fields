#include "header.h"
#include <math.h>

CircleCharge::CircleCharge(int x, int y, int size, double value) : Charge(x, y, size, value){}

bool CircleCharge::isPointInside(int tx, int ty){
    return ( (pow(x-tx, 2) + pow(y-ty, 2)) <= pow(size, 2) );
}

double CircleCharge::potInPoint(int tx, int ty){

	double distance = sqrt( pow(tx-x, 2) + pow(ty-y, 2) );

    return (k0*(value)) / distance;
}

//SFML takes as size radius not diameter in CircleShape constructor
void CircleCharge::drawCharge(sf::RenderWindow& window){

    sf::CircleShape shape(size);
    shape.setFillColor(greyCharge);
    shape.setPosition(x-size , y-size);
    
    window.draw(shape);
}

Vector CircleCharge::elFieldInPoint(int tx, int ty){

	double distance = sqrt( pow(tx-x, 2) + pow(ty-y, 2) );
	double alpha = mainField.angleFromPoints(tx,ty,x,y);
	double intensity = (k0*value)/pow(distance,2);

	return Vector(cos(alpha)*intensity,sin(alpha)*intensity,intensity,alpha);
}

Point CircleCharge::externalPoint(double alpha){
	int tx = round(x+((size+MARGIN_CHARGE_LINES)*cos(alpha)));
	int ty = round(y+((size+MARGIN_CHARGE_LINES)*sin(alpha)));
	return Point(tx, ty, alpha);
}

