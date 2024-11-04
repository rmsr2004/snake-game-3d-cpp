/*
*	Author:	Rodrigo Miguel Santos Rodrigues
*	Nº:		2022233032
*/

#pragma once

#include "ofMain.h"

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
    NONE
} Direction;

/**
* @struct Segment
* @brief Represents a segment of the snake in the game.
*
* This structure holds the position of a segment in 3D space,
* represented by an ofVec3f object.
*/
typedef struct Segment {
    ofVec3f position;

    // Constructor
    Segment(ofVec3f pos) : position(pos) {}
} Segment;

/**
* @struct SnakeBody
* @brief Represents a segment of the snake's body in the snake game.
* 
* This structure is used to link segments of the snake's body together.
* Each segment points to the next segment in the snake's body.
* 
*/
typedef struct {
    Segment* segment;
    Segment* next;
} SnakeBody;

/**
 * @class Snake
 * @brief Represents a snake in the game.
 * 
 * The Snake class manages the snake's head, body, speed, and direction.
 */
class Snake {
    Segment* head;          // Pointer to the head of the snake
    SnakeBody* body;        // The snake's body
    float speed;            // The speed of the snake
    Direction direction;    // The direction of the snake

    public: 
        Snake(ofVec3f pos);
        ~Snake();
        void draw_snake();
        void resize(int w, int h);
        Direction get_direction();
        void set_direction(Direction direction);
        ofVec3f get_position();
        void set_position(ofVec3f position);
        float get_speed();
        void set_speed(float speed);
};

// end of Snake.h