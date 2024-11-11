/*
*	Author:	Rodrigo Miguel Santos Rodrigues
*	Nº:		2022233032
*/

#pragma once

#include "ofMain.h"
#include "Snake.h"
#include "Food.h"

#define WINDOW_WIDTH 	1024	// The width of the window
#define WINDOW_HEIGHT 	768		// The height of the window

/**
* @enum DisplayMode
* @brief Enumeration to represent the display mode of the application.
* 
* This enumeration defines the possible display modes for the application.
* It can be either FULLSCREEN or WINDOWED.
*/
typedef enum{
	FULLSCREEN,
	WINDOWED
} DisplayMode;

class ofApp : public ofBaseApp{
	Snake* snake;
	Food* food;

	int GAME_PAUSED = 0;				// Indicates the paused state of the game. (0 = not paused, 1 = paused)
	Direction last_direction = NONE;	// The last direction the snake was moving in.

	int last_witdh = 0;
	int last_height = 0;

	DisplayMode display_mode = WINDOWED;
	DisplayMode last_display_mode = display_mode;

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void toggleDisplayMode();
		bool check_snake_collision();
		bool check_food_collision();
		int random_number(int min, int max);
};

// end of ofApp.h