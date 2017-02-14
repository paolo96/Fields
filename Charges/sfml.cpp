#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include "header.h"

const int windowWidth = 800;
const int windowHeight = 600;

const int lineDrawingReduction = 4;
const int buttonsSize = 80;

sf::Font font;
sf::RectangleShape pop_up_rect;
const sf::Color greyCharge(128, 128, 128);
const sf::Color redButtonOn(255, 99, 71);
const sf::Color redBoundsButtonOn(255, 69, 0);
const sf::Color orangeButtonOff(255, 165, 0);
const sf::Color orangeBoundsButtonOff(255, 140, 0);

Field mainField;
std::vector<Button> buttons;

std::string size_input = "";
std::string value_input = "";
int add_charge_holding_x = -1;
int add_charge_holding_y = -1;
int selected_index = -1;
bool inputing_size = true;
bool showing_pop_up = false;
bool adding_charge = true;
bool removing_charge = false;
bool selecting_charge = false;
bool isEqPotLines = true;
bool isElFieldLines = false;
bool isElFieldColor = false;
bool shapingCircle = true;
bool shapingSquare = false;
bool shapingCustom = false;

int main()
{
	
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Charges");
	
    initializeConfig();

	//Main window loop
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
            if (event.type == sf::Event::Closed){
				window.close();
            } else if(event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i position = sf::Mouse::getPosition(window);
                std::cout<<"Mouse click at ("<<position.x<<","<<position.y<<")"<<std::endl;
                
                if(showing_pop_up && !( (position.x>=pop_up_rect.getPosition().x) && (position.y>=pop_up_rect.getPosition().y) && (position.x<=(pop_up_rect.getPosition().x+pop_up_rect.getSize().x)) && (position.y<=(pop_up_rect.getPosition().y+pop_up_rect.getSize().y)) ) ){
                    showing_pop_up = false;
                    selected_index = -1;
                } else if(!checkForButtonsClick(position.x, position.y)){
                    if(adding_charge){
                        showing_pop_up=true;
                        add_charge_holding_x = position.x;
                        add_charge_holding_y = position.y;
                    }else if(removing_charge && mainField.qCharges.size()>1)
                        mainField.removeCharge(position.x, position.y);
                    else if(selecting_charge)
                        mainField.selectCharge(position.x, position.y);
                }
            } else if (event.type == sf::Event::TextEntered) {
                std::cout<<"Pressed "<<event.text.unicode<<std::endl;
                //Accept only numbers and the decimal point
                if ((((event.text.unicode <= 57) && (event.text.unicode >= 48)) || (event.text.unicode == 46)) && adding_charge && showing_pop_up){
                    if(inputing_size)
                        size_input += static_cast<char>(event.text.unicode);
                    else
                        value_input += static_cast<char>(event.text.unicode);
                
                //Accet enter or space to proceed to next input
                } else if((event.text.unicode==10 || event.text.unicode==32) && showing_pop_up && adding_charge){
                    if(inputing_size)
                        inputing_size = false;
                    else {
                        showing_pop_up = false;
                        inputing_size = true;
                        if(shapingCircle)
                            mainField.addCharge(Charge(CIRCLE_SHAPE, add_charge_holding_x, add_charge_holding_y, atof(size_input.c_str()), atof(value_input.c_str())));
                        else if(shapingSquare)
                            mainField.addCharge(Charge(SQUARE_SHAPE, add_charge_holding_x, add_charge_holding_y, atof(size_input.c_str()), atof(value_input.c_str())));
                        else if(shapingCustom)
                            mainField.addCharge(CustomCharge(add_charge_holding_x, add_charge_holding_y, atof(size_input.c_str()), atof(value_input.c_str())));
                        size_input="";
                        value_input="";
                    }
                }
            }
            
        }
        

		window.clear();
		draw(window);
		window.display();
	}
	
	return 0;
}

