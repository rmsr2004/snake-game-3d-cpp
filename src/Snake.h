/*
*	Author:	Rodrigo Miguel Santos Rodrigues
*	Nº:		2022233032
*/

#pragma once

#include "ofMain.h"

#define SNAKE_SIZE 25

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
} Direction;

typedef struct Segment {
    ofVec3f position;

    // Constructor
    Segment(ofVec3f pos) : position(pos) {}
} Segment;

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
        Direction get_direction();
        void set_direction(Direction direction);
        ofVec3f get_position();
        void set_position(ofVec3f position);
        float get_speed();
        void set_speed(float speed);
};

// end of Snake.h