/*
*	Author:	Rodrigo Miguel Santos Rodrigues
*	Nº:		2022233032
*/

#include "ofApp.h"

#include "Snake.h"
#include "Food.h"
#include "cg_extras.h"
#include "cg_drawing_extras.h"

using namespace std;

/**
* @brief Sets up the initial state of the application.
* 
* This function initializes the snake at the center of the screen and creates the first food item.
* The first food item will always be red and will increase the snake's size.
*/
void ofApp::setup(){
	snake = new Snake(ofVec3f(gw()/2, gh()/2, 0));	// Create a new snake at the center of the screen

	/*
	*	Creates first food. The first food will always be red and will increase the snake's size.
	*/

	int food_x = random_number(FOOD_SIZE/2, gw()-FOOD_SIZE/2);
	int food_y = random_number(FOOD_SIZE/2, gh()-FOOD_SIZE/2);
	food = new Food(ofVec3f(food_x, food_y, 0), GROWTH, RED);
}
/**
* @brief Updates the game state.
* 
* This function is called to update the state of the game. It handles the movement of the snake,
* checks for collisions with itself and with food, and pauses the game if necessary.
* 
* - If the game is not paused, the snake moves and its last direction is recorded.
* - Collision detection is performed to check if the snake has collided with itself.
* - If a collision is detected, the game is paused.
* - Food collision detection is performed to check if the snake has eaten the food.
* - If the snake eats the food, the food is consumed, and a new food item is generated at a random position.
* 
* If the game is paused, the snake's direction is maintained, and the game remains in a paused state.
*/
void ofApp::update(){
	/*	Snake Movement	*/
	if(GAME_PAUSED != 1){
		snake->move();
		last_direction = snake->direction;

		/*	Collision Detection	*/
		if(check_snake_collision()){
			GAME_PAUSED = 1;
		}

		/*	Food Collision	*/
		if(check_food_collision()){
			// Eat the food
			snake->food_eaten(food->get_type());
			
			// Generate a new food
			int food_x = random_number(FOOD_SIZE/2, gw()-FOOD_SIZE/2);
			int food_y = random_number(FOOD_SIZE/2, gh()-FOOD_SIZE/2);
			food->set_position(ofVec3f(food_x, food_y, 0)); 
		}
	}

	if(GAME_PAUSED == 1){
		// Pause the game
		snake->tail[0] = snake->tail[0];
		snake->direction = last_direction;
	}
}
/**
* @brief Draws the game elements on the screen.
*
* This function is responsible for rendering the game elements, including
* a wireframe cube, the snake, and the food. It sets the color to white,
* switches to wireframe mode, and applies transformations to position and
* scale the cube. After drawing the cube, it calls the draw methods of the
* snake and food objects to render them on the screen.
*/
void ofApp::draw(){
	glColor3f(1.0f, 1.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPushMatrix();
		glTranslatef(gw()/2, gh()/2, 0);
		glScalef(gw(), gh(), 1);
		cube_unit();
	glPopMatrix();

	snake->draw();			// Draw the snake
	food->draw_food();		// Draw the food

}
/**
* Handles key press events to control the snake's movement, pause/unpause the game,
* exit the game, and toggle fullscreen mode.
*
* @param key The key code of the pressed key.
*
* Key mappings:
* - Movement keys:
*   - Up: OF_KEY_UP, 'w'
*   - Down: OF_KEY_DOWN, 's'
*   - Left: OF_KEY_LEFT, 'a'
*   - Right: OF_KEY_RIGHT, 'd'
* - 'p': Pause/Unpause the game
* - OF_KEY_ESC: Exit the game
* - OF_KEY_F12: Toggle fullscreen mode
*/
void ofApp::keyPressed(int key){
	switch(key){
	// Movement keys
	case OF_KEY_UP:
	case 'w':
		if(snake->direction == DOWN || snake->direction == UP){
			break;
		}
		snake->direction = UP;
		break;
	case OF_KEY_DOWN:
	case 's':
		if(snake->direction == UP || snake->direction == DOWN){
			break;
		}
		snake->direction = DOWN;
		break;
	case OF_KEY_LEFT:	
	/*
	*	Snake movement logic
	*/
	case 'a':
		if(snake->direction == RIGHT || snake->direction == LEFT){
			break;
		}
		snake->direction = LEFT;
		break;
	case OF_KEY_RIGHT:
	case 'd':
		if(snake->direction == LEFT || snake->direction == RIGHT){
			break;
		}
		snake->direction = RIGHT;
		break;
	// end of movement keys

	case 'p':
		// Pause / Unpause the game
		if(GAME_PAUSED == 0){
			GAME_PAUSED = 1;
		} else{
			GAME_PAUSED = 0;
		}
		break;
	case OF_KEY_ESC:
		// Exit the game
		ofExit();
		break;
	case OF_KEY_F12:
		// Switch between fullscreen and windowed mode
		last_witdh = gw(), last_height = gh();
		toggleDisplayMode();
		ofToggleFullscreen();
		break;
	}	
}
void ofApp::keyReleased(int key){}
void ofApp::mouseMoved(int x, int y ){}
void ofApp::mouseDragged(int x, int y, int button){}
void ofApp::mousePressed(int x, int y, int button){}
void ofApp::mouseReleased(int x, int y, int button){}
void ofApp::mouseEntered(int x, int y){}
void ofApp::mouseExited(int x, int y){}
/**
* @brief Handles the window resize event.
* 
* This function is called whenever the window is resized. It checks if the display mode has changed
* since the last resize event. If the display mode has changed, it resizes the snake and food objects
* to the previous width and height, and updates the last display mode to the current display mode.
* 
* @param w The new width of the window.
* @param h The new height of the window.
*/
void ofApp::windowResized(int w, int h){
	if(last_display_mode != display_mode){
		snake->resize(last_witdh, last_height);
		food->resize(last_witdh, last_height);
		last_display_mode = display_mode;
	}
	return;
}
void ofApp::gotMessage(ofMessage msg){}
void ofApp::dragEvent(ofDragInfo dragInfo){}
/**
* @brief Toggles the display mode between windowed and fullscreen.
*
* This function switches the display mode of the application. If the current
* display mode is windowed, it changes to fullscreen. If the current display
* mode is fullscreen, it changes to windowed.
*/
void ofApp::toggleDisplayMode(){
	if(display_mode == WINDOWED){
		display_mode = FULLSCREEN;
	} else{
		display_mode = WINDOWED;
	}
}
/**
* @brief Checks if the snake has collided with the boundaries of the game window.
* 
* This function retrieves the current position of the snake and checks if it has 
* collided with the edges of the game window. The collision is determined based on 
* the snake's size and the dimensions of the game window.
* 
* @return true if the snake has collided with the boundaries, false otherwise.
*/
bool ofApp::check_snake_collision(){
	ofVec3f snake_pos = snake->head;
	if(
		snake_pos.x < SNAKE_SIZE/2 || snake_pos.x > gw()-SNAKE_SIZE/2 || 
		snake_pos.y < SNAKE_SIZE/2 || snake_pos.y > gh()-SNAKE_SIZE/2
	){
		Direction current_direction = snake->direction;
		ofVec3f current_position = snake->head;

		switch(current_direction){
		case UP:
			snake->head = ofVec3f(current_position.x, SNAKE_SIZE/2, 0);
			break;
		case DOWN:
			snake->head = ofVec3f(current_position.x, gh()-SNAKE_SIZE/2, 0);
			break;
		case LEFT:
			snake->head = ofVec3f(SNAKE_SIZE/2, current_position.y, 0);
			break;
		case RIGHT:
			snake->head = ofVec3f(gw()-SNAKE_SIZE/2, current_position.z, 0);
			break;
		case NONE:
		default:
			break;
		}
		return true;
	}
	return false;
}
/**
* @brief Checks if the snake's head has collided with the food.
* 
* This function calculates the distance between the snake's head position
* and the food's position. If the distance is less than the defined 
* SNAKE_SIZE, it indicates a collision.
* 
* @return true if the snake's head has collided with the food, false otherwise.
*/
bool ofApp::check_food_collision(){
	ofVec3f snake_pos = snake->head;
	ofVec3f food_pos = food->get_position();
	if(snake_pos.distance(food_pos) < SNAKE_SIZE){
		// Collision detected
		return true;
	}
	return false;
}
/**
* Generates a random integer between the specified minimum and maximum values (inclusive).
*
* @param min The minimum value of the random number range.
* @param max The maximum value of the random number range.
* @return A random integer between min and max (inclusive).
*/
int ofApp::random_number(int min, int max){
	return min + (rand() % (max - min + 1));
}

// end of ofApp.cpp