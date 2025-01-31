/*
*	Author:	Rodrigo Miguel Santos Rodrigues
*	Nº:		2022233032
*/

#include "Food.h"

#include "cg_drawing_extras.h"

/**
* @brief Constructs a new Food object.
* 
* @param position The position of the food in 3D space.
* @param type The type of the food.
* @param color The color of the food.
*/
Food::Food(ofVec3f position, FoodType type, int color_index) : color(RED) {
    this->position = position;
    this->type = type;

    switch(color_index){
    case 0:
        color = RED;
        break;
    case 1:
        color = GREEN;
        break;
    case 2:
        color = YELLOW;
        break;
    default:
        color = RED;
        break;
    }
}
/**
* @brief Draws the food object on the screen.
* 
* This function sets the color of the food object, configures the polygon mode,
* and applies transformations to position and scale the food object before drawing it.
* 
* The food object is drawn using a unit cube scaled by FOOD_SIZE.
*/
void Food::draw_food(int dimension){
    //glColor3f(color.r, color.g, color.b);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPushMatrix();
        glTranslatef(position.x, position.y, position.z);
        if(dimension == 0){
            glScalef(FOOD_SIZE, FOOD_SIZE, 0);
        } else {
            glScalef(FOOD_SIZE, FOOD_SIZE, FOOD_SIZE);
        }
        cube_unit();
    glPopMatrix();
}
/**
* @brief Resizes the food object based on the new width and height.
* 
* This function adjusts the position of the food object according to the new
* dimensions provided. It calculates the scaling factors for both the x and y
* coordinates and updates the position of the food object accordingly.
* 
* @param w The new width to resize to.
* @param h The new height to resize to.
*/
void Food::resize(int w, int h){
    int width = gw(), height = gh();

    double factor_x = static_cast<double>(width) / static_cast<double>(w);
    double factor_y = static_cast<double>(height) / static_cast<double>(h);

    int new_x = position.x * factor_x;
    int new_y = position.y * factor_y;
    
    position = ofVec3f(new_x, new_y, position.z);
}

// end of Food.cpp