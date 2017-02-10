#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include "header.h"

//Evaluates potential intensity in every point of the window
void Field::mapField(){

	for (int x=0; x<windowWidth; x++){
		for (int y=0; y<windowHeight; y++){
			bool isPointInsideCharge=false;
			for(int i=0; i<qCharges.size(); i++){
				if( ((abs(x-qCharges[0].x)<(qCharges[0].size/2)) && (abs(y-qCharges[0].y)<(qCharges[0].size/2))) ) {
					isPointInsideCharge = true;
					break;
				}
			}
			if(!isPointInsideCharge){
				//std::cout<<"DEBUG: ("<<x<<","<<y<<") -> "<<potValueInPoint(x,y)<<std::endl;
				potIntensityMap[x][y] = potValueInPoint(x,y);
			}
		}
	}

	
	std::cout<<"Potential mapped"<<std::endl;
	findEquipotentialPoints();
	std::cout<<"Found " << eqPotPoints.size()  << " equipotential points"<<std::endl;
	findEquipotentialLines();
	std::cout<<"Found " << eqPotLines.size() << " equipotential lines"<<std::endl;

}

//Return potential in the point (x,y) made by all charges in the field
double Field::potValueInPoint(int x, int y){

	double totPotValue = 0;
	double distance, potFromSingleCharge;
	
	for(int i=0; i<qCharges.size(); i++){
		distance = sqrt( pow(x-qCharges[i].x, 2) + pow(y-qCharges[i].y, 2) );
		
		potFromSingleCharge = ( (k0*qCharges[i].value) / distance );
		
		totPotValue += potFromSingleCharge;
	}
	
	return totPotValue;
}

//Fill vector eqPotPoints with Points at same potential
void Field::findEquipotentialPoints(){

	double delta;

	for (int x=0; x<windowWidth; x++){
		for (int y=0; y<windowHeight; y++){

			delta = abs( potIntensityMap[x][y]/linesDeepReduction );
			
			for (int X=x; X<windowWidth; X++){
				for (int Y=y+1; Y<windowHeight; Y++){
					if( (potIntensityMap[x][y]+delta)>=(potIntensityMap[X][Y]-delta) && (potIntensityMap[x][y]-delta)<=(potIntensityMap[X][Y]+delta) ) {
						
						Point point = Point(x, y, potIntensityMap[x][y]);
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

	for(int i=0; i<eqPotPoints.size(); i++){
		Line temp;
		for(int y=i+1; y<eqPotPoints.size(); y++){
			double delta = abs(eqPotPoints[i].value/linesDeepReduction);
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

Field::Field(){
	potIntensityMap = std::vector< std::vector<double> >(windowWidth, std::vector<double>(windowHeight));
}
