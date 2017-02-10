#include <SFML/Graphics.hpp>
#include "header.h"

const int windowWidth = 800;
const int windowHeight = 600;

const int lineDrawingReduction = 4;
const sf::Color greyCharge(128, 128, 128);

Field mainField;

void draw(sf::RenderWindow& window){

	//Drawing equipotential lines
	for(int i=0; i<mainField.eqPotLines.size(); i+=lineDrawingReduction){
		for(int y=0; y<mainField.eqPotLines[i].pointsCoord.size(); y++){
			sf::RectangleShape pixel(sf::Vector2f(1, 1));
			pixel.setFillColor(sf::Color::White);
			pixel.setPosition(mainField.eqPotLines[i].pointsCoord[y].x, mainField.eqPotLines[i].pointsCoord[y].y);

		window.draw(pixel);
		}
	}

	//Drawing charges
	for(int i=0; i<mainField.qCharges.size(); i++){

		//WARNING: SFML takes as size radius not diameter in CircleShape
		sf::CircleShape shape(mainField.qCharges[i].size);
		shape.setFillColor(greyCharge);
		shape.setPosition( mainField.qCharges[i].x-mainField.qCharges[i].size , mainField.qCharges[i].y-mainField.qCharges[i].size );

		window.draw(shape);
	}

}

int main()
{
	
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Charges");
	
	const int firstChargeSize = 10;
	Charge firstCenterCharge = Charge(windowWidth/2, windowHeight/2, firstChargeSize, 1);
	mainField.qCharges.push_back(firstCenterCharge);
	mainField.mapField();

	//Main window loop
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			}

		window.clear();
		draw(window);
		window.display();
	}
	
	return 0;
}


