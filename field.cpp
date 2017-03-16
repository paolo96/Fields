#include "header.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <memory>

//Fills Field's arrays with needed values of current modality
//It's called when you add or remove a charge and when you switch modality
void Field::update(){
	if(isEqPotLines) {
		mapPotential();
		std::cout<<"Potential mapped"<<std::endl;
		findEquipotentialPoints();
		std::cout<<"Found " << eqPotPoints.size()  << " equipotential points"<<std::endl;
		findEquipotentialLines();
		std::cout<<"Found " << eqPotLines.size() << " equipotential lines"<<std::endl;
	} else if(isElFieldLines){
		findElFieldLines();
		std::cout<<"Found " << elFieldLines.size() << " electric field lines"<<std::endl;
	} else if(isElFieldColor){
		mapElFieldIntensity();
		std::cout<<"Field Intensity mapped"<<std::endl;
	}

}

//Sort vector eqPotPoints by potential intensity and fill vector eqPotLines with Lines of Points at same potential intensity
void Field::findEquipotentialLines(){

	std::sort(eqPotPoints.begin(), eqPotPoints.end());
    
    double delta;
    for(int i=0; i<eqPotPoints.size(); i++){
        Line temp;
        delta = abs( int(eqPotPoints[i].value/linesDeepReduction) );
        for(int y=i+1; y<eqPotPoints.size(); y++){
			if( (eqPotPoints[i].value+delta)>=(eqPotPoints[y].value-delta) && (eqPotPoints[i].value-delta)<=(eqPotPoints[y].value+delta) ){
				temp.pointsCoord.push_back(eqPotPoints[y]);
            } else {
				eqPotLines.push_back(temp);
				i=y-1;
				break;
			}
		}
    }
}

//Fills vector eqPotPoints with Points at same potential
void Field::findEquipotentialPoints(){

	double delta;
    for (int x=0; x<windowWidth; x++){
        for (int y=0; y<windowHeight; y++){

            delta = abs( int(potIntensityMap[x][y]/linesDeepReduction) );
            Point point = Point(x, y, potIntensityMap[x][y]);
			
			for (int X=x; X<windowWidth; X++){
				for (int Y=y+1; Y<windowHeight; Y++){
					if( (potIntensityMap[x][y]+delta)>=(potIntensityMap[X][Y]-delta) && (potIntensityMap[x][y]-delta)<=(potIntensityMap[X][Y]+delta) ) {
						
						eqPotPoints.push_back(point);
						//Using goto because in C++ doesn't exist "break" for nested loops
						goto end;
					}
				}
                    
			}
		
			end:;
		}
	}
}

//Fills vector potIntensityMap with potential value of every visible point of the window
void Field::mapPotential(){

	bool isPointInsideCharge=false;
	bool isPointInsideButtons=false;

	for (int x=0; x<windowWidth; x++){
		for (int y=0; y<windowHeight; y++){
			isPointInsideCharge=false;
            for(int i=0; i<qCharges.size(); i++){
                    isPointInsideCharge = qCharges[i]->isPointInside(x, y);
                    if(isPointInsideCharge)
                        break;
            }
			for(int i=0; i<buttons.size(); i++){
                    isPointInsideButtons = buttons[i].isPointInside(x, y);
                    if(isPointInsideButtons)
                        break;
            }
            if(!isPointInsideCharge && !isPointInsideButtons){
				potIntensityMap[x][y] = potValueInPoint(x,y);
			}
		}
	}
}

//Returns potential in the point (x,y) given by all charges in the field
double Field::potValueInPoint(int x, int y){

	double totPotValue = 0;
    
	for(int i=0; i<qCharges.size(); i++)
		totPotValue += qCharges[i]->potInPoint(x, y);

	return totPotValue;
}

