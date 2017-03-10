#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <memory>
#include "header.h"

//Evaluates potential intensity in every point of the window
void Field::mapField(){

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

	
	std::cout<<"Potential mapped"<<std::endl;
	findEquipotentialPoints();
	std::cout<<"Found " << eqPotPoints.size()  << " equipotential points"<<std::endl;
	findEquipotentialLines();
	std::cout<<"Found " << eqPotLines.size() << " equipotential lines"<<std::endl;
	findElFieldLines();
	std::cout<<"Found " << elFieldLines.size() << " electric field lines"<<std::endl;

}

//Return potential in the point (x,y) made by all charges in the field
double Field::potValueInPoint(int x, int y){

	double totPotValue = 0;
	double distance, potFromSingleCharge;
    
	for(int i=0; i<qCharges.size(); i++){
        if(qCharges[i]->shape==CIRCLE_SHAPE){
            distance = sqrt( pow(x-qCharges[i]->x, 2) + pow(y-qCharges[i]->y, 2) );
		
            potFromSingleCharge = ( (k0*(qCharges[i]->value)) / distance );
		
            totPotValue += potFromSingleCharge;
        } else if(qCharges[i]->shape==SQUARE_SHAPE){
            
            for(int X=qCharges[i]->x; X<=(qCharges[i]->x+qCharges[i]->size); X++){
                for(int Y=qCharges[i]->y; Y<=(qCharges[i]->y+qCharges[i]->size); Y++){
                    distance = sqrt( pow(x-X, 2) + pow(y-Y, 2) );
                    
                    potFromSingleCharge = ( (k0*(qCharges[i]->value)) / distance );
                    
                    totPotValue += potFromSingleCharge;
                }
            }
        } else if(qCharges[i]->shape==CUSTOM_SHAPE){
            int posX = qCharges[i]->x;
            int posY = qCharges[i]->y;
            std::shared_ptr<CustomCharge> tempPTR = std::static_pointer_cast<CustomCharge>(qCharges[i]);
            for(int i=0; i<tempPTR->shape_map.size(); i++){
                distance = sqrt( pow(x-(tempPTR->shape_map[i].x+posX), 2) + pow(y-(tempPTR->shape_map[i].y+posY), 2) );
                
                potFromSingleCharge = ( (k0*(tempPTR->value)) / distance );
                
                totPotValue += potFromSingleCharge;
            }
        }
	}
	return totPotValue;
}

//Fill vector eqPotPoints with Points at same potential
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

//Sort vector eqPotPoints by potential intensity and fill vector eqPotLines with Lines of Points at same potential intensity
void Field::findEquipotentialLines(){

	std::sort(eqPotPoints.begin(), eqPotPoints.end());
    
    double delta;
    for(int i=0; i<eqPotPoints.size(); i++){
        Line temp;
        delta = abs( int(eqPotPoints[i].value/linesDeepReduction) );
        for(int y=i+1; y<eqPotPoints.size(); y++){
            //std::cout<<"DEBUG: i->"<<eqPotPoints[i].value<<" y->"<<eqPotPoints[y].value<<" delta->"<<delta<<std::endl;
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

Vector Field::elFieldValueInPoint(int x, int y){
	
	double totFieldValueX = 0, totFieldValueY = 0, distance, alpha;

	for(int i=0; i<qCharges.size(); i++){
		distance = sqrt( pow(x-qCharges[i]->x, 2) + pow(y-qCharges[i]->y, 2) );

		alpha = angleFromPoints(x,y,qCharges[i]->x,qCharges[i]->y);

		totFieldValueX += cos(alpha)*((k0*qCharges[i]->value)/pow(distance,2));
		totFieldValueY += sin(alpha)*((k0*qCharges[i]->value)/pow(distance,2));

	}

	Vector v( x, y, sqrt(pow(totFieldValueX, 2) + pow(totFieldValueY, 2)), angleFromPoints(totFieldValueX, totFieldValueY,0,0) );
	return v;
}

Line Field::buildElFieldLine(Point startPoint, int startCharge){
	//TODO write it assuming pass by value not by reference
	Line temp;
	temp.pointsCoord.push_back(startPoint);

	double tx = startPoint.x;
	double ty = startPoint.y;
	double talpha = startPoint.value;

	while(tx<windowWidth-2 && tx>2 && ty<windowHeight-2 && ty>2 && !insideCharges(tx, ty, startCharge) ){
		tx+=cos(talpha);
		ty+=sin(talpha);
		talpha = elFieldValueInPoint(round(tx), round(ty)).alpha;
		
		if(qCharges[startCharge]->value<0){
			talpha-=PI;
			if(talpha<0)
				talpha+=2*PI;
		}
		
		//TODO can avoid this?
		double tpx = tx, tpy = ty;
		Point loopPoint(tpx, tpy, talpha);
		temp.pointsCoord.push_back(loopPoint);
	}
	
	return temp;
}

void Field::findElFieldLines(){
	for(int i=0; i<qCharges.size(); i++)
		for(double alpha=0; alpha<PI*2; alpha+=((PI*2)/linesPerCharge)){
			int tx = round(qCharges[i]->x+(qCharges[i]->size*cos(alpha)));
			int ty = round(qCharges[i]->y+(qCharges[i]->size*sin(alpha)));
			Point temp(tx, ty, alpha);
			elFieldLines.push_back(buildElFieldLine(temp, i));
		}
}

bool Field::insideCharges(double tx, double ty, int startCharge){
	//TODO check square and custom charges cases
	for(int i=0; i<qCharges.size(); i++)
		if(i!=startCharge && (sqrt(pow(qCharges[i]->x-tx, 2)+pow(qCharges[i]->y-ty, 2))<qCharges[i]->size*sqrt(2)) )
			return true;
	return false;
}

void Field::addCharge(){
    clear();
    mapField();
}

void Field::removeCharge(int x, int y){
    for(int i=0; i<qCharges.size(); i++)
        if(qCharges[i]->isPointInside(x,y)){
            qCharges.erase(qCharges.begin() + i);
            break;
        }
    clear();
    mapField();
}

void Field::selectCharge(int x, int y){
    for(int i=0; i<qCharges.size(); i++)
        if(qCharges[i]->isPointInside(x,y)){
            showing_pop_up = true;
            selected_index = i;
            break;
        }
}

void Field::clear(){
	elFieldLines.clear();
    eqPotLines.clear();
    eqPotPoints.clear();
    for(int i=0; i<potIntensityMap.size(); i++){
        potIntensityMap[i].clear();
    }
    potIntensityMap.clear();
    potIntensityMap = std::vector< std::vector<double> >(windowWidth, std::vector<double>(windowHeight));
}

Field::Field(){
	potIntensityMap = std::vector< std::vector<double> >(windowWidth, std::vector<double>(windowHeight));
}
