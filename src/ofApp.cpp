/*
*	Author:	Rodrigo Miguel Santos Rodrigues
*	Nº:		2022233032
*/

#include "ofApp.h"

#include "Snake.h"
#include "cg_extras.h"
#include "cg_drawing_extras.h"

using namespace std;

void ofApp::setup(){
	snake = new Snake(ofVec3f(gw()/2, gh()/2, 0));	// Create a new snake at the center of the screen
}

void ofApp::update(){
	/*	Snake Movement	*/
	if(GAME_PAUSED != 1){
		snake->move();
		last_direction = snake->get_direction();

		if(check_collision()){
			GAME_PAUSED = 1;

			Direction current_direction = snake->get_direction();
			ofVec3f current_position = snake->get_position();
			switch(current_direction){
			case UP:
				snake->set_position(ofVec3f(current_position.x, SNAKE_SIZE/2, 0));
				break;
			case DOWN:
				snake->set_position(ofVec3f(current_position.x, gh()-SNAKE_SIZE/2, 0));
				break;
			case LEFT:
				snake->set_position(ofVec3f(SNAKE_SIZE/2, current_position.y, 0));
				break;
			case RIGHT:
				snake->set_position(ofVec3f(gw()-SNAKE_SIZE/2, current_position.y, 0));
				break;
			case NONE:
				break;
			default:
				break;
			}
		}
	}

	if(GAME_PAUSED == 1){
		// Pause the game
		snake->set_position(snake->get_position());
		snake->set_direction(last_direction);
	}
}

void ofApp::draw(){
	glColor3f(1.0f, 1.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPushMatrix();
		glTranslatef(gw()/2, gh()/2, 0);
		glScalef(gw(), gh(), 1);
		cube_unit();
	glPopMatrix();

	snake->draw_snake();	// Draw the snake
}

void ofApp::keyPressed(int key){
	switch(key){
	// Movement keys
	case OF_KEY_UP:
	case 'w':
		if(snake->get_direction() == DOWN || snake->get_direction() == UP){
			break;
		}
		snake->set_direction(UP);
		break;
	case OF_KEY_DOWN:
	case 's':
		if(snake->get_direction() == UP || snake->get_direction() == DOWN){
			break;
		}
		snake->set_direction(DOWN);
		break;
	case OF_KEY_LEFT:	/*
	*	Snake movement logic
	*/
	case 'a':
		if(snake->get_direction() == RIGHT || snake->get_direction() == LEFT){
			break;
		}
		snake->set_direction(LEFT);
		break;
	case OF_KEY_RIGHT:
	case 'd':
		if(snake->get_direction() == LEFT || snake->get_direction() == RIGHT){
			break;
		}
		snake->set_direction(RIGHT);
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

void ofApp::windowResized(int w, int h){
	if(last_display_mode != display_mode){
		snake->resize(last_witdh, last_height);
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
bool ofApp::check_collision(){
	ofVec3f snake_pos = snake->get_position();
	if(
		snake_pos.x < SNAKE_SIZE/2 || snake_pos.x > gw()-SNAKE_SIZE/2 || snake_pos.y < SNAKE_SIZE/2 
		|| snake_pos.y > gh()-SNAKE_SIZE/2
	){
		return true;
	}
	return false;
}

// end of ofApp.cpp