#include "header.h"
#include <iostream>

const sf::Color greyCharge(128, 128, 128);
const sf::Color redButtonOn(255, 99, 71);
const sf::Color redBoundsButtonOn(255, 69, 0);
const sf::Color orangeButtonOff(255, 165, 0);
const sf::Color orangeBoundsButtonOff(255, 140, 0);

sf::Font font;
sf::RectangleShape pop_up_rect;

std::vector<Button> buttons;

//Draw loop called every FPS
void draw(sf::RenderWindow& window, Field& mainField){
    
    //Drawing equipotential lines
    for(int i=0; i<mainField.eqPotLines.size(); i+=lineDrawingReduction){
        for(int y=0; y<mainField.eqPotLines[i].pointsCoord.size(); y++){
            sf::RectangleShape pixel(sf::Vector2f(1, 1));
            pixel.setFillColor(sf::Color::White);
            pixel.setPosition(mainField.eqPotLines[i].pointsCoord[y].x, mainField.eqPotLines[i].pointsCoord[y].y);
            
            window.draw(pixel);
        }
    }
    
    drawCharges(window, mainField);
    drawButtons(window);
    drawPopUp(window, mainField);
    
}

//Drawing charges
void drawCharges(sf::RenderWindow& window, Field& mainField){
    for(int i=0; i<mainField.qCharges.size(); i++){
        
        if(mainField.qCharges[i]->shape==CIRCLE_SHAPE){
            //WARNING: SFML takes as size radius not diameter in CircleShape
            sf::CircleShape shape(mainField.qCharges[i]->size);
            shape.setFillColor(greyCharge);
            shape.setPosition( mainField.qCharges[i]->x-mainField.qCharges[i]->size , mainField.qCharges[i]->y-mainField.qCharges[i]->size );
            
            window.draw(shape);
        } else if(mainField.qCharges[i]->shape==SQUARE_SHAPE){
            sf::RectangleShape shape(sf::Vector2f(mainField.qCharges[i]->size, mainField.qCharges[i]->size));
            shape.setFillColor(greyCharge);
            shape.setPosition(mainField.qCharges[i]->x , mainField.qCharges[i]->y);
            
            window.draw(shape);
        } else if(mainField.qCharges[i]->shape==CUSTOM_SHAPE){
            int posX = mainField.qCharges[i]->x;
            int posY = mainField.qCharges[i]->y;
            std::shared_ptr<CustomCharge> tempPRT = std::static_pointer_cast<CustomCharge>(mainField.qCharges[i]);
            for(int i=0; i<tempPRT->shape_map.size(); i++){
                sf::RectangleShape shape(sf::Vector2f(1, 1));
                shape.setFillColor(greyCharge);
                shape.setPosition(tempPRT->shape_map[i].x+posX, tempPRT->shape_map[i].y+posY);
                
                window.draw(shape);
            }
        }
    }
}

//Drawing buttons
void drawButtons(sf::RenderWindow& window){
    for(int i=0; i<buttons.size(); i++){
        sf::RectangleShape rectButton(sf::Vector2f(buttons[i].width, buttons[i].height));
        if((i==0 && adding_charge) || (i==1 && removing_charge) || (i==2 && selecting_charge) || (i==3 && isEqPotLines) || (i==4 && isElFieldLines) || (i==5 && isElFieldColor) || (i==6 && shapingCircle) || (i==7 && shapingSquare) || (i==8 && shapingCustom)){
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
        else if (i==8)
            buttText1.setPosition(buttons[i].x+(buttonsSize/5.5), buttons[i].y+(buttonsSize/4.8));
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

//Draw the pop-up
void drawPopUp(sf::RenderWindow& window, Field& mainField){
    if(showing_pop_up){
        
        pop_up_rect.setFillColor(sf::Color::White);
        pop_up_rect.setOutlineColor(sf::Color::Red);
        pop_up_rect.setOutlineThickness(-10);
        
        //draw custom charge drawing pop-up
        if(adding_charge && inputing_custom){
            pop_up_rect.setOutlineThickness(10);
            window.draw(pop_up_rect);
            for(int i=0; i<currentCustom->shape_map.size(); i++){
                sf::RectangleShape pixel(sf::Vector2f(1, 1));
                pixel.setFillColor(greyCharge);
                pixel.setPosition(currentCustom->shape_map[i].x+pop_up_rect.getPosition().x, currentCustom->shape_map[i].y+pop_up_rect.getPosition().y);
                
                window.draw(pixel);
            }
        } else window.draw(pop_up_rect);
        
        //draw add charge input pop-up
        if(adding_charge && !inputing_custom){
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
            pop_x.setString("x = "+std::to_string(mainField.qCharges[selected_index]->x));
            pop_x.setPosition(pop_up_rect.getPosition().x+20, pop_up_rect.getPosition().y+pop_up_rect.getSize().y/4);
            
            sf::Text pop_y;
            pop_y.setCharacterSize(15);
            pop_y.setFillColor(sf::Color::Black);
            pop_y.setFont(font);
            pop_y.setString("y = "+std::to_string(mainField.qCharges[selected_index]->y));
            pop_y.setPosition(pop_up_rect.getPosition().x+20, pop_up_rect.getPosition().y+pop_up_rect.getSize().y/4+(pop_up_rect.getSize().y/8));
            
            sf::Text pop_size;
            pop_size.setCharacterSize(15);
            pop_size.setFillColor(sf::Color::Black);
            pop_size.setFont(font);
            pop_size.setString("Size = "+std::to_string(mainField.qCharges[selected_index]->size));
            pop_size.setPosition(pop_up_rect.getPosition().x+20, pop_up_rect.getPosition().y+pop_up_rect.getSize().y/4+(pop_up_rect.getSize().y/8)*2);
            
            sf::Text pop_value;
            pop_value.setCharacterSize(15);
            pop_value.setFillColor(sf::Color::Black);
            pop_value.setFont(font);
            pop_value.setString("Value = "+std::to_string(mainField.qCharges[selected_index]->value));
            pop_value.setPosition(pop_up_rect.getPosition().x+20, pop_up_rect.getPosition().y+pop_up_rect.getSize().y/4+(pop_up_rect.getSize().y/8)*3);
            
            sf::Text pop_type;
            pop_type.setCharacterSize(15);
            pop_type.setFillColor(sf::Color::Black);
            pop_type.setFont(font);
            pop_type.setString("Type = "+mainField.qCharges[selected_index]->shape);
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
