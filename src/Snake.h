/*
*	Author:	Rodrigo Miguel Santos Rodrigues
*	Nº:		2022233032
*/

#ifndef SNAKE_H
#define SNAKE_H

#include "ofMain.h"
#include "Food.h"

#define SNAKE_SIZE 25

/**
* @enum Direction
* @brief Represents the possible directions for the snake's movement.
* 
* Defines the possible directions that the snake can move in the game.
*/
typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    FORWARD,
    BACKWARD,
    NONE
} Direction;

/**
* @class Snake
* @brief Represents a snake in the game.
* 
* The Snake class handles the snake's position, movement, and growth.
*/
class Snake {
    public: 
        ofVec3f head;           // The position of the snake's head
        vector<ofVec3f> tail;   // The positions of the snake's tail
        float speed;            // The speed of the snake
        Direction direction;    // The direction of the snake

        float aux_speed;        // The speed of the snake before the food effect
        bool is_effect;         // Indicates if the snake has an effect
        float effect_time;      // The time the effect lasts

        bool is_snake_visible;  // Indicates if the snake is visible

        Snake(ofVec3f pos);
        void draw(int dimension);
        void draw_head(int dimension);
        void draw_tail(int dimension);
        void resize(int w, int h);
        void move();
        void food_eaten(FoodType type, int* score);
        void grow();
        ofVec3f get_direction_vector();
};

#endif // SNAKE_H

// end of Snake.h