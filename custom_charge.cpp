#include "header.h"
#include <math.h>

CustomCharge::CustomCharge(int x, int y, int size, double value) : Charge(x, y, size, value){}

bool CustomCharge::isPointInside(int tx, int ty){
    for(int i=0; i<shape_map.size(); i++)
        if(((shape_map[i].x+x) == tx) && ((shape_map[i].y+y) == ty))
            return true;
    return false;
}

//The potential value is calculated as if the custom charge was composed of circle charge of size 1
double CustomCharge::potInPoint(int tx, int ty){

	double totPotValue = 0;
	double distance;
	double dividedValue = value / shape_map.size();

    for(int i=0; i<shape_map.size(); i++){

        distance = sqrt( pow(tx-(shape_map[i].x+x), 2) + pow(ty-(shape_map[i].y+y), 2) );
        totPotValue += (k0*(dividedValue)) / distance;
    }

	return totPotValue;
}

void CustomCharge::drawCharge(sf::RenderWindow& window){

    std::vector<sf::Vertex> shapePixels;

    for(int i=0; i<shape_map.size(); i++){
		sf::Vertex pixel(sf::Vector2f(shape_map[i].x+x, shape_map[i].y+y), greyCharge);
		shapePixels.push_back(pixel);
    }

	window.draw(&shapePixels[0], shapePixels.size(), sf::Points);
}

Vector CustomCharge::elFieldInPoint(int tx, int ty){
	double distance, alpha;
	double totElFieldValueX = 0, totElFieldValueY = 0;
	double dividedValue = value / shape_map.size();

	for(int i=0; i<shape_map.size(); i++){

        distance = sqrt( pow(tx-(shape_map[i].x+x), 2) + pow(ty-(shape_map[i].y+y), 2) );
		alpha = mainField.angleFromPoints(tx,ty,shape_map[i].x+x,shape_map[i].y+y);
        totElFieldValueX += cos(alpha)*((k0*dividedValue)/pow(distance,2));
        totElFieldValueY += sin(alpha)*((k0*dividedValue)/pow(distance,2));
    }

	return Vector(totElFieldValueX,totElFieldValueY,0,0);
}

//Finds the external point with closer alpha direction
Point CustomCharge::externalPoint(double alpha){

	if (external_points.size()==0 && shape_map.size()!=0)
		findExternalPoints();

	double deltaMin=abs(mainField.angleFromPoints(external_points[0].x,external_points[0].y,size/2,size/2)-alpha);
	int minPoint = 0;
	for(int i=1; i<external_points.size(); i++){
		double alphaPoint = mainField.angleFromPoints(external_points[i].x,external_points[i].y,size/2,size/2);
		if(abs(alphaPoint-alpha)<deltaMin){
			deltaMin=abs(alphaPoint-alpha);
			minPoint=i;
		}
	}
	return Point(external_points[minPoint].x+x, external_points[minPoint].y+y, alpha);
}

//Finds the external points of the shape
void CustomCharge::findExternalPoints(){

	std::vector< std::vector<bool> > shape_map_xy(size, std::vector<bool>(size));

	for(int i=0; i<shape_map.size(); i++)
		shape_map_xy[shape_map[i].x][shape_map[i].y]=true;

	bool firstFound = false;
	bool lastFound = false;
	int xlast, ylast;

	for(int x=0; x<shape_map_xy.size(); x++){
		for(int y=0; y<shape_map_xy[x].size(); y++){
			if(shape_map_xy[x][y]){
				if(!firstFound){
					firstFound=true;
					external_points.push_back(Point(x, y));
					continue;
				}
				xlast=x;
				ylast=y;
				lastFound=true;
			}
			
		}
		firstFound=false;
		if(lastFound){
			external_points.push_back(Point(xlast, ylast));
			lastFound=false;
		}
	}


	for(int y=0; x<shape_map_xy.size(); y++){
		for(int x=0; y<shape_map_xy[x].size(); x++){
			if(shape_map_xy[x][y]){
				if(!firstFound){
					firstFound=true;
					external_points.push_back(Point(x, y));
						
					continue;
				}
				xlast=x;
				ylast=y;
				lastFound=true;
			}
			
		}
		firstFound=false;
		if(lastFound){
			external_points.push_back(Point(xlast, ylast));
			lastFound=false;
		}
	}
}
