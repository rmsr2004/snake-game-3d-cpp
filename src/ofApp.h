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

typedef enum{
	_2D,
	_3D,
	FIRST_PERSON
} Dimension;

class ofApp : public ofBaseApp{
	Snake* snake;
	Food* food;

	int GAME_PAUSED;					// Indicates the paused state of the game. (0 = not paused, 1 = paused)
	int GAME_OVER;						// Indicates the game over state. (0 = not over, 1 = over)
	Direction last_direction;			// The last direction the snake was moving in.

	int score;							// The player's score.

	int last_witdh;
	int last_height;

	DisplayMode display_mode;
	DisplayMode last_display_mode;

	Dimension dimension;

	ofVec3f cam_pos;
	float angleX;
	float angleY;
	float rotation_speedX;
	float rotation_speedY;
	float distance;

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
		Food* create_food();
		bool check_snake_collision();
		bool check_tail_collision();
		bool check_food_collision();
		int random_number(int min, int max);
		void draw_score();
		void update_cam_pos();
};

// end of ofApp.h