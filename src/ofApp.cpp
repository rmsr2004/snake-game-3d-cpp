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

#define MAX_X 	gw()-SNAKE_SIZE		// The maximum x-coordinate of the game window
#define MAX_Y 	gh()-SNAKE_SIZE		// The maximum y-coordinate of the game window

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

	food = create_food();
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
	if(GAME_OVER == 1){
		// Game Over
		snake->is_snake_visible = true;
		cout << "Game Over" << endl;
		return;
	}

	if(GAME_PAUSED != 1){
		if(snake->is_effect){
			/*
			*	Verify if the snake has an effect and if the effect time has passed.
			* 	If the effect time has passed, the snake's speed is restored to its original value.
			*/
			if(ofGetElapsedTimef() - snake->effect_time > 5){
				snake->speed = snake->aux_speed;
				snake->is_effect = false;

				// If the snake is invisible, make it visible again
				if(snake->is_snake_visible == false){
					snake->is_snake_visible = true;
				}

				cout << "Effect time passed" << endl;
			}
		}

		snake->move();
		last_direction = snake->direction;

		/*	Collision Detection	*/
		if(check_snake_collision()){
			GAME_OVER = 1;
		}

		if(check_tail_collision()){
			GAME_OVER = 1;
		}

		/*	Food Collision	*/
		if(check_food_collision()){
			// Eat the food
			snake->food_eaten(food->get_type(), &score);
			
			// Generate a new food
			food = create_food();
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
	// Draw the world
	glColor3f(1.0f, 1.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPushMatrix();
		glTranslatef(gw()/2, gh()/2, 0);
		glScalef(MAX_X, MAX_Y, 1);
		cube_unit();
	glPopMatrix();

	snake->draw();			// Draw the snake
	food->draw_food();		// Draw the food
	draw_score();			// Draw the score
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
*   - 'p': Pause/Unpause the game
*	- 'r': Restart the game
*   - OF_KEY_ESC: Exit the game
*   - OF_KEY_F12: Toggle fullscreen mode
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
	case 'r':
		if(GAME_OVER == 1){
			// Restart the game
			GAME_OVER = 0;
			setup();
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
* @brief Creates a new food item at a random position not occupied by the snake.
* 
* This function generates a new food item with a random position and type. The position
* is chosen such that it does not overlap with any part of the snake's tail. The type of 
* food is determined based on probabilities that change with the score to increase the 
* difficulty of the game. The food type probabilities are:
* - GROWTH: 85% (decreases with score)
* - SUPER_SPEED: 5% (increases with score)
* - SUPER_SLOWDOWN: 5% (increases with score)
* - INVISIBLE: 5% (increases with score)
* 
* @return A pointer to the newly created Food object.
*/
Food* ofApp::create_food(){
	// Get a position that is not occupied by the snake
	int food_x = random_number(FOOD_SIZE/2, MAX_X-FOOD_SIZE/2);
	int food_y = random_number(FOOD_SIZE/2, MAX_Y-FOOD_SIZE/2);
	ofVec3f food_position = ofVec3f(food_x, food_y, 0);

	if(snake->tail.size() > 0){
		for(size_t i = 0; i < snake->tail.size(); i++){
			while(snake->tail[i] == food_position){
				food_x = random_number(FOOD_SIZE/2, MAX_X-FOOD_SIZE/2);
				food_y = random_number(FOOD_SIZE/2, MAX_Y-FOOD_SIZE/2);
				food_position = ofVec3f(food_x, food_y, 0);
			}
		}
	}

	// Food is not on the snake
	// Generate a random food type and color
	// Food type must has probabilities (GROWTH(85) > SUPER_SPEED(5) > SUPER_SLOWDOWN(5) > INVISIBLE(5))
	// Probabilities must change with the score to be more difficult
	// Food color must be random

	int growth_prob = 85, super_speed_prob = 5, super_slowdown_prob = 5, invisible_prob = 5;

	int difficulty_factor = score/10; 

	growth_prob = max(55, growth_prob - difficulty_factor);
	super_speed_prob += difficulty_factor/3;
	super_slowdown_prob += difficulty_factor/3;
	invisible_prob += difficulty_factor/3;

    int total_prob = growth_prob + super_speed_prob + super_slowdown_prob + invisible_prob;
    int random_value = random_number(0, total_prob - 1);

	FoodType type;
    if(random_value < growth_prob){
        type = GROWTH;
    } else if (random_value < growth_prob + super_speed_prob) {
        type =  SUPER_SPEED;
    } else if (random_value < growth_prob + super_slowdown_prob) {
        type = SUPER_SLOWDOWN;
    } else {
        type = INVISIBLE;
    }

	int color_index = random_number(0, 4);

	return new Food(food_position, type, color_index);
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
	if(snake_pos.x < SNAKE_SIZE || snake_pos.x > MAX_X || snake_pos.y < SNAKE_SIZE || snake_pos.y > MAX_Y){
		Direction current_direction = snake->direction;
		ofVec3f current_position = snake->head;

		switch(current_direction){
		case UP:
			snake->head = ofVec3f(current_position.x, SNAKE_SIZE, 0);
			break;
		case DOWN:
			snake->head = ofVec3f(current_position.x, MAX_Y, 0);
			break;
		case LEFT:
			snake->head = ofVec3f(SNAKE_SIZE, current_position.y, 0);
			break;
		case RIGHT:
			snake->head = ofVec3f(MAX_X, current_position.y, 0);
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
* @brief Checks if the snake's head has collided with its tail.
*
* This function iterates through the segments of the snake's tail and checks 
* if the distance between the head and any tail segment is less than the 
* defined SNAKE_SIZE. If such a collision is detected, the function returns true.
*
* @return true if the snake's head has collided with its tail, false otherwise.
*/
bool ofApp::check_tail_collision(){
	if(snake->tail.empty()){
		return false;
	}

	for(size_t i = 0; i < snake->tail.size(); i++){
		if(snake->head == snake->tail[i]){
			return true;
		}
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
* @brief Draws the current score on the screen.
*
* This function sets the drawing color to white and displays the current score
* at a fixed position on the screen using a bitmap string.
*/
void ofApp::draw_score(){
	ofSetColor(255, 255, 255);
	ofDrawBitmapString("Score: " + to_string(score), 20, 30);
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