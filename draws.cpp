#include "header.h"
#include <string>
#include <sstream>
#include <math.h>

sf::Font font;
sf::RectangleShape pop_up_rect;

std::vector<Button> buttons;

//Draws everything 
//It's called every FPS by main
void draw(sf::RenderWindow& window){
    drawField(window);
    drawCharges(window);
    drawButtons(window);
    drawPopUp(window);
}

//Draws field in current modality (equipotential lines, field lines, field color)
void drawField(sf::RenderWindow& window){

	std::vector<sf::Vertex> toDrawPixels;
	if(isEqPotLines){
		for(int i=0; i<mainField.eqPotLines.size(); i+=lineDrawingReduction){
		    for(int y=0; y<mainField.eqPotLines[i].pointsCoord.size(); y++){
		        sf::Vertex pixel(sf::Vector2f(mainField.eqPotLines[i].pointsCoord[y].x, mainField.eqPotLines[i].pointsCoord[y].y), sf::Color::White);
			toDrawPixels.push_back(pixel);
		    }
		}
	} else if(isElFieldLines){
		for(int i=0; i<mainField.elFieldLines.size(); i++){
		    for(int y=0; y<mainField.elFieldLines[i].pointsCoord.size(); y++){
				sf::Vertex pixel(sf::Vector2f(mainField.elFieldLines[i].pointsCoord[y].x, mainField.elFieldLines[i].pointsCoord[y].y), sf::Color::White);
				toDrawPixels.push_back(pixel);
		    }
		}
		//Draws arrows
		for(int i=0; i<mainField.arrows.size(); i++)
			window.draw(&mainField.arrows[i][0], mainField.arrows[i].size(), sf::Triangles);
	} else if(isElFieldColor){
		double colorValue;
		double fadedMax = mainField.maxElFieldIntensity*COLOR_FADE;
		for(int x=0; x<windowWidth; x++){
			for(int y=0; y<windowHeight; y++){
				if(mainField.elFieldIntensityMap[x][y]>fadedMax)
					colorValue = 255;
				else
					colorValue = ((mainField.elFieldIntensityMap[x][y]-mainField.minElFieldIntensity)/(fadedMax-mainField.minElFieldIntensity))*255;
				sf::Vertex pixel(sf::Vector2f(x,y), sf::Color(255,255,255,(int)colorValue));
				toDrawPixels.push_back(pixel);
			}		
		}
	}
	window.draw(&toDrawPixels[0], toDrawPixels.size(), sf::Points);
}

//Draws charges
void drawCharges(sf::RenderWindow& window){

    for(int i=0; i<mainField.qCharges.size(); i++)
		mainField.qCharges[i]->drawCharge(window);
}

//Draws buttons
void drawButtons(sf::RenderWindow& window){

    for(int i=0; i<buttons.size(); i++){

        sf::RectangleShape rectButton(sf::Vector2f(buttons[i].width, buttons[i].height));
        rectButton.setPosition(buttons[i].x, buttons[i].y);
        rectButton.setOutlineThickness(-10);
		
        if((i==0 && adding_charge) || (i==1 && removing_charge) || (i==2 && selecting_charge) || (i==3 && isEqPotLines) || (i==4 && isElFieldLines) || (i==5 && isElFieldColor) || (i==6 && shapingCircle) || (i==7 && shapingSquare) || (i==8 && shapingCustom)){
            rectButton.setFillColor(redButtonOn);
            rectButton.setOutlineColor(redBoundsButtonOn);
        } else {
            rectButton.setFillColor(orangeButtonOff);
            rectButton.setOutlineColor(orangeBoundsButtonOff);
        }
        
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
        else if (i==6)
            buttText1.setPosition(buttons[i].x+(buttonsSize/3.8), buttons[i].y+(buttonsSize/4.8));
        else if (i==7)
            buttText1.setPosition(buttons[i].x+(buttonsSize/4.7), buttons[i].y+(buttonsSize/4.8));
        else if (i==8)
            buttText1.setPosition(buttons[i].x+(buttonsSize/5.5), buttons[i].y+(buttonsSize/4.8));
		else
            buttText1.setPosition(buttons[i].x+(buttonsSize/4), buttons[i].y+(buttonsSize/4.8));

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

//Draws the pop-up
void drawPopUp(sf::RenderWindow& window){
    if(showing_pop_up){
        
        pop_up_rect.setFillColor(sf::Color::White);
        pop_up_rect.setOutlineColor(sf::Color::Red);
        pop_up_rect.setOutlineThickness(-10);
        
        //draws painting pop-up
        if(adding_charge && inputing_custom){
            pop_up_rect.setOutlineThickness(10);
            window.draw(pop_up_rect);

        	sf::RectangleShape titleRect(sf::Vector2f(200, 40));
            titleRect.setFillColor(sf::Color::White);
            titleRect.setOutlineColor(sf::Color::Red);
			titleRect.setPosition(300, pop_up_rect.getPosition().y-60);
        	titleRect.setOutlineThickness(5);
			window.draw(titleRect);
			
			sf::Text pop_title;
            pop_title.setCharacterSize(16);
            pop_title.setFillColor(sf::Color::Black);
            pop_title.setFont(font);
            pop_title.setStyle(sf::Text::Bold);
            pop_title.setString("Painting Size = "+std::to_string(mouse_draw_size));
            pop_title.setPosition(335, pop_up_rect.getPosition().y-58);
			window.draw(pop_title);

			sf::Text pop_subtitle;
            pop_subtitle.setCharacterSize(13);
            pop_subtitle.setFillColor(sf::Color::Black);
            pop_subtitle.setFont(font);
            pop_subtitle.setString("Change with Keyboard Numbers");
            pop_subtitle.setPosition(303, pop_up_rect.getPosition().y-38);
			window.draw(pop_subtitle);
			
	    	std::vector<sf::Vertex> shapePixels;
            for(int i=0; i<currentCustom->shape_map.size(); i++){
                sf::Vertex pixel(sf::Vector2f(currentCustom->shape_map[i].x+pop_up_rect.getPosition().x, currentCustom->shape_map[i].y+pop_up_rect.getPosition().y), greyCharge);
				shapePixels.push_back(pixel);
            }
			window.draw(&shapePixels[0], shapePixels.size(), sf::Points);
			
        } else window.draw(pop_up_rect);
        
        //draws adding charge input pop-up
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
        
        //draws selecting pop up
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
            pop_value.setString("Value = " + std::to_string(mainField.qCharges[selected_index]->value));
            pop_value.setPosition(pop_up_rect.getPosition().x+20, pop_up_rect.getPosition().y+pop_up_rect.getSize().y/4+(pop_up_rect.getSize().y/8)*3);
            
            
            window.draw(pop_title);
            window.draw(pop_x);
            window.draw(pop_y);
            window.draw(pop_size);
            window.draw(pop_value);
        }
    }
}
