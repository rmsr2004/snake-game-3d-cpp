/*
*	Author:	Rodrigo Miguel Santos Rodrigues
*	Nº:		2022233032
*/

#include "Snake.h"

#include <iostream>
#include "ofApp.h"
#include "cg_drawing_extras.h"
#include "cg_extras.h"

using namespace std;

#define NULL nullptr

/**
 * @brief Constructs a new Snake object with the specified initial position.
 * 
 * @param x The x-coordinate of the snake's initial position.
 * @param y The y-coordinate of the snake's initial position.
 * @param z The z-coordinate of the snake's initial position.
 */
Snake::Snake(ofVec3f pos) {
    head = new Segment(pos);
    speed = 1.0f;
    direction = NONE;
}

/**
 * @brief Destructor for the Snake class.
 * 
 * This destructor is responsible for cleaning up the dynamically allocated memory
 * for the head of the snake. It ensures that the memory allocated for the head
 * is properly released to prevent memory leaks.
 */
Snake::~Snake() {
    delete head;
}

/**
 * @brief Draws the snake on the screen.
 * 
 * This function sets the color and polygon mode for drawing the snake.
 * It then applies transformations to position and scale the snake's head
 * before drawing it using the cube_unit() function.
 * 
 * The snake is drawn in white color and in wireframe mode.
 * 
 * @note This function assumes that the head of the snake is a pointer to a 
 *       structure containing the x, y, and z coordinates of the snake's head.
 */
void Snake::draw_snake(){
    glColor3f(1.0f, 1.0f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    glPushMatrix();
        glTranslatef(head->position.x, head->position.y, head->position.z);
        glScalef(SNAKE_SIZE, SNAKE_SIZE, 0);
        cube_unit();
    glPopMatrix();
}

/**
* @brief Resizes the snake's position based on the new width and height.
*
* This function adjusts the snake's position proportionally to the change in 
* the width and height of the game area. It calculates the scaling factors 
* for both the x and y dimensions and applies these factors to the current 
* position of the snake to determine its new position.
*
* @param w The new width of the game area.
* @param h The new height of the game area.
*/
void Snake::resize(int w, int h){
    int width = gw(), height = gh();

    ofVec3f current_position = get_position();

    double factor_x = static_cast<double>(width) / static_cast<double>(w);
    double factor_y = static_cast<double>(height) / static_cast<double>(h);

    int new_x = current_position.x * factor_x;
    int new_y = current_position.y * factor_y;
    
    set_position(ofVec3f(new_x, new_y, 0));
}

/**
* @brief Moves the snake in the current direction.
*
* This function updates the position of the snake based on its current direction.
* The direction can be UP, DOWN, LEFT, or RIGHT, and the position is updated
* accordingly by adding a unit vector in the respective direction.
*/
void Snake::move(){
    if(get_direction() == UP)
        set_position(get_position() + ofVec3f(0, -1, 0));	
    else if(get_direction() == DOWN)
        set_position(get_position() + ofVec3f(0, 1, 0));	
    else if(get_direction() == LEFT)
        set_position(get_position() + ofVec3f(-1, 0, 0));	
    else if(get_direction() == RIGHT)
        set_position(get_position() + ofVec3f(1, 0, 0));	
}

/**
* @brief Gets the current direction of the snake.
* 
* @return Direction The current direction of the snake.
*/
Direction Snake::get_direction(){
    return this->direction;
}

/**
* @brief Sets the direction of the snake.
* 
* This function updates the direction of the snake to the specified direction.
* 
* @param dir The new direction to set for the snake.
*/
void Snake::set_direction(Direction direction){
    this->direction = direction;
}

/**
 * @brief Retrieves the current position of the snake's head.
 * 
 * @return ofVec3f The 3D vector representing the position of the snake's head.
 */
ofVec3f Snake::get_position(){
    return ofVec3f(head->position.x, head->position.y, head->position.z);
}

/**
 * @brief Sets the position of the snake's head.
 * 
 * This function updates the position of the snake's head to the specified coordinates.
 * 
 * @param position The new position for the snake's head, represented as an ofVec3f object.
 */
void Snake::set_position(ofVec3f position){
    this->head->position = position;
}

/**
 * @brief Retrieves the current speed of the snake.
 * 
 * @return float The current speed of the snake.
 */
float Snake::get_speed(){
    return this->speed;
}

/**
 * @brief Sets the speed of the snake.
 * 
 * This function sets the speed of the snake to the specified value.
 * 
 * @param speed The new speed value for the snake.
 */
void Snake::set_speed(float speed){
    this->speed = speed;
}

// end of Snake.cpp