//Fills vector elFieldLines
void Field::findElFieldLines(){
	double totChargeAbs = 0;
	
	for(int i=0; i<qCharges.size(); i++)
		totChargeAbs += abs(qCharges[i]->value);

	for(int i=0; i<qCharges.size(); i++){
		double propFactor = totChargeAbs/abs(qCharges[i]->value);
		for(double alpha=0; alpha<PI*2; alpha+=(((PI*2)/linesPerCharge)*propFactor)){
			elFieldLines.push_back(buildElFieldLine(qCharges[i]->externalPoint(alpha), i));
		}
	}
}

//Builds one electric field line starting from startPoint with direction startPoint.value
Line Field::buildElFieldLine(Point startPoint, int startCharge){

	Line temp;
	temp.pointsCoord.push_back(startPoint);

	double tx = startPoint.x;
	double ty = startPoint.y;
	double talpha = startPoint.value;

	for(int i=0; tx<windowWidth && tx>0 && ty<windowHeight && ty>0 && !insideCharges(tx, ty, startCharge); i++ ){
		tx+=cos(talpha);
		ty+=sin(talpha);
		talpha = elFieldValueInPoint(round(tx), round(ty)).alpha;
		
		if(i%arrowsReduction==0 && i!=0)
			arrows.push_back(buildElFieldArrowHead(tx, ty, talpha));

		if(qCharges[startCharge]->value<0){
			talpha-=PI;
			if(talpha<0)
				talpha+=2*PI;
		}

		Point loopPoint(tx, ty, talpha);
		temp.pointsCoord.push_back(loopPoint);

		if(i==EL_FIELD_LINE_LENGHT_LIMIT){
			temp.pointsCoord.clear();
			break;
		}
	}
	
	return temp;
}

//Returns Field intensity and direction in (x, y)
//Direction=0 mean field with right direction, direction=PI mean field left directed
Vector Field::elFieldValueInPoint(int x, int y){
	
	double totFieldValueX = 0, totFieldValueY = 0;

	for(int i=0; i<qCharges.size(); i++){

		Vector elFieldByOneCharge = qCharges[i]->elFieldInPoint(x, y);
		totFieldValueX += elFieldByOneCharge.x;
		totFieldValueY += elFieldByOneCharge.y;
	}

	Vector v( x, y, sqrt(pow(totFieldValueX, 2) + pow(totFieldValueY, 2)), angleFromPoints(totFieldValueX, totFieldValueY,0,0) );
	return v;
}

//Returns an angle between the rects:
//1. Passing through (x1, y1) and (x2, y2)
//2. Horizontal directed
//The angle is measured counterclock wise
double Field::angleFromPoints(double x1, double y1, double x2, double y2){
	double alpha=0;
	if(x1!=x2){
		alpha = atan((y1-y2)/(x1-x2));
		if(y1<y2 && x1>x2)
			alpha = abs(alpha);
		else if (y1<y2 && x1<x2)
			alpha = PI - alpha;
		else if (y1>y2 && x1<x2)
			alpha = PI + abs(alpha);
		else if (y1>y2 && x1>x2)
			alpha = 2*PI - alpha;
	} else if(y1<y2){
		alpha = PI/2;	
	} else if(y1>y2){
		alpha = (3*PI)/2;
	}
	if(y1==y2 && x1<x2)
		alpha = PI;
	return alpha;
}

//Draws arrow head: a little triangle rotated and positioned on the electic field line
//Triangle is isosceles with vertex_angle=arrowAngle/2 and side=arrowSize
//Arrows Head is being drawn here and not in draws.cpp because I want to do the rotation and translation only when updated and not every FPS
std::vector<sf::Vertex> Field::buildElFieldArrowHead(double tx, double ty, double talpha){
	std::vector<sf::Vertex> temp;

	double px = -(cos(arrowAngle)*arrowSize);
	double py = sin(arrowAngle)*arrowSize;
	double px2 = px;
	double py2 = -py;

	//Rotation equations
	double rotPX = px*cos(talpha)-py*sin(talpha);
	double rotPY = px*sin(talpha)+py*cos(talpha);
	double rotPX2 = px2*cos(talpha)-py2*sin(talpha);
	double rotPY2 = px2*sin(talpha)+py2*cos(talpha);

	temp.push_back(sf::Vertex(sf::Vector2f(tx, ty), sf::Color::White));
	temp.push_back(sf::Vertex(sf::Vector2f(tx+rotPX, ty+rotPY), sf::Color::White));
	temp.push_back(sf::Vertex(sf::Vector2f(tx+rotPX2, ty+rotPY2), sf::Color::White));

	return temp;
}