//Initializes configurations of field (like default first charge), buttons and graphics objects
void initializeConfig(){
    
    font.loadFromFile("sansation.ttf");
    
    pop_up_rect = sf::RectangleShape(sf::Vector2f(windowWidth/2.5, windowHeight/2.5));
    pop_up_rect.setPosition((windowWidth/2)-(windowWidth/5), (windowHeight/2)-(windowHeight/5));
    
    const int firstChargeSize = 10;
    Charge firstCenterCharge = Charge(CIRCLE_SHAPE, windowWidth/2, windowHeight/2, firstChargeSize, 1);
    mainField.qCharges.push_back(firstCenterCharge);
    mainField.mapField();
    
    buttons.push_back(Button("Add Charge", windowWidth-buttonsSize,0,buttonsSize,buttonsSize,adding_charge));
    buttons.push_back(Button("Remove Charge", windowWidth-buttonsSize,buttonsSize,buttonsSize,buttonsSize,removing_charge));
    buttons.push_back(Button("Select Charge", windowWidth-buttonsSize,buttonsSize*2,buttonsSize,buttonsSize,selecting_charge));
    buttons.push_back(Button("EqPot Lines", 0,windowHeight-buttonsSize,buttonsSize,buttonsSize,isEqPotLines));
    buttons.push_back(Button("ElField Lines", buttonsSize,windowHeight-buttonsSize,buttonsSize,buttonsSize,isElFieldLines));
    buttons.push_back(Button("ElField Color", buttonsSize*2,windowHeight-buttonsSize,buttonsSize,buttonsSize,isElFieldColor));
    buttons.push_back(Button("Circle Shape", 0,0,buttonsSize,buttonsSize,shapingCircle));
    buttons.push_back(Button("Square Shape", 0,buttonsSize,buttonsSize,buttonsSize,shapingSquare));
    buttons.push_back(Button("Custom Shape", 0,buttonsSize*2,buttonsSize,buttonsSize,shapingCustom));
}

//Draw loop callade every FPS
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
    
    
    drawCharges(window);
    drawButtons(window);
    drawPopUp(window);
    
}

//Drawing charges
void drawCharges(sf::RenderWindow& window){
    for(int i=0; i<mainField.qCharges.size(); i++){
        
        if(mainField.qCharges[i].shape==CIRCLE_SHAPE){
            //WARNING: SFML takes as size radius not diameter in CircleShape
            sf::CircleShape shape(mainField.qCharges[i].size);
            shape.setFillColor(greyCharge);
            shape.setPosition( mainField.qCharges[i].x-mainField.qCharges[i].size , mainField.qCharges[i].y-mainField.qCharges[i].size );
        
            window.draw(shape);
        } else if(mainField.qCharges[i].shape==SQUARE_SHAPE){
            sf::RectangleShape shape(sf::Vector2f(mainField.qCharges[i].size, mainField.qCharges[i].size));
            shape.setFillColor(greyCharge);
            shape.setPosition(mainField.qCharges[i].x , mainField.qCharges[i].y);
            
            window.draw(shape);
        } else if(mainField.qCharges[i].shape==CUSTOM_SHAPE){
            CustomCharge temp = static_cast<CustomCharge>(mainField.qCharges[i]);
            for(int x=0; mainField.qCharges[i].)
            sf::RectangleShape shape(sf::Vector2f(mainField.qCharges[i].size, mainField.qCharges[i].size));
            shape.setFillColor(greyCharge);
            shape.setPosition(mainField.qCharges[i].x , mainField.qCharges[i].y);
            
            window.draw(shape);
        }
    }
}

