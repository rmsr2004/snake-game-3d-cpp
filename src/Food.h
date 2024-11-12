/*
*	Author:	Rodrigo Miguel Santos Rodrigues
*	Nº:		2022233032
*/

#pragma once

#include "ofMain.h"

#define FOOD_SIZE 25

/**
* @enum FoodType
* @brief Represents the different types of food available in the snake game.
* 
* This enumeration defines the various effects that food can have on the snake
* when consumed.
*/
typedef enum {
    SUPER_SPEED,
    SUPER_SLOWDOWN,
    GROWTH,
    INVISIBLE,
} FoodType;

/**
* @struct FoodColor
* @brief Represents the color of the food in the snake game.
*
* This structure holds the RGB color values for the food.
*/
struct FoodColor {
    float r;
    float g;
    float b;

    FoodColor(float red, float green, float blue) : r(red), g(green), b(blue) {}
};

/*
* Generation of the different colors of food.
* RED, GREEN, BLUE, YELLOW, BLACK
*/
const FoodColor RED(1.0f, 0.0f, 0.0f);
const FoodColor GREEN(0.0f, 1.0f, 0.0f);
const FoodColor BLUE(0.0f, 0.0f, 1.0f);
const FoodColor YELLOW(1.0f, 1.0f, 0.0f);
const FoodColor BLACK(0.0f, 0.0f, 0.0f);

/**
* @class Food
* @brief Represents a piece of food in the snake game.
*
* The Food class encapsulates the properties and behaviors of a food item
* in the snake game, including its position, type, and color.
*/
class Food {
    ofVec3f position;
    FoodType type;
    FoodColor color;

    public:
        Food(ofVec3f position, FoodType type, int color_index);

        void draw_food();
        void resize(int w, int h);
        void set_position(ofVec3f position);
        ofVec3f get_position();
        FoodType get_type();
};

// end of food.h