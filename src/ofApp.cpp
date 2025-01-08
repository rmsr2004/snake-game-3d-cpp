/*
*	Author:	Rodrigo Miguel Santos Rodrigues
*	Nº:		2022233032
*/

#include "ofApp.h"

#include "Snake.h"
#include "Food.h"
#include "Lights.h"
#include "Materiais.h"
#include "cg_extras.h"
#include "cg_drawing_extras.h"
#include "cg_cam_extras.h"

using namespace std;

#define MAX_X 	gw()-SNAKE_SIZE		// The maximum x-coordinate of the game window
#define MAX_Y 	gh()-SNAKE_SIZE		// The maximum y-coordinate of the game window
#define MAX_Z   1000				// The maximum z-coordinate of the game window

/**
* @brief Sets up the initial state of the application.
* 
* This function initializes the snake at the center of the screen and creates the first food item.
* The first food item will always be red and will increase the snake's size.
*/
void ofApp::setup(){
	/*
	*	OpenGl and OpenFrameworks setup
	*/
	// Set the display mode
	display_mode = WINDOWED;
	// Resize functionality settings
	last_display_mode = display_mode;
	last_witdh = 0;
	last_height = 0;
	// Set the first dimension
	dimension = _2D;
	// Camera settings
	angleX = 0, angleY = 0;
	rotation_speedX = 0, rotation_speedY = 0;
	distance = 1000;

	ofSetFrameRate(60);
	glEnable(GL_DEPTH_TEST);
	ofBackground(0, 0, 0);

	/*
	*	Snake Game Setup
	*/
	
	snake = new Snake(ofVec3f(gw()/2, gh()/2, 0));	// Create a new snake at the center of the screen
	food = create_food();	// Create the first food
	score = 0;
	GAME_PAUSED = 0;				
	GAME_OVER = 0;						
	last_direction = NONE;

	particles.clear();


	/*
	*	Lighting Setup
	*/

	mat_index = 0;

	point_on = false;
	ambient_on = false;
	spot_on = false;
	point_ambient = true;
	point_diffuse = true;
	point_specular = true;
	dir_ambient = true;
	dir_diffuse = true;
	dir_specular = true;
	spot_ambient = true;
	spot_diffuse = true;
	spot_specular = true;

	customMatCoef = 1;

	lights = new Lights();

	// Set the point light (lamp)
	lights->point_pos[0] = gw()/2;
	lights->point_pos[1] = 0.0;
	lights->point_pos[2] = 0.0;

	lights->point_amb[0] = 1.0;
	lights->point_amb[1] = 1.0;
	lights->point_amb[2] = 1.0;

	lights->point_diff[0] = 1.0;
	lights->point_diff[1] = 1.0;
	lights->point_diff[2] = 1.0;

	lights->point_spec[0] = 1.0;
	lights->point_spec[1] = 1.0;
	lights->point_spec[2] = 1.0;

	// Set the directional light (sun)
	angle = 0.0f;
	radius = 1000.0f;

	lights->dir_pos[0] = -100.0;
	lights->dir_pos[1] = -250.0;
	lights->dir_pos[2] = -100.0;
	lights->dir_pos[3] = 0.0;

	lights->dir_direction[0] = 0.0;
	lights->dir_direction[1] = -1.0;
	lights->dir_direction[2] = -1.0;
	lights->dir_direction[3] = 0.0;

	lights->dir_amb[0] = 1.0;
	lights->dir_amb[1] = 1.0;
	lights->dir_amb[2] = 0.0;

	lights->dir_diff[0] = 1.0;
	lights->dir_diff[1] = 1.0;
	lights->dir_diff[2] = 0.0;

	lights->dir_spec[0] = 1.0;
	lights->dir_spec[1] = 1.0;
	lights->dir_spec[2] = 0.0;

	// Set the spot light (flashlight)
	lights->spot_pos[0] = snake->head.x;
	lights->spot_pos[1] = snake->head.y + 10;
	lights->spot_pos[2] = snake->head.z;
	
	lights->spot_dir[0] = snake->get_direction_vector().x;
	lights->spot_dir[1] = snake->get_direction_vector().y;
	lights->spot_dir[2] = snake->get_direction_vector().z;
	lights->spot_amb[0] = 1.0;
	lights->spot_amb[1] = 1.0;
	lights->spot_amb[2] = 1.0;

	lights->spot_diff[0] = 1.0;
	lights->spot_diff[1] = 1.0;
	lights->spot_diff[2] = 1.0;

	lights->spot_spec[0] = 0.5;
	lights->spot_spec[1] = 0.5;
	lights->spot_spec[2] = 0.;

	lights->spot_cutoff = 60.0;
	lights->spot_exponent = 500.0;
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
		ambient_on = true;
		snake->is_snake_visible = true;
		GAME_OVER = 2;
		int n = 50;
		if(snake->tail.size() > 5){
			n = 100;
		}
		for(int i = 0; i < n; i++){
            Particle aux;
			if(dimension == _2D){
            	aux.setup(
					snake->head.x-SNAKE_SIZE, snake->head.x + SNAKE_SIZE, 
					gh()-snake->head.y-SNAKE_SIZE, gh()-snake->head.y + SNAKE_SIZE, 
					0, 0
				);
			} else if(dimension == _3D){
				aux.setup(
					snake->head.x - SNAKE_SIZE, snake->head.x + SNAKE_SIZE, 
					snake->head.y - SNAKE_SIZE, snake->head.y + SNAKE_SIZE, 
					snake->head.z - SNAKE_SIZE, snake->head.z + SNAKE_SIZE
				);
			} else if(dimension == _PERSPECTIVE){
				aux.setup(
					snake->head.x-SNAKE_SIZE, snake->head.x + SNAKE_SIZE, 
					snake->head.y-SNAKE_SIZE, snake->head.y + SNAKE_SIZE,
					snake->head.z-SNAKE_SIZE, snake->head.z + SNAKE_SIZE
				);	
			}
            particles.push_back(aux);
        }
	}

	if(GAME_OVER == 2){
		for(int i = 0; i < particles.size(); i++){
            particles[i].update();
            if(particles[i].dead){
                particles.erase(particles.begin() + i);
            }
        }
		return;
	}

	if(GAME_PAUSED != 1 && GAME_OVER == 0){
		/*
		*
		*
		*	Game is running....
		*
		*
		*/

		for(int i = 0; i < particles.size(); i++){
            particles[i].update();
            if(particles[i].dead){
                particles.erase(particles.begin() + i);
            }
        }

		// Update camera position if the dimension is 3D
		if(dimension == _3D){
			angleX += rotation_speedX;
			angleY += rotation_speedY;
			update_cam_pos();
		}

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
			}
		}

		snake->move();	// Move the snake
		last_direction = snake->direction;

		// Collisions Detection	
		// Check if the snake has collided with itself or with the tail
		if(check_snake_collision() || check_tail_collision()){
			GAME_OVER = 1;
		}
		// Check if the snake has collided with the food
		if(check_food_collision()){
			// Eat the food
			snake->food_eaten(food->type, &score);

			for(int i = 0; i < 100; i++){
				Particle aux;
				if(dimension == _2D){
					aux.setup(
						snake->head.x-SNAKE_SIZE, snake->head.x + SNAKE_SIZE, 
						gh()-snake->head.y-SNAKE_SIZE, gh()-snake->head.y + SNAKE_SIZE, 
						0, 0
					);
				} else if(dimension == _3D){
					aux.setup(
						snake->head.x - SNAKE_SIZE, snake->head.x + SNAKE_SIZE, 
						snake->head.y - SNAKE_SIZE, snake->head.y + SNAKE_SIZE, 
						snake->head.z - SNAKE_SIZE, snake->head.z + SNAKE_SIZE
					);
				} else if(dimension == _PERSPECTIVE){
					aux.setup(
						snake->head.x-SNAKE_SIZE, snake->head.x + SNAKE_SIZE, 
						snake->head.y-SNAKE_SIZE, snake->head.y + SNAKE_SIZE,
						snake->head.z-SNAKE_SIZE, snake->head.z + SNAKE_SIZE
					);	
				}
				particles.push_back(aux);
			}
			
			// Generate a new food
			food = create_food();
		}

		// update the spot light position and direction
		lights->spot_pos[0] = snake->head.x;
		lights->spot_pos[1] = snake->head.y;
		lights->spot_pos[2] = snake->head.z;

		ofVec3f dir = snake->get_direction_vector().getNormalized();
		
		lights->spot_dir[0] = dir.x;
		lights->spot_dir[1] = dir.y;
		lights->spot_dir[2] = dir.z;

		// updat direction light position
		angle += 0.005f;

		lights->dir_pos[0] = gw()/2 + radius * cos(angle);
		lights->dir_pos[1] = gh()/2 + radius * sin(angle);
		lights->dir_pos[2] = 0;
		lights->dir_pos[3] = 0.0f;

		lights->dir_direction[0] = -cos(angle);
		lights->dir_direction[1] = -sin(angle);
		lights->dir_direction[2] = 0.0f;
		lights->dir_direction[3] = 0.0f;
	}

	if(GAME_PAUSED == 1){
		// Pause the game
		snake->head = snake->head;
		snake->direction = last_direction;
	}
}
/**
* @brief Draws the current frame of the application.
* 
* This function handles the rendering of the game scene based on the current
* dimension mode (_2D, _3D, _PERSPECTIVE, _FIRST_PERSON). It sets up the 
* appropriate OpenGL matrices and viewport, and then draws the game elements 
* including the snake, food, score, and direction arrow.
*/
void ofApp::draw(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glViewport(0, 0, gw(), gh());

	if(show_instructions){
		draw_instructions();
	}

	if(GAME_PAUSED == 1){
		draw_pause();
	}

	for(int i = 0; i<particles.size(); i++){
		loadMaterial(2);
		if(GAME_OVER == 2){
        	particles[i].draw(0.0f, 0.45f, 0.0f);
		} else {
			particles[i].draw(food->color.r, food->color.g, food->color.b);
		}
	}

	if(GAME_OVER == 2){
		draw_game_over();
	}

	if(dimension == _2D){
		// 2d setup
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glOrtho(0, gw(), 0, gh(), -1, 1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	if(dimension == _3D){
		// 3d setup
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		lookat(
			cam_pos.x, cam_pos.y, cam_pos.z,            	// Camera position
			snake->head.x, snake->head.y, snake->head.z,  	// Target position (snake head)
			0, 1, 0                              			// Up vector
		);
	}

	if(dimension == _PERSPECTIVE){
		// Perspective setup
		perspective(60.f, 1.f, 1000.f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		lookat(
			gw()/2, gh()*2, 700, 		// Camera position
			gw()/2, gh(), 0, 			// Target position
			0, 1, 0						// Up vector
		);
	}

	if(dimension == _FIRST_PERSON){
		// First person setup
		//perspective(120, 1, 700);

		//glMatrixMode(GL_MODELVIEW);
		//glLoadIdentity();

		//ofVec3f target;
		//target = snake->head + 50*snake->get_direction_vector();

		//lookat(
		//	snake->head.x, snake->head.y, snake->head.z,	// Camera position (snake head)
		//	target.x, target.y, target.z,					// Target position (snake head + direction)
		//	0, 1, 0											// Up vector
		//);
	}

	// Draw the world
	loadMaterial(16);
	//glColor3f(1.0f, 1.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPushMatrix();
		glTranslatef(gw()/2, gh()/2, 0);
		glScalef(MAX_X, MAX_Y, MAX_Z);
		cube_unit();
	glPopMatrix();
	
	loadMaterial(14);
	snake->draw((int)(dimension));		// Draw the snake
	loadMaterial(19);
	food->draw_food((int)(dimension));	// Draw the food

	// Draw the score
	// The score is drawn in 2D mode to ensure it is always visible
    ofPushMatrix();
		ofSetupScreen();	// Set up the screen for 2D drawing
		draw_score();
    ofPopMatrix();

	// Draw Direction Arrow
	if(dimension != _2D){
		draw_direction_arrow();
	}

	// Draw the lights
	loadMaterial(16);
	lights->draw_light(0, (int)(dimension));	// Draw the point light
	loadMaterial(10);
	lights->draw_light(1, (int)(dimension));	// Draw the directional light

	// Start lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, true);

	if(ambient_on){
		ambientLight[0] = 1.;//R
		ambientLight[1] = 0.5;//G
		ambientLight[2] = 0.5;//B
		ambientLight[3] = 1;//useless
	} else {
		ambientLight[0] = 0.0;
		ambientLight[1] = 0.0;
		ambientLight[2] = 0.0;
		ambientLight[3] = 1.0;
	}

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

	GLfloat nulL_vector[4] = {0.0, 0.0, 0.0, 1.0};

	// Point light
	glLightfv(GL_LIGHT0, GL_POSITION, lights->point_pos);
	if(point_ambient){
		glLightfv(GL_LIGHT0, GL_AMBIENT, lights->point_amb);
	} else {
		glLightfv(GL_LIGHT0, GL_AMBIENT, nulL_vector);
	}
	if(point_diffuse){
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lights->point_diff);
	} else {
		glLightfv(GL_LIGHT0, GL_DIFFUSE, nulL_vector);
	}
	if(point_specular){
		glLightfv(GL_LIGHT0, GL_SPECULAR, lights->point_spec);
	} else {
		glLightfv(GL_LIGHT0, GL_SPECULAR, nulL_vector);
	}

	lights->point_atc = 1;
	lights->point_atl = 0.0001;
	lights->point_atq = 0.00001;
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, lights->point_atc);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, lights->point_atl);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, lights->point_atq);

	if(point_on){
		glEnable(GL_LIGHT0);
	} else {
		glDisable(GL_LIGHT0);
	}

	// Directional light

	glLightfv(GL_LIGHT1, GL_POSITION, lights->dir_direction);
	if(dir_ambient){
		glLightfv(GL_LIGHT1, GL_AMBIENT, lights->dir_amb);
	} else {
		glLightfv(GL_LIGHT1, GL_AMBIENT, nulL_vector);
	}
	if(dir_diffuse){
		glLightfv(GL_LIGHT1, GL_DIFFUSE, lights->dir_diff);
	} else {
		glLightfv(GL_LIGHT1, GL_DIFFUSE, nulL_vector);
	}
	if(dir_specular){
		glLightfv(GL_LIGHT1, GL_SPECULAR, lights->dir_spec);
	} else {
		glLightfv(GL_LIGHT1, GL_SPECULAR, nulL_vector);
	}

	if(dir_on){
		glEnable(GL_LIGHT1);
	} else {
		glDisable(GL_LIGHT1);
	}

	// Spot light

	glLightfv(GL_LIGHT2, GL_POSITION, lights->spot_pos);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, lights->spot_dir);

	if(spot_ambient){
		glLightfv(GL_LIGHT2, GL_AMBIENT, lights->spot_amb);
	} else {
		glLightfv(GL_LIGHT2, GL_AMBIENT, nulL_vector);
	}
	if(spot_diffuse){
		glLightfv(GL_LIGHT2, GL_DIFFUSE, lights->spot_diff);
	} else {
		glLightfv(GL_LIGHT2, GL_DIFFUSE, nulL_vector);
	}
	if(spot_specular){
		glLightfv(GL_LIGHT2, GL_SPECULAR, lights->spot_spec);
	} else {
		glLightfv(GL_LIGHT2, GL_SPECULAR, nulL_vector);
	}
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, lights->spot_exponent);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, lights->spot_cutoff);

	lights->spot_atc = 1.;
	lights->spot_atl = 0.;
	lights->spot_atq = 0.;
	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, lights->spot_atc);
	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, lights->spot_atl);
	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, lights->spot_atq);

	if(spot_on){
		glEnable(GL_LIGHT2);
	} else {
		glDisable(GL_LIGHT2);
	}

	// Set the material properties
	//GLfloat mat_diffuse[] = {0.6f, 0.6f, 0.6f, 1.0f};  // Difusão cinza claro
	//GLfloat mat_ambient[] = {0.3f, 0.3f, 0.3f, 1.0f};  // Componente ambiente
	//GLfloat mat_specular[] = {0.8f, 0.8f, 0.8f, 1.0f}; // Brilho especular
	//GLfloat mat_shininess[] = {50.0f};                 // Brilho intenso
	//
	//glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	//glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	loadMaterial(2);
}
/**
* @brief Handles key press events to control the game.
* 
* @param key The key code of the pressed key.
* 
* This function processes various key presses to control the game's views, snake movement, camera movement, and game state.
* 
* Key mappings:
* - '1': Switch to 2D view.
* - '2': Switch to 3D view.
* - '3': Switch to Perspective view.
* - '4': Switch to First Person view (not implemented yet).
* - OF_KEY_UP: Move the snake up.
* - OF_KEY_DOWN: Move the snake down.
* - OF_KEY_LEFT: Move the snake left.
* - OF_KEY_RIGHT: Move the snake right.
* - 'w': Move the snake forward (3D/Perspective view only).
* - 's': Move the snake backward (3D/Perspective view only).
* - 'i': Rotate camera up (3D view only).
* - 'k': Rotate camera down (3D view only).
* - 'j': Rotate camera left (3D view only).
* - 'l': Rotate camera right (3D view only).
* - 'o': Reset camera position (3D view only).
* - '+': Zoom in (3D view only).
* - '-': Zoom out (3D view only).
* - 'p': Pause/Unpause the game.
* - 'r': Restart the game if it is over.
* - OF_KEY_ESC: Exit the game.
* - OF_KEY_F12: Toggle fullscreen mode.
*/
void ofApp::keyPressed(int key){
	key = tolower(key);
	if(GAME_OVER == 2){
		if(key == 'r'){
			GAME_OVER = 0;
			setup();
		}
		return;
	}
	switch(key){
	// Switch views
	case '1':
		/*
		*	2D View
		*/
		if(dimension == _2D){
			break;
		}
		
		glDisable(GL_CULL_FACE);

		// Fix the direction when changing to 2D
		if(dimension == _3D){
			// Invert snake and food positions
			snake->head = ofVec3f(snake->head.x, gh()-snake->head.y, 0);
			food->position = ofVec3f(food->position.x, gh()-food->position.y, 0);
			for(size_t i = 0; i < snake->tail.size(); i++){
				snake->tail[i] = ofVec3f(snake->tail[i].x, gh()-snake->tail[i].y, 0);
			}

			dimension = _2D;

			if(snake->direction == FORWARD){
				snake->direction = UP;
				break;
			}

			if(snake->direction == BACKWARD){
				snake->direction = DOWN;
				break;
			}

			if(snake->direction == UP){
				snake->direction = DOWN;
				break;
			}

			if(snake->direction == DOWN){
				snake->direction = UP;
				break;
			}
		}

		if(dimension == _PERSPECTIVE){
			// Convert Z position to Y position
			snake->head.y = MAX_Y - map(snake->head.z, -MAX_Z/2, MAX_Z, SNAKE_SIZE, MAX_Y);
			snake->head.z = 0;

			food->position.y = MAX_Y - map(food->position.z, -MAX_Z/2, MAX_Z, SNAKE_SIZE, MAX_Y);
			food->position.z = 0;

			for(size_t i = 0; i < snake->tail.size(); i++){
				snake->tail[i].y = MAX_Y - map(snake->tail[i].z, -MAX_Z/2, MAX_Z, SNAKE_SIZE, MAX_Y);
				snake->tail[i].z = 0;
			}

			if(snake->direction == FORWARD){
				snake->direction = UP;
			}

			if(snake->direction == BACKWARD){
				snake->direction = DOWN;
			}
		}

		dimension = _2D;

		break;
	case '2':
		/*
		*	3D View
		*/
		if(dimension == _3D){
			break;
		}

		glDisable(GL_CULL_FACE);

		if(dimension == _PERSPECTIVE){
			// We need to take off snake from the top face of the cube
			snake->head = ofVec3f(snake->head.x, snake->head.y - SNAKE_SIZE, snake->head.z);
			food->position = ofVec3f(food->position.x, food->position.y - FOOD_SIZE, food->position.z);
			for(size_t i = 0; i < snake->tail.size(); i++){
				snake->tail[i] = ofVec3f(snake->tail[i].x, snake->tail[i].y - SNAKE_SIZE, snake->tail[i].z);
			}
		}

		if(dimension == _2D){
			// Invert snake and food positions
			snake->head = ofVec3f(snake->head.x, gh()-snake->head.y, snake->head.z);
			food->position = ofVec3f(food->position.x, gh()-food->position.y, food->position.z);
			for(size_t i = 0; i < snake->tail.size(); i++){
				snake->tail[i] = ofVec3f(snake->tail[i].x, gh()-snake->tail[i].y, snake->tail[i].z);
			}
		}

		dimension = _3D;

		// Fix the direction when changing to 3D
		if(snake->direction == UP){
			snake->direction = DOWN;
			break;
		}

		if(snake->direction == DOWN){
			snake->direction = UP;
			break;
		}

		break;
	case '3':
		/*
		*	Perspective View
		*/
		if(dimension == _PERSPECTIVE){
			break;
		}

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		if(dimension == _2D){
			// z -> [-500, 500]
			// y (2D) -> [SNAKE_SIZE, MAX_Y] -> [25, 743]

			snake->head.z = -map(snake->head.y, 25, 743, -500, 500);
			snake->head.y = gh();

			food->position.z = -map(food->position.y, 25, 743, -500, 500);
			food->position.y = gh();

			for(size_t i = 0; i < snake->tail.size(); i++){
				snake->tail[i].z = -map(snake->tail[i].y, 25, 743, -500, 500);
				snake->tail[i].y = gh();
			}

		}
		
		if(dimension == _3D){
			// translate elements to the top face of the cube
			snake->head = ofVec3f(snake->head.x, gh(), snake->head.z);
			food->position = ofVec3f(food->position.x, gh(), food->position.z);
			for(size_t i = 0; i < snake->tail.size(); i++){
				snake->tail[i] = ofVec3f(snake->tail[i].x, gh(), snake->tail[i].z);
			}
		}

		if(snake->direction == UP){
			snake->direction = FORWARD;
		}

		if(snake->direction == DOWN){
			snake->direction = BACKWARD;
		}

		dimension = _PERSPECTIVE;

		break;
	case '4':
		/*
		*	First Person View
		*   Not implemented yet
		*/
		glDisable(GL_CULL_FACE);
		dimension = _FIRST_PERSON;
		break;
	// end of switch views
	
	// Movement keys
	case OF_KEY_UP:
		if(snake->direction == DOWN || snake->direction == UP){
			break;
		}
		snake->direction = UP;

		if(dimension == _2D){
			snake->direction = DOWN;
		}

		if(dimension == _PERSPECTIVE){
			snake->direction = BACKWARD;
		}
		
		if(show_instructions == true) ambient_on = false;
		show_instructions = false;
		break;
	case OF_KEY_DOWN:
		if(snake->direction == UP || snake->direction == DOWN){
			break;
		}
		snake->direction = DOWN;

		if(dimension == _2D){
			snake->direction = UP;
		}

		if(dimension == _PERSPECTIVE){
			snake->direction = FORWARD;
		}
		
		if(show_instructions == true) ambient_on = false;
		show_instructions = false;
		break;
	case OF_KEY_LEFT:
		if(snake->direction == RIGHT || snake->direction == LEFT){
			break;
		}
		snake->direction = LEFT;

		if(show_instructions == true) ambient_on = false;
		show_instructions = false;
		break;
	case OF_KEY_RIGHT:
		if(snake->direction == LEFT || snake->direction == RIGHT){
			break;
		}
		snake->direction = RIGHT;
		
		if(show_instructions == true) ambient_on = false;
		show_instructions = false;
		break;
	case 'w':
		if(dimension != _2D){
			if(snake->direction == FORWARD || snake->direction == BACKWARD){
				break;
			}
			snake->direction = FORWARD;
		}
		break;
	case 's':
		if(dimension != _2D){
			if(snake->direction == BACKWARD || snake->direction == FORWARD){
				break;
			}
			snake->direction = BACKWARD;
		}
		break;
	// end of movement keys

	// Camera movement
	case 'i':
		if(dimension == _3D) rotation_speedY = 0.5;
		break;
	case 'k':
		if(dimension == _3D) rotation_speedY = -0.5;
		break;
	case 'j':
		if(dimension == _3D) rotation_speedX = -0.5;
		break;
	case 'l':
		if(dimension == _3D) rotation_speedX = 0.5;
		break;
	case 'o':
		// Reset camera position
		if(dimension == _3D){
			angleX = 0;
			angleY = 0;
			distance = 700;
			rotation_speedX = 0;
			rotation_speedY = 0;
		}
		break;
	case '+':
		if(dimension == _3D) distance += 10;
		break;
	case '-':
		if(dimension == _3D) distance -= 10;
		break;
	// end of camera movement

	case 'p':
		// Pause / Unpause the game
		if(GAME_PAUSED == 0){
			GAME_PAUSED = 1;
			ambient_on = true;
		} else{
			GAME_PAUSED = 0;
			ambient_on = false;
		}
		break;
	case 'r':
		if(GAME_OVER == 1){
			// Restart the game
			GAME_OVER = 0;
			setup();
		}
		break;
	case 'a':
		// Toggle ambient light
		ambient_on = !ambient_on;
		break;
	case OF_KEY_F5:
		point_on = !point_on;
		break;
	case OF_KEY_F6:
		dir_on = !dir_on;
		break;
	case OF_KEY_F7:
		spot_on = !spot_on;
		break;
	case '5':
		point_ambient = !point_ambient;
		break;
	case '6':
		point_diffuse = !point_diffuse;
		break;
	case '7':
		point_specular = !point_specular;
		break;
	case '8':
		dir_ambient = !dir_ambient;
		break;
	case '9':
		dir_diffuse = !dir_diffuse;
		break;
	case '0':
		dir_specular = !dir_specular;
		break;
	case '\'':
		spot_ambient = !spot_ambient;
		break;
	case OF_KEY_BACKSPACE:
		spot_diffuse = !spot_diffuse;
		break;
	case OF_KEY_RETURN:
		spot_specular = !spot_specular;
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

void ofApp::mousePressed(int x, int y, int button){
	switch(button){
	case OF_MOUSE_BUTTON_LEFT:
		// Change the material index
		mat_index -= 1;
		if(mat_index < 0){
			mat_index = NUM_MAT - 1;
		}
		break;
	case OF_MOUSE_BUTTON_RIGHT:
		// Change the material index
		mat_index += 1;
		if(mat_index > NUM_MAT){
			mat_index = 0;
		}
		break;
	}
}
/**
* Handles the event when a key is released.
*
* @param key The key code of the released key.
*
* This function stops the rotation of the object in the 3D space when the 'i', 'k', 'j', or 'l' keys are released.
* - 'i' and 'k' keys stop the rotation around the Y-axis.
* - 'j' and 'l' keys stop the rotation around the X-axis.
*/
void ofApp::keyReleased(int key){
	switch(key){
	case 'i':
	case 'k':
		rotation_speedY = 0;
		break;
	case 'j':
	case 'l':
		rotation_speedX = 0;
		break;
	}
}
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
	// Get a position that is not occupied by the snake and his tail
	int food_x = random_number(FOOD_SIZE*1.5, MAX_X-FOOD_SIZE/2);
	int food_y = random_number(FOOD_SIZE*1.5, MAX_Y-FOOD_SIZE/2);
	if(dimension == _PERSPECTIVE){
		food_y = gh();
	}
	int food_z = random_number(-MAX_Z/2+FOOD_SIZE/2, MAX_Z/2-FOOD_SIZE/2);
	if(dimension == _2D){
		food_z = 0;
	}
	
	ofVec3f food_position = ofVec3f(food_x, food_y, food_z);

	if(snake->tail.size() > 0){
		for(size_t i = 0; i < snake->tail.size(); i++){
			while(snake->tail[i] == food_position){
				food_x = random_number(FOOD_SIZE/2, MAX_X-FOOD_SIZE/2);
				food_y = random_number(FOOD_SIZE/2, MAX_Y-FOOD_SIZE/2);
				if(dimension == _PERSPECTIVE){
					food_y = gh();
				}
				food_z = random_number(-MAX_Z/2+FOOD_SIZE/2, MAX_Z/2-FOOD_SIZE/2);
				if(dimension == _2D){
					food_z = 0;
				}

				food_position = ofVec3f(food_x, food_y, food_z);
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

	int color_index = random_number(0, 2);

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
	int collision = 0;
	ofVec3f snake_pos = snake->head;
	Direction current_direction;
	ofVec3f current_position;

	if(dimension == _2D){
		if(snake_pos.x < SNAKE_SIZE || snake_pos.x > MAX_X || snake_pos.y < SNAKE_SIZE || snake_pos.y > MAX_Y){
			current_direction = snake->direction;
			current_position = snake->head;

			collision = 1;
		}
	} else if(dimension == _PERSPECTIVE){
		if(snake_pos.x < SNAKE_SIZE || snake_pos.x > MAX_X || snake_pos.z < -MAX_Z/2+SNAKE_SIZE/2 || snake_pos.z > MAX_Z/2-SNAKE_SIZE/2){
			current_direction = snake->direction;
			current_position = snake->head;

			collision = 1;
		}
	} else {
		if(snake_pos.x < SNAKE_SIZE || snake_pos.x > MAX_X || snake_pos.y < SNAKE_SIZE || snake_pos.y > MAX_Y || snake_pos.z < -MAX_Z/2 || snake_pos.z > MAX_Z/2){
			current_direction = snake->direction;
			current_position = snake->head;

			collision = 1;
		}
	}

	if(collision == 1){
		switch(current_direction){
		case UP:
			snake->head = ofVec3f(current_position.x, SNAKE_SIZE, current_position.z);
			break;
		case DOWN:
			snake->head = ofVec3f(current_position.x, MAX_Y, current_position.z);
			break;
		case LEFT:
			snake->head = ofVec3f(SNAKE_SIZE, current_position.y, current_position.z);
			break;
		case RIGHT:
			snake->head = ofVec3f(MAX_X, current_position.y, current_position.z);
			break;
		case FORWARD:
			snake->head = ofVec3f(current_position.x, current_position.y, MAX_Z/2-SNAKE_SIZE/2);
			break;
		case BACKWARD:
			snake->head = ofVec3f(current_position.x, current_position.y, -MAX_Z/2+SNAKE_SIZE/2);
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
	ofVec3f food_pos = food->position;
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
* @brief Draws the pause screen.
*
* This function is responsible for rendering the pause screen when the game is paused.
* It displays a "Game Paused" message and instructions to unpause the game.
*/
void ofApp::draw_pause(){
	// Draw the pause screen
	ofSetColor(255, 255, 255);
	ofDrawBitmapString("Game Paused", gw()/2 - 50, gh()/2);
	ofDrawBitmapString("Press 'p' to unpause", gw()/2 - 50, gh()/2 + 20);
}
/**
* @brief Draws the game over screen.
*
* This function sets the color to white and draws the "Game Over" message
* along with instructions to restart the game at the center of the screen.
*/
void ofApp::draw_game_over(){
	// Draw the game over screen
	ofSetColor(255, 255, 255);
	ofDrawBitmapString("Game Over", gw()/2 - 50, gh()/2);
	ofDrawBitmapString("Press 'r' to restart", gw()/2 - 50, gh()/2 + 20);
}
/**
* @brief Updates the camera position based on the current angles and distance.
*
* This function calculates the new camera position in 3D space using spherical coordinates.
* The angles angleX and angleY are converted from degrees to radians and used to compute
* the new x, y, and z coordinates of the camera position.
*/
void ofApp::update_cam_pos(){
    float radX = ofDegToRad(angleX);
    float radY = ofDegToRad(angleY);

    cam_pos.x = distance * cos(radY) * sin(radX);
    cam_pos.y = distance * sin(radY);
    cam_pos.z = distance * cos(radY) * cos(radX);
}
/**
* @brief Draws the instructions for the snake game on the screen.
*
* This function creates a rectangle to display the instructions and then
* renders the instructions text within that rectangle. The instructions
* include the game rules, controls, and other relevant information for
* the player.
*/
void ofApp::draw_instructions(){
	// Create a rectangle to display the instructions
	int width = 500, height = 600;
	int x = gw()/2 , y = gh()/2;

	ambient_on = true;

	glColor4f(0.0f, 0.0f, 0.0f, 1.f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPushMatrix();
		glTranslatef(x, y, 0);
		glScalef(width, height, 0);
		cube_unit();
	glPopMatrix();

	// Display the instructions text

	string text = "Game Rules:\n\n- Use arrow keys to control the snake.\n- Avoid walls and your own tail.\n- Eat food to grow and gain points.\n- Special food items have different effects.\n- Press 'p' to pause/unpause the game.\n- Press 'r' to restart the game.\n- '1' - 2D, '2' - 3D, '3' - Perspective.\n- Control camera position with 'i', 'k', 'j', 'l'\n- Control camera zoom with '+' and '-'\n- 'o' to reset camera settings\n- Press 'f12' to toggle fullscreen mode.\n\nPress any movement key to start the game.";

    ofSetColor(255);
	ofDrawBitmapString(text, gw()/2 - 220, gh()/2 - 250);
	ofDrawBitmapString("Developed by Rodrigo Rodrigues", gw()/2 - 220, gh()/2 + 250);
}
/**
* @brief Draws an arrow on the screen with a specified offset.
*
* This function draws an arrow starting from a fixed point on the screen and extending
* to a point defined by the given offsets. The arrow is drawn in 3D space.
*
* @param offsetX The offset in the X direction from the starting point of the arrow.
* @param offsetY The offset in the Y direction from the starting point of the arrow.
* @param offsetZ The offset in the Z direction from the starting point of the arrow.
*
*
* NOTE: FUNCTION GENERATED BY AI
*/
void ofApp::draw_arrow(int offsetX, int offsetY, int offsetZ){
    ofPushMatrix();
    ofSetupScreen();

    int arrow_x = gw() - 50;
    int arrow_y = 50;
    int arrow_z = 0;
	
    float x1 = arrow_x + offsetX;
    float y1 = arrow_y + offsetY;
    float z1 = arrow_z + offsetZ;

    ofSetColor(255, 255, 255);
    ofDrawLine(arrow_x, arrow_y, arrow_z, x1, y1, z1);

    ofVec3f direction(offsetX, offsetY, offsetZ);
    direction.getNormalized();

    ofVec3f up(0, 1, 0);
    if(direction == up || direction == -up){
        up.set(1, 0, 0);
    }

    ofVec3f perpendicular1 = direction.getCrossed(up).getNormalized();

    float arrowHeadLength = 10.0f;
    float arrowHeadWidth = 5.0f;

    ofVec3f base1 = ofVec3f(x1, y1, z1) - direction * arrowHeadLength + perpendicular1 * arrowHeadWidth;
    ofVec3f base2 = ofVec3f(x1, y1, z1) - direction * arrowHeadLength - perpendicular1 * arrowHeadWidth;

    glBegin(GL_TRIANGLES);
    glVertex3f(x1, y1, z1);
    glVertex3f(base1.x, base1.y, base1.z);
    glVertex3f(base2.x, base2.y, base2.z);
    glEnd();

    ofPopMatrix();
}
/**
* @brief Draws an arrow indicating the direction of the snake.
*
* This function draws an arrow based on the current direction of the snake.
*/
void ofApp::draw_direction_arrow(){
	switch(snake->direction){
	case UP:
		draw_arrow(0, 30, 0);
		break;
	case DOWN:
		draw_arrow(0, -30, 0);
		break;
	case LEFT:
		draw_arrow(-30, 0, 0);
		break;
	case RIGHT:
		draw_arrow(30, 0, 0);
		break;
	case FORWARD:
		draw_arrow(0, 0, 30);
		break;
	case BACKWARD:
		draw_arrow(0, 0, -30);
		break;
	case NONE:
	default:
		break;
	}
}
/**
* Maps a value from one range to another.
*
* @param value The value to be mapped.
* @param inMin The lower bound of the input range.
* @param inMax The upper bound of the input range.
* @param outMin The lower bound of the output range.
* @param outMax The upper bound of the output range.
* @return The mapped value in the output range.
*/
float ofApp::map(float value, float inMin, float inMax, float outMin, float outMax) {
    return outMin + ((value - inMin) / (inMax - inMin)) * (outMax - outMin);
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