//Drawing buttons
void drawButtons(sf::RenderWindow& window){
    for(int i=0; i<buttons.size(); i++){
        sf::RectangleShape rectButton(sf::Vector2f(buttons[i].width, buttons[i].height));
        if((i==0 && adding_charge) || (i==1 && removing_charge) || (i==2 && selecting_charge) || (i==3 && isEqPotLines) || (i==4 && isElFieldLines) || (i==5 && isElFieldColor) || (i==6 && shapingCircle) || (i==7 && shapingSquare)){
            rectButton.setFillColor(redButtonOn);
            rectButton.setOutlineColor(redBoundsButtonOn);
        } else {
            rectButton.setFillColor(orangeButtonOff);
            rectButton.setOutlineColor(orangeBoundsButtonOff);
        }
        rectButton.setPosition(buttons[i].x, buttons[i].y);
        rectButton.setOutlineThickness(-10);
        
        std::string firstLine = buttons[i].name.substr(0, buttons[i].name.find(" "));
        std::string secondLine = buttons[i].name.substr(buttons[i].name.find(" "), buttons[i].name.length());
        sf::Text buttText1;
        buttText1.setCharacterSize(14);
        buttText1.setFillColor(sf::Color::White);
        buttText1.setFont(font);
        buttText1.setString(firstLine);
        if(i==0)
            buttText1.setPosition(buttons[i].x+(buttonsSize/3), buttons[i].y+(buttonsSize/4.8));
        else if (i==1)
            buttText1.setPosition(buttons[i].x+(buttonsSize/6), buttons[i].y+(buttonsSize/4.8));
        else if (i==2)
            buttText1.setPosition(buttons[i].x+(buttonsSize/4), buttons[i].y+(buttonsSize/4.8));
        else if (i==3)
            buttText1.setPosition(buttons[i].x+(buttonsSize/4), buttons[i].y+(buttonsSize/4.8));
        else if (i==4)
            buttText1.setPosition(buttons[i].x+(buttonsSize/4), buttons[i].y+(buttonsSize/4.8));
        else if (i==5)
            buttText1.setPosition(buttons[i].x+(buttonsSize/4), buttons[i].y+(buttonsSize/4.8));
        else if (i==6)
            buttText1.setPosition(buttons[i].x+(buttonsSize/3.8), buttons[i].y+(buttonsSize/4.8));
        else if (i==7)
            buttText1.setPosition(buttons[i].x+(buttonsSize/4.7), buttons[i].y+(buttonsSize/4.8));
        sf::Text buttText2;
        buttText2.setCharacterSize(14);
        buttText2.setFillColor(sf::Color::White);
        buttText2.setFont(font);
        buttText2.setString(secondLine);
        if(i==3 || i==4)
            buttText2.setPosition(buttons[i].x+(buttonsSize/4.2), buttons[i].y+(buttonsSize/1.9));
        else
            buttText2.setPosition(buttons[i].x+(buttonsSize/5.2), buttons[i].y+(buttonsSize/1.9));
        
        window.draw(rectButton);
        window.draw(buttText1);
        window.draw(buttText2);
    }
}

//Draw kind of pop_up
void drawPopUp(sf::RenderWindow& window){
    if(showing_pop_up){
        pop_up_rect.setFillColor(sf::Color::White);
        pop_up_rect.setOutlineColor(sf::Color::Red);
        pop_up_rect.setOutlineThickness(-10);
        
        window.draw(pop_up_rect);
        
        //draw add charge input pop up
        if(adding_charge){
            sf::Text pop_title;
            pop_title.setCharacterSize(20);
            pop_title.setFillColor(sf::Color::Black);
            pop_title.setFont(font);
            pop_title.setString("Adding Charge");
            pop_title.setStyle(sf::Text::Bold);
            pop_title.setPosition(windowWidth/2.5, pop_up_rect.getPosition().y+20);
            
            
            sf::Text input_size;
            input_size.setCharacterSize(17);
            input_size.setFillColor(sf::Color::Black);
            input_size.setFont(font);
            input_size.setString("Input Size: "+size_input);
            if(inputing_size)
                input_size.setString("Input Size: "+size_input+"   <-");
            input_size.setPosition(pop_up_rect.getPosition().x+50, pop_up_rect.getPosition().y+pop_up_rect.getSize().y/4+(pop_up_rect.getSize().y/8));
            
            sf::Text input_value;
            input_value.setCharacterSize(17);
            input_value.setFillColor(sf::Color::Black);
            input_value.setFont(font);
            input_value.setString("Input Value: "+value_input);
            if(!inputing_size)
                input_value.setString("Input Value: "+value_input+"   <-");
            input_value.setPosition(pop_up_rect.getPosition().x+50, pop_up_rect.getPosition().y+pop_up_rect.getSize().y/4+(pop_up_rect.getSize().y/8)*2);
            
            sf::Text enter_confirm;
            enter_confirm.setCharacterSize(15);
            enter_confirm.setFillColor(sf::Color::Black);
            enter_confirm.setFont(font);
            enter_confirm.setString("Press enter on keyboard when done");
            enter_confirm.setPosition(pop_up_rect.getPosition().x+40, pop_up_rect.getPosition().y+pop_up_rect.getSize().y/4+(pop_up_rect.getSize().y/8)*4);
            
            window.draw(pop_title);
            window.draw(input_size);
            window.draw(input_value);
            window.draw(enter_confirm);
        }
        
        //draw selecting pop up
        if(selected_index!=-1 && selecting_charge){
            sf::Text pop_title;
            pop_title.setCharacterSize(20);
            pop_title.setFillColor(sf::Color::Black);
            pop_title.setFont(font);
            pop_title.setString("Charge Selected");
            pop_title.setStyle(sf::Text::Bold);
            pop_title.setPosition(windowWidth/2.5, pop_up_rect.getPosition().y+20);
            
            sf::Text pop_x;
            pop_x.setCharacterSize(15);
            pop_x.setFillColor(sf::Color::Black);
            pop_x.setFont(font);
            pop_x.setString("x = "+std::to_string(mainField.qCharges[selected_index].x));
            pop_x.setPosition(pop_up_rect.getPosition().x+20, pop_up_rect.getPosition().y+pop_up_rect.getSize().y/4);
            
            sf::Text pop_y;
            pop_y.setCharacterSize(15);
            pop_y.setFillColor(sf::Color::Black);
            pop_y.setFont(font);
            pop_y.setString("y = "+std::to_string(mainField.qCharges[selected_index].y));
            pop_y.setPosition(pop_up_rect.getPosition().x+20, pop_up_rect.getPosition().y+pop_up_rect.getSize().y/4+(pop_up_rect.getSize().y/8));
            
            sf::Text pop_size;
            pop_size.setCharacterSize(15);
            pop_size.setFillColor(sf::Color::Black);
            pop_size.setFont(font);
            pop_size.setString("Size = "+std::to_string(mainField.qCharges[selected_index].size));
            pop_size.setPosition(pop_up_rect.getPosition().x+20, pop_up_rect.getPosition().y+pop_up_rect.getSize().y/4+(pop_up_rect.getSize().y/8)*2);
            
            sf::Text pop_value;
            pop_value.setCharacterSize(15);
            pop_value.setFillColor(sf::Color::Black);
            pop_value.setFont(font);
            pop_value.setString("Value = "+std::to_string(mainField.qCharges[selected_index].value));
            pop_value.setPosition(pop_up_rect.getPosition().x+20, pop_up_rect.getPosition().y+pop_up_rect.getSize().y/4+(pop_up_rect.getSize().y/8)*3);
            
            sf::Text pop_type;
            pop_type.setCharacterSize(15);
            pop_type.setFillColor(sf::Color::Black);
            pop_type.setFont(font);
            pop_type.setString("Type = "+mainField.qCharges[selected_index].shape);
            pop_type.setPosition(pop_up_rect.getPosition().x+20, pop_up_rect.getPosition().y+pop_up_rect.getSize().y/4+(pop_up_rect.getSize().y/8)*4);
            
            window.draw(pop_title);
            window.draw(pop_x);
            window.draw(pop_y);
            window.draw(pop_size);
            window.draw(pop_value);
            window.draw(pop_type);
        }
    }
}

//Check if on mouse click buttons have been clicked
bool checkForButtonsClick(int x, int y){
    for(int i=0; i<buttons.size(); i++){
        if(buttons[i].isPointInside(x, y)){
            std::cout<<"Clicked button "<<i<<std::endl;
            if(i==0){
                adding_charge=true;
                removing_charge=false;
                selecting_charge=false;
                selected_index=-1;
            } else if (i==1){
                adding_charge=false;
                removing_charge=true;
                selecting_charge=false;
                selected_index=-1;
            } else if(i==2){
                adding_charge=false;
                removing_charge=false;
                selecting_charge=true;
                selected_index=-1;
            } else if(i==3){
                isEqPotLines=true;
                isElFieldLines=false;
                isElFieldColor=false;
            } else if(i==4){
                isEqPotLines=false;
                isElFieldLines=true;
                isElFieldColor=false;
            } else if(i==5){
                isEqPotLines=false;
                isElFieldLines=false;
                isElFieldColor=true;
            } else if(i==6){
                shapingCircle=true;
                shapingSquare=false;
            } else if(i==7){
                shapingCircle=false;
                shapingSquare=true;
            }
            return true;
        }
    }
    return false;
}
