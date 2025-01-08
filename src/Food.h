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
const FoodColor YELLOW(1.0f, 1.0f, 0.0f);

class Food {
    public:
        ofVec3f position;
        FoodType type;
        FoodColor color;

        Food(ofVec3f position, FoodType type, int color_index);

        void draw_food(int dimension);
        void resize(int w, int h);
};

// end of food.h