#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include "header.h"

int add_charge_holding_x = -1;
int add_charge_holding_y = -1;

int mouse_draw_size = 2;
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
Field mainField;

int main()
{
	

	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Charges");

	initializeConfig();

	//Main window loop
	while (window.isOpen())
	{

		//Event loop (keyboard, mouse, exit window)
		sf::Event event;
		while (window.pollEvent(event)) 
		{
            if (event.type == sf::Event::Closed)
				window.close();
            else if((event.type == sf::Event::MouseButtonPressed) && (event.mouseButton.button == sf::Mouse::Left))
                leftButtonMousePress(window);
            else if ((event.type == sf::Event::MouseButtonReleased) && (event.mouseButton.button == sf::Mouse::Left))
    			dragging_mouse=false;
            else if ((event.type == sf::Event::MouseMoved) && dragging_mouse && inputing_custom)
				paintingCustomCharge(window);
			else if (event.type == sf::Event::TextEntered)
                keyboardPress(window, event);
        }
        
        window.clear();
        draw(window);
		window.display();
	}
	
	return 0;
}

//Initializes default first charge, buttons and graphics objects
void initializeConfig(){
    
    font.loadFromFile("sansation.ttf");
    
    pop_up_rect = sf::RectangleShape(sf::Vector2f(windowWidth/2.5, windowHeight/2.5));
    pop_up_rect.setPosition((windowWidth/2)-(windowWidth/5), (windowHeight/2)-(windowHeight/5));
    
    std::shared_ptr<CircleCharge> firstCenterCharge(new CircleCharge(windowWidth/2, windowHeight/2, 10, 1));
    mainField.qCharges.push_back(firstCenterCharge);
    mainField.update();
    
    buttons.push_back(Button("Add Charge", windowWidth-BUTTONS_SIZE,0,BUTTONS_SIZE,BUTTONS_SIZE,adding_charge));
    buttons.push_back(Button("Remove Charge", windowWidth-BUTTONS_SIZE,BUTTONS_SIZE,BUTTONS_SIZE,BUTTONS_SIZE,removing_charge));
    buttons.push_back(Button("Select Charge", windowWidth-BUTTONS_SIZE,BUTTONS_SIZE*2,BUTTONS_SIZE,BUTTONS_SIZE,selecting_charge));
    buttons.push_back(Button("EqPot Lines", 0,windowHeight-BUTTONS_SIZE,BUTTONS_SIZE,BUTTONS_SIZE,isEqPotLines));
    buttons.push_back(Button("ElField Lines", BUTTONS_SIZE,windowHeight-BUTTONS_SIZE,BUTTONS_SIZE,BUTTONS_SIZE,isElFieldLines));
    buttons.push_back(Button("ElField Color", BUTTONS_SIZE*2,windowHeight-BUTTONS_SIZE,BUTTONS_SIZE,BUTTONS_SIZE,isElFieldColor));
    buttons.push_back(Button("Circle Shape", 0,0,BUTTONS_SIZE,BUTTONS_SIZE,shapingCircle));
    buttons.push_back(Button("Square Shape", 0,BUTTONS_SIZE,BUTTONS_SIZE,BUTTONS_SIZE,shapingSquare));
    buttons.push_back(Button("Custom Shape", 0,BUTTONS_SIZE*2,BUTTONS_SIZE,BUTTONS_SIZE,shapingCustom));
}

//Handles left button mouse click
void leftButtonMousePress(sf::RenderWindow& window){

	sf::Vector2i position = sf::Mouse::getPosition(window);
    //std::cout<<"Mouse click at ("<<position.x<<","<<position.y<<")"<<std::endl;
    dragging_mouse=true;
    
    if(showing_pop_up){
		//Removes pop_up if click outside it

        bool mouse_inside_pop_up =(position.x>=pop_up_rect.getPosition().x) && (position.y>=pop_up_rect.getPosition().y) && (position.x<=(pop_up_rect.getPosition().x+pop_up_rect.getSize().x)) && (position.y<=(pop_up_rect.getPosition().y+pop_up_rect.getSize().y));

        if(!mouse_inside_pop_up){
			pop_up_rect = sf::RectangleShape(sf::Vector2f(windowWidth/2.5, windowHeight/2.5));
			pop_up_rect.setPosition((windowWidth/2)-(windowWidth/5), (windowHeight/2)-(windowHeight/5));
            showing_pop_up = false;
			inputing_custom = false;
			inputing_size = true;
            selected_index = -1;
        }
    } else if(!checkForButtonsClick(position.x, position.y)){
		//Handles click on field adding, removing or selecting charge

        if(adding_charge){
            if(shapingCustom && !(currentCustom.use_count() == 0)){
                currentCustom->x=position.x;
                currentCustom->y=position.y;
                std::shared_ptr<CustomCharge> copy_of_curr_cust = std::make_shared<CustomCharge>(*currentCustom);
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
}

//Handles mouse drag to draw custom charge shape
void paintingCustomCharge(sf::RenderWindow& window){

	sf::Vector2i position = sf::Mouse::getPosition(window);
	bool mouseInsidePopUp = ((position.x-pop_up_rect.getPosition().x)>=0 && (position.x-pop_up_rect.getPosition().x)<=pop_up_rect.getSize().x) && ((position.y-pop_up_rect.getPosition().y)>=0 && (position.y-pop_up_rect.getPosition().y)<=pop_up_rect.getSize().y);

    if(mouseInsidePopUp){
        
        int xRelShape = position.x-pop_up_rect.getPosition().x;
        int yRelShape = position.y-pop_up_rect.getPosition().y;
        for(int i=-mouse_draw_size; i<=mouse_draw_size; i++)
            if( (xRelShape+i>=0) && (xRelShape+i<currentCustom->size) )
                for(int j=-mouse_draw_size; j<=mouse_draw_size; j++)
                    if( (yRelShape+j>=0) && (yRelShape+j<currentCustom->size) )
                        currentCustom->shape_map.push_back(Point(xRelShape+i, yRelShape+j));
                
    }
}

//Handles key press on keyboard
void keyboardPress(sf::RenderWindow& window, sf::Event& event){
	//char output = event.text.unicode;
	//std::cout<<"Pressed \""<<output<<"\""<<std::endl;
   
    if ((((event.text.unicode <= 57) && (event.text.unicode >= 48)) || (event.text.unicode == 46) || (event.text.unicode == 45)) && adding_charge && showing_pop_up && !inputing_custom){
    	//Accepts numbers, decimal point and '-' to input size and value of the charge

        if(inputing_size)
            size_input += static_cast<char>(event.text.unicode);
        else
            value_input += static_cast<char>(event.text.unicode);
    
    } else if(((event.text.unicode <= 57) && (event.text.unicode >= 48)) && inputing_custom){
    	//Accepts only numbers to change painting size

		std::cout<<"Painting size changed to "<<event.text.unicode-48<<std::endl;
		mouse_draw_size = event.text.unicode-48;
	} else if((event.text.unicode==10 || event.text.unicode==32 || event.text.unicode==13) && showing_pop_up && adding_charge){
        //Accepts enter or space to proceed to next input

        if(inputing_custom){
			//Ends painting and add custom charge to field

            inputing_custom=false;
            showing_pop_up=false;
            std::shared_ptr<CustomCharge> copy_of_curr_cust = std::make_shared<CustomCharge>(*currentCustom);
            mainField.qCharges.push_back(copy_of_curr_cust);
            mainField.addCharge();
            pop_up_rect = sf::RectangleShape(sf::Vector2f(windowWidth/2.5, windowHeight/2.5));
            pop_up_rect.setPosition((windowWidth/2)-(windowWidth/5), (windowHeight/2)-(windowHeight/5));
        } else {
            if(inputing_size)
                inputing_size = false;
            else {
				//Ends inputing size and value

                inputing_size = true;
                if(shapingCircle) {
                    std::shared_ptr<CircleCharge> qToAdd(new CircleCharge(add_charge_holding_x, add_charge_holding_y, atof(size_input.c_str()), atof(value_input.c_str())));
                    mainField.qCharges.push_back(qToAdd);
                    mainField.addCharge();
                } else if (shapingSquare) {
                    std::shared_ptr<SquareCharge> qToAdd(new SquareCharge(add_charge_holding_x, add_charge_holding_y, atof(size_input.c_str()), atof(value_input.c_str())));
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
                } else
                    showing_pop_up=false;
            }
        }
    }
}

//Check if mouse clicked buttons
bool checkForButtonsClick(int x, int y){
    for(int i=0; i<buttons.size(); i++){
        if(buttons[i].isPointInside(x, y)){
            std::cout<<"Clicked button \""<<buttons[i].name<<"\""<<std::endl;
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
				mainField.clear();
				mainField.update();
            } else if(i==4){
                isEqPotLines=false;
                isElFieldLines=true;
                isElFieldColor=false;
				mainField.clear();
				mainField.update();
            } else if(i==5){
                isEqPotLines=false;
                isElFieldLines=false;
                isElFieldColor=true;
				mainField.clear();
				mainField.update();
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
