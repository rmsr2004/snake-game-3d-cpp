/*
*	Author:	Rodrigo Miguel Santos Rodrigues
*	Nº:		2022233032
*/

#include "Lights.h"

#include "cg_drawing_extras.h"

Lights::Lights(){
    point_pos[0] = 0.0;
    point_pos[1] = 0.0;
    point_pos[2] = 0.0;
    point_pos[3] = 1.0;

    point_amb[0] = 0.0;
    point_amb[1] = 0.0;
    point_amb[2] = 0.0;
    point_amb[3] = 1.0;

    point_diff[0] = 0.0;
    point_diff[1] = 0.0;
    point_diff[2] = 0.0;
    point_diff[3] = 1.0;

    point_spec[0] = 0.0;
    point_spec[1] = 0.0;
    point_spec[2] = 0.0;
    point_spec[3] = 1.0;

    dir_pos[0] = 0.0;
    dir_pos[1] = 0.0;
    dir_pos[2] = 0.0;
    dir_pos[3] = 1.0;

    dir_amb[0] = 0.0;
    dir_amb[1] = 0.0;
    dir_amb[2] = 0.0;
    dir_amb[3] = 1.0;

    dir_diff[0] = 0.0;
    dir_diff[1] = 0.0;
    dir_diff[2] = 0.0;
    dir_diff[3] = 1.0;

    dir_spec[0] = 0.0;
    dir_spec[1] = 0.0;
    dir_spec[2] = 0.0;
    dir_spec[3] = 1.0;

    dir_direction[0] = 0.0;
    dir_direction[1] = 0.0;
    dir_direction[2] = 0.0;
    dir_direction[3] = 1.0;

    spot_pos[0] = 0.0;
    spot_pos[1] = 0.0;
    spot_pos[2] = 0.0;
    spot_pos[3] = 1.0;

    spot_dir[0] = 0.0;
    spot_dir[1] = 0.0;
    spot_dir[2] = 0.0;
    spot_dir[3] = 1.0;

    spot_amb[0] = 0.0;
    spot_amb[1] = 0.0;
    spot_amb[2] = 0.0;
    spot_amb[3] = 1.0;

    spot_diff[0] = 0.0;
    spot_diff[1] = 0.0;
    spot_diff[2] = 0.0;

    spot_spec[0] = 0.0;
    spot_spec[1] = 0.0;
    spot_spec[2] = 0.0;
    spot_spec[3] = 1.0;
    spot_cutoff = 0.0;
    spot_exponent = 0.0;
    spot_atc = 0.0;
    spot_atl = 0.0;
    spot_atq = 0.0;
}

Lights::~Lights(){}

void Lights::draw_light(int light_type, int dimension){
    switch(light_type){
    case 0:
        // draw point light (cube)

        // draw the line from the origin to the point light
        glBegin(GL_LINES);
            glVertex3f(point_pos[0], -500, point_pos[2]);
            glVertex3f(point_pos[0], point_pos[1], point_pos[2]);
        glEnd();

        // draw cube
        glPushMatrix();
            glTranslatef(point_pos[0], point_pos[1], point_pos[2]);
            if(dimension == 0){
                glScalef(POINT_LIGHT_SIZE, POINT_LIGHT_SIZE, 0);
            } else {
                glScalef(POINT_LIGHT_SIZE, POINT_LIGHT_SIZE, POINT_LIGHT_SIZE);
            }
            cube_unit();
        glPopMatrix();
        break;
    case 1:
        // draw directional light (sun)
        glPushMatrix();
            glTranslatef(dir_pos[0], dir_pos[1], dir_pos[2]);
            glScalef(DIR_LIGHT_SIZE, DIR_LIGHT_SIZE, DIR_LIGHT_SIZE);
            ofDrawSphere(1.);
        glPopMatrix();
        break;
    case 2:
        // draw spot light (flashlight)
        glPushMatrix();
            glTranslatef(spot_pos[0], spot_pos[1], spot_pos[2]);
            glScalef(SPOT_LIGHT_SIZE, SPOT_LIGHT_SIZE, SPOT_LIGHT_SIZE);
            cube_unit();
        glPopMatrix();
        break;
    default:
        break;
    }
}

// end of Lights.cpp