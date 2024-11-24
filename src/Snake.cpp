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
* @param pos The initial position of the snake's head.
*/
Snake::Snake(ofVec3f pos){
    head.set(pos);
    tail.clear();
    speed = 2.f;
    direction = NONE;
    is_effect = false;
    is_snake_visible = true;
}
/**
* @brief Draws the snake on the screen.
* 
* This function is responsible for drawing the snake's head and tail.
* It first calls the draw_head() function to draw the head of the snake.
* If the snake has more than one segment in its tail, it then calls the 
* draw_tail() function to draw the rest of the snake's body.
*/
void Snake::draw(int dimension){
    if(is_snake_visible){
        draw_head(dimension);
        draw_tail(dimension);
    }
}
/**
* @brief Draws the head of the snake.
*
* This function sets the color to white and fills the polygon mode. It then
* translates and scales the head of the snake to the appropriate position and
* size before drawing it using the cube_unit function.
*/
void Snake::draw_head(int dimension){
    glColor3f(0.0f, 0.45f, 0.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glPushMatrix();
        glTranslatef(head.x, head.y, head.z);
        if(dimension == 0){     // 2D
            glScalef(SNAKE_SIZE, SNAKE_SIZE, 0);
        } else {
            glScalef(SNAKE_SIZE, SNAKE_SIZE, SNAKE_SIZE);
        }
        cube_unit();
    glPopMatrix();

    glEnd();
}
/**
* @brief Draws the tail of the snake.
* 
* This function iterates through the segments of the snake's tail and draws each segment as a scaled cube.
* The tail segments are drawn in blue color.
* 
* The function uses OpenGL functions to set the color, polygon mode, and transformations for each tail segment.
* 
* @note The first segment of the tail (index 0) is skipped as it is presumably the head of the snake.
*/
void Snake::draw_tail(int dimension){
    if(tail.empty()) return;
    
    glColor3f(0.0f, 0.5f, 0.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    for(size_t i = 0; i < tail.size(); i++){
        glPushMatrix();
            glTranslatef(tail[i].x, tail[i].y, tail[i].z);
            if(dimension == 0){
                glScalef(SNAKE_SIZE, SNAKE_SIZE, 0);
            } else {
                glScalef(SNAKE_SIZE, SNAKE_SIZE, SNAKE_SIZE);
            }
            cube_unit();
        glPopMatrix();
    }
}
/**
* @brief Resizes the snake's position based on the new width and height.
*
* This function adjusts the snake's position proportionally to the change in 
* the width and height of the game area. It calculates the scaling factors 
* for both the x and y dimensions and applies these factors to the currentent 
* position of the snake to determine its new position.
*
* @param w The new width of the game area.
* @param h The new height of the game area.
*/
void Snake::resize(int w, int h){
    int width = gw(), height = gh();

    ofVec3f current_position = head;

    double factor_x = static_cast<double>(width) / static_cast<double>(w);
    double factor_y = static_cast<double>(height) / static_cast<double>(h);

    int new_x = current_position.x * factor_x;
    int new_y = current_position.y * factor_y;
    
    head = ofVec3f(new_x, new_y, head.z);
    
    // Resize the tail
    if(!tail.empty()){
        for(size_t i = 0; i < tail.size(); i++){
            ofVec3f current_tail = tail[i];
            int new_tail_x = current_tail.x * factor_x;
            int new_tail_y = current_tail.y * factor_y;
            tail[i] = ofVec3f(new_tail_x, new_tail_y, current_tail.z);
        }
    }
}
/**
* @brief Moves the snake in the current direction.
* 
* This function updates the position of the snake's head based on the current direction and speed.
* If the snake has a tail, it updates the positions of the tail segments to follow the head.
*/
void Snake::move(){
    ofVec3f old_head = head;                        // Save the current head position to update the tail
    head = head + get_direction_vector() * speed;   // Move the head

    // Tail Movement
    if(!tail.empty()){
        for(size_t i = tail.size()-1; i > 0; --i){
            tail[i] = tail[i-1];
        }
        tail[0] = old_head;
    }
}
/**
* @brief Handles the effects of eating different types of food.
* 
* This function is called when the snake eats a piece of food. Depending on the type of food eaten,
* it will apply different effects to the snake, such as changing its speed, growing, decreasing in size,
* or becoming invisible.
* 
* @param type The type of food eaten by the snake. It can be one of the following:
* - SUPER_SPEED: Doubles the snake's speed.
* - SUPER_SLOWDOWN: Halves the snake's speed.
* - GROWTH: Increases the snake's length.
* - INVISIBLE: Makes the snake invisible.
*/
void Snake::food_eaten(FoodType type, int* score){
    if(is_effect) return;

    switch(type){
    case SUPER_SPEED:
        if(!is_effect){
            aux_speed = speed;
            is_effect = true;
            speed =  speed * 2;
            effect_time = ofGetElapsedTimef();
            *score += 10;
        }
        break;
    case SUPER_SLOWDOWN:
        if(!is_effect){
            aux_speed = speed;
            is_effect = true;
            speed = speed / 2;
            effect_time = ofGetElapsedTimef();
            *score += 10;
        }
        break;
    case GROWTH:
        if(!is_effect){
            grow();
            speed = speed + .25f;
            *score += 5;
        }
        break;
    case INVISIBLE:
        if(!is_effect){
            is_snake_visible = false;
            is_effect = true;
            aux_speed = speed;
            effect_time = ofGetElapsedTimef();
            *score += 30;
        }
        break;
    default:
        break;
    }
}
/**
* @brief Grows the snake by adding a new segment to its tail.
* 
* This function adds a new segment to the snake's tail. If the tail is not empty,
* it calculates the position of the new segment based on the last segment in the tail
* and the current direction of the snake. If the tail is empty, it adds the new segment
* based on the head's position and the current direction of the snake.
*/
void Snake::grow(){
    if(!tail.empty()){
        ofVec3f aux = tail[tail.size()-1];
        ofVec3f current_direction = get_direction_vector();

        for(int i = 0; i < 5; i++){
            ofVec3f new_segment(
                aux.x + current_direction.x * SNAKE_SIZE, 
                aux.y + current_direction.y * SNAKE_SIZE,
                aux.z + current_direction.z * SNAKE_SIZE
            );
            tail.push_back(new_segment);
        }
    } else {
        ofVec3f new_segment = head + get_direction_vector() * SNAKE_SIZE;
        tail.push_back(new_segment);
    }
}
/**
* @brief Get the direction vector based on the current direction of the snake.
* 
* This function returns a 3D vector representing the direction in which the snake is moving.
* The direction is determined by the `direction` member variable of the Snake class.
* 
* @return ofVec3f A 3D vector representing the direction of the snake.
* - (0, -1, 0) if the direction is UP
* - (0, 1, 0) if the direction is DOWN
* - (-1, 0, 0) if the direction is LEFT
* - (1, 0, 0) if the direction is RIGHT
* - (0, 0, 0) if the direction is NONE or any other undefined value
*/
ofVec3f Snake::get_direction_vector(){
    ofVec3f direction_vector;
    switch(direction){
    case UP:
        direction_vector = ofVec3f(0, -1, 0);
        break;
    case DOWN:
        direction_vector = ofVec3f(0, 1, 0);
        break;
    case LEFT:
        direction_vector = ofVec3f(-1, 0, 0);
        break;
    case RIGHT:
        direction_vector = ofVec3f(1, 0, 0);
        break;
    case FORWARD:
        direction_vector = ofVec3f(0, 0, 1);
        break;
    case BACKWARD:
        direction_vector = ofVec3f(0, 0, -1);
        break;
    case NONE:
    default:
        direction_vector = ofVec3f(0, 0, 0);
        break;
    }
    return direction_vector;
}

// end of Snake.cpp