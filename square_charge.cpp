#include "header.h"
#include <math.h>
#include <iostream>

SquareCharge::SquareCharge(int x, int y, int size, double value) : Charge(x, y, size, value){}

bool SquareCharge::isPointInside(int tx, int ty){
	return (tx>=(x)) && (ty>=(y)) && (tx<=(x+size)) && (ty<=(y+size));
}

//The potential value is calculated as if the square charge was composed of circle charge of size 1
double SquareCharge::potInPoint(int tx, int ty){

	double totPotValue = 0;
	double distance;
	double dividedValue = value / (size*size);

	for(int X=x; X<=(x+size); X++){
        for(int Y=y; Y<=(y+size); Y++){

            distance = sqrt( pow(tx-X, 2) + pow(ty-Y, 2) );
            totPotValue += (k0*(dividedValue)) / distance;
        }
    }

	return totPotValue;
}

void SquareCharge::drawCharge(sf::RenderWindow& window){

    sf::RectangleShape shape(sf::Vector2f(size, size));
    shape.setFillColor(greyCharge);
    shape.setPosition(x, y);
    
    window.draw(shape);
}

Vector SquareCharge::elFieldInPoint(int tx, int ty){

	double distance, alpha;
	double totElFieldValueX = 0, totElFieldValueY = 0;
	double dividedValue = value / (size*size);

	for(int X=x; X<=(x+size); X++){
        for(int Y=y; Y<=(y+size); Y++){

            distance = sqrt( pow(tx-X, 2) + pow(ty-Y, 2) );
			alpha = mainField.angleFromPoints(tx,ty,X,Y);
            totElFieldValueX += cos(alpha)*((k0*dividedValue)/pow(distance,2));
            totElFieldValueY += sin(alpha)*((k0*dividedValue)/pow(distance,2));
        }
    }

	return Vector(totElFieldValueX,totElFieldValueY,0,0);
}

Point SquareCharge::externalPoint(double alpha){
	const int SPACE = MARGIN_CHARGE_LINES+size;
	int tx = x-MARGIN_CHARGE_LINES/2;
	int ty = y-MARGIN_CHARGE_LINES/2;
	if (alpha==PI/2){
		tx = tx+SPACE/2;
		ty = ty;
	} else if (alpha==(3*PI/2)){
		tx = tx+SPACE/2;
		ty = ty+SPACE;
	} else if( (alpha>=0 && alpha<PI/4) || (alpha>=(PI*7/4) && alpha<2*PI) ){
		tx = tx+SPACE;
		ty = ty+( (SPACE/2)*(1-tan(alpha)) );
	} else if ( alpha>=PI/4 && alpha<(3*PI/4) ) {
		tx = tx+( (SPACE/2)*(1+(1/tan(alpha))) );
		ty = ty;
	} else if ( alpha>=(3*PI/4) && alpha<(5*PI/4) ) {
		tx = tx;
		ty = ty+( (SPACE/2)*(1+tan(alpha)) );
	} else if ( alpha>=(5*PI/4) && alpha<(7*PI/4) ) {
		tx = tx+( (SPACE/2)*(1-(1/tan(alpha))) );
		ty = ty+SPACE;
	}
	tx = round(tx);
	ty = round(ty);

	return Point(tx+1, ty+1, alpha);
}
