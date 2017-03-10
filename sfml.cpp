#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include "header.h"

const int windowWidth = 800;
const int windowHeight = 600;
const int mouse_draw_size = 2;

int add_charge_holding_x = -1;
int add_charge_holding_y = -1;

int selected_index = -1;
bool inputing_size = true;
bool inputing_custom = false;
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
bool dragging_mouse = false;

std::string size_input = "";
std::string value_input = "";

std::shared_ptr<CustomCharge> currentCustom;
std::vector <sf::Vertex> paintingLines;
Field mainField;

int main()
{
	

	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Charges");

	sf::Texture texture;
   	sf::Sprite sprite;
   	texture.create(800,600);
        window.clear(sf::Color::Black);
        texture.update(window);

	initializeConfig();

	//Main window loop
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
            if (event.type == sf::Event::Closed){
				window.close();
            } else if((event.type == sf::Event::MouseButtonPressed) && (event.mouseButton.button == sf::Mouse::Left)) {
                sf::Vector2i position = sf::Mouse::getPosition(window);
                std::cout<<"Mouse click at ("<<position.x<<","<<position.y<<")"<<std::endl;
                dragging_mouse=true;
                
                if(showing_pop_up){
                    bool mouse_inside_pop_up =(position.x>=pop_up_rect.getPosition().x) && (position.y>=pop_up_rect.getPosition().y) && (position.x<=(pop_up_rect.getPosition().x+pop_up_rect.getSize().x)) && (position.y<=(pop_up_rect.getPosition().y+pop_up_rect.getSize().y));
                    if(!mouse_inside_pop_up){
                        showing_pop_up = false;
			inputing_custom = false;
			inputing_size = true;
                        selected_index = -1;
                    }
                } else if(!checkForButtonsClick(position.x, position.y)){
                    if(adding_charge){
                        if(shapingCustom && !(currentCustom.use_count() == 0)){
                            currentCustom->x=position.x;
                            currentCustom->y=position.y;
                            std::shared_ptr<CustomCharge> copy_of_curr_cust = std::make_shared<CustomCharge>(*currentCustom);
                            copy_of_curr_cust->value = copy_of_curr_cust->value / (copy_of_curr_cust->shape_map.size());
                            mainField.qCharges.push_back(copy_of_curr_cust);
                            mainField.addCharge();
                        } else {
                            showing_pop_up=true;
                            add_charge_holding_x = position.x;
                            add_charge_holding_y = position.y;
                        }
                    } else if(removing_charge && mainField.qCharges.size()>1)
                        mainField.removeCharge(position.x, position.y);
                    else if(selecting_charge)
                        mainField.selectCharge(position.x, position.y);
                }
            } else if ((event.type == sf::Event::MouseButtonReleased) && (event.mouseButton.button == sf::Mouse::Left)){
                sf::Vector2i position = sf::Mouse::getPosition(window);
                std::cout<<"Mouse released at ("<<position.x<<","<<position.y<<")"<<std::endl;
                dragging_mouse=false;
            	paintingLines.clear();
                
            } else if ((event.type == sf::Event::MouseMoved) && dragging_mouse && inputing_custom) {
		paintingLines.push_back(sf::Vertex(sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y), sf::Color::Black));
	} else if (event.type == sf::Event::TextEntered) {
                std::cout<<"Pressed "<<event.text.unicode<<std::endl;

                
                //Accept only numbers and the decimal point
                if ((((event.text.unicode <= 57) && (event.text.unicode >= 48)) || (event.text.unicode == 46) || (event.text.unicode == 45)) && adding_charge && showing_pop_up){
                    if(inputing_size)
                        size_input += static_cast<char>(event.text.unicode);
                    else
                        value_input += static_cast<char>(event.text.unicode);
                
                } else if((event.text.unicode==10 || event.text.unicode==32 || event.text.unicode==13) && showing_pop_up && adding_charge){
                    
                    //Accept enter or space to proceed to next input
                    if(inputing_custom){
                        inputing_custom=false;
                        showing_pop_up=false;
                        std::shared_ptr<CustomCharge> copy_of_curr_cust = std::make_shared<CustomCharge>(*currentCustom);
                        copy_of_curr_cust->value = copy_of_curr_cust->value / (copy_of_curr_cust->shape_map.size());
                        mainField.qCharges.push_back(copy_of_curr_cust);
                        mainField.addCharge();
                        pop_up_rect = sf::RectangleShape(sf::Vector2f(windowWidth/2.5, windowHeight/2.5));
                        pop_up_rect.setPosition((windowWidth/2)-(windowWidth/5), (windowHeight/2)-(windowHeight/5));
                    } else {
                        if(inputing_size)
                            inputing_size = false;
                        else {
                            inputing_size = true;
                            if(shapingCircle) {
                                std::shared_ptr<Charge> qToAdd(new Charge(CIRCLE_SHAPE, add_charge_holding_x, add_charge_holding_y, atof(size_input.c_str()), atof(value_input.c_str())));
                                mainField.qCharges.push_back(qToAdd);
                                mainField.addCharge();
                            } else if (shapingSquare) {
                                double size = atof(size_input.c_str());
                                double valueCorrect = atof(value_input.c_str()) / (size*size);
                                std::shared_ptr<SquareCharge> qToAdd(new SquareCharge(add_charge_holding_x, add_charge_holding_y, size, valueCorrect));
                                mainField.qCharges.push_back(qToAdd);
                                mainField.addCharge();
                            } else if(shapingCustom) {
                                std::shared_ptr<CustomCharge> qToAdd(new CustomCharge(add_charge_holding_x, add_charge_holding_y, atof(size_input.c_str()), atof(value_input.c_str())));
                                currentCustom = qToAdd;
                            }
                            size_input="";
                            value_input="";
                            if(shapingCustom){
                                inputing_custom=true;
                                pop_up_rect.setSize(sf::Vector2f((currentCustom->size),(currentCustom->size)));
                                pop_up_rect.setPosition((windowWidth/2)-((currentCustom->size)/2), (windowHeight/2)-((currentCustom->size)/2));
                            }else
                                showing_pop_up=false;
                        }
                    }
                }
            }
            
        }
        
        window.clear();
        draw(window, mainField);
        window.draw(&paintingLines[0], paintingLines.size(), sf::LinesStrip);
	window.display();
	}
	
	return 0;
}

//Initializes configurations of field (like default first charge), buttons and graphics objects
void initializeConfig(){
    
    font.loadFromFile("sansation.ttf");
    
    pop_up_rect = sf::RectangleShape(sf::Vector2f(windowWidth/2.5, windowHeight/2.5));
    pop_up_rect.setPosition((windowWidth/2)-(windowWidth/5), (windowHeight/2)-(windowHeight/5));
    
    std::shared_ptr<Charge> firstCenterCharge(new Charge(CIRCLE_SHAPE, windowWidth/2, windowHeight/2, 10, 1));
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
                currentCustom.reset();
                shapingCircle=true;
                shapingSquare=false;
                shapingCustom=false;
            } else if(i==7){
                currentCustom.reset();
                shapingCircle=false;
                shapingSquare=true;
                shapingCustom=false;
            } else if(i==8){
                currentCustom.reset();
                shapingCircle=false;
                shapingSquare=false;
                shapingCustom=true;
            }
            return true;
        }
    }
    return false;
}