//Decides if the point (tx, ty) collides with any charge in qCharges array
bool Field::insideCharges(double tx, double ty, int startCharge){
	for(int i=0; i<qCharges.size(); i++)
		if(i!=startCharge && qCharges[i]->isPointInside(tx, ty) )
			return true;
	return false;
}

//Fills vector elFieldIntensityMap and finds max and min field intensity in screen
void Field::mapElFieldIntensity(){
	bool isPointInsideCharge=false;
	bool isPointInsideButtons=false;
	bool max_min_toInit = true;

	for (int x=0; x<windowWidth; x++){
		for (int y=0; y<windowHeight; y++){
			isPointInsideCharge=false;
            for(int i=0; i<qCharges.size(); i++){
                    isPointInsideCharge = qCharges[i]->isPointInside(x, y);
                    if(isPointInsideCharge)
                        break;
            }
			for(int i=0; i<buttons.size(); i++){
                    isPointInsideButtons = buttons[i].isPointInside(x, y);
                    if(isPointInsideButtons)
                        break;
            }
            if(!isPointInsideCharge && !isPointInsideButtons){
				elFieldIntensityMap[x][y] = elFieldValueInPoint(x,y).intensity;
				if(max_min_toInit){
					max_min_toInit = false;
					maxElFieldIntensity = elFieldIntensityMap[x][y];
					minElFieldIntensity = elFieldIntensityMap[x][y];
				}
				if(elFieldIntensityMap[x][y]>maxElFieldIntensity)
					maxElFieldIntensity = elFieldIntensityMap[x][y];
				if(elFieldIntensityMap[x][y]<minElFieldIntensity)
					minElFieldIntensity = elFieldIntensityMap[x][y];
			}
		}
	}
}

void Field::addCharge(){
    clear();
    update();
}

void Field::removeCharge(int x, int y){
    for(int i=0; i<qCharges.size(); i++)
        if(qCharges[i]->isPointInside(x,y)){
            qCharges.erase(qCharges.begin() + i);
            break;
        }
    clear();
    update();
}

void Field::selectCharge(int x, int y){
    for(int i=0; i<qCharges.size(); i++)
        if(qCharges[i]->isPointInside(x,y)){
            showing_pop_up = true;
            selected_index = i;
            break;
        }
}

//Clears Field and return to an initial stage
//It's called whenever you change modality or add or remove charge
void Field::clear(){
	elFieldLines.clear();
    eqPotLines.clear();
    eqPotPoints.clear();

    for(int i=0; i<potIntensityMap.size(); i++){
        potIntensityMap[i].clear();
    }
    potIntensityMap.clear();
    potIntensityMap = std::vector< std::vector<double> >(windowWidth, std::vector<double>(windowHeight));

	for(int i=0; i<elFieldIntensityMap.size(); i++){
        elFieldIntensityMap[i].clear();
    }
    elFieldIntensityMap.clear();
    elFieldIntensityMap = std::vector< std::vector<double> >(windowWidth, std::vector<double>(windowHeight));

	for(int i=0; i<arrows.size(); i++){
        arrows[i].clear();
    }
	arrows.clear();
}

//Constructor initializes vectors of vectors to prevent segmentation fault
Field::Field(){
	potIntensityMap = std::vector< std::vector<double> >(windowWidth, std::vector<double>(windowHeight));
	elFieldIntensityMap = std::vector< std::vector<double> >(windowWidth, std::vector<double>(windowHeight));
}
