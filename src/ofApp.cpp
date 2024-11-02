/*
*	Author:	Rodrigo Miguel Santos Rodrigues
*	Nº:		2022233032
*/

#include "ofApp.h"

#include "Snake.h"
#include "cg_extras.h"
#include "cg_drawing_extras.h"

void ofApp::setup(){
	snake = new Snake(ofVec3f(gw()/2, gh()/2, 0));	// Create a new snake at the center of the screen
}

void ofApp::update(){
	/*
	*	Snake movement logic
	*/

	if(GAME_PAUSED != 1){
		if(snake->get_direction() == UP)
			snake->set_position(snake->get_position() + ofVec3f(0, -1, 0));	// Move the snake
		else if(snake->get_direction() == DOWN)
			snake->set_position(snake->get_position() + ofVec3f(0, 1, 0));	// Move the snake
		else if(snake->get_direction() == LEFT)
			snake->set_position(snake->get_position() + ofVec3f(-1, 0, 0));	// Move the snake
		else if(snake->get_direction() == RIGHT)
			snake->set_position(snake->get_position() + ofVec3f(1, 0, 0));	// Move the snake

		last_direction = snake->get_direction();
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
		glScalef(gw()-SNAKE_SIZE, gh()-SNAKE_SIZE, 1);
		malha_unit(gw()/SNAKE_SIZE, gh()/SNAKE_SIZE);
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
	case OF_KEY_LEFT:
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

void ofApp::windowResized(int w, int h){}

void ofApp::gotMessage(ofMessage msg){}

void ofApp::dragEvent(ofDragInfo dragInfo){}

// end of ofApp.cpp