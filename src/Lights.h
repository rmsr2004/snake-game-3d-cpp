/*
*	Author:	Rodrigo Miguel Santos Rodrigues
*	Nº:		2022233032
*/

#pragma once

#include "ofMain.h"

#define POINT_LIGHT_SIZE    50
#define DIR_LIGHT_SIZE      100
#define SPOT_LIGHT_SIZE     50

class Lights {
    public:
        /* Point light */
        GLfloat point_pos[4];       // position of the point light
        GLfloat point_amb[4];       // ambient color of the point light
        GLfloat point_diff[4];      // diffuse color of the point light
        GLfloat point_spec[4];      // specular color of the point light
        GLfloat point_atc;          // constant attenuation of the point light
        GLfloat point_atl;          // linear attenuation of the point light
        GLfloat point_atq;          // quadratic attenuation of the point light
        
        /* Directional light */
        GLfloat dir_pos[4];         // direction of the spot light
        GLfloat dir_amb[4];         // ambient color of the spot light
        GLfloat dir_diff[4];        // diffuse color of the spot light
        GLfloat dir_spec[4];        // specular color of the spot light
        GLfloat dir_direction[4];   // direction of the spot light

        /* Spot light */
        GLfloat spot_pos[4];        // position of the spot light
        GLfloat spot_dir[3];        // direction of the spot light
        GLfloat spot_amb[4];        // ambient color of the spot light
        GLfloat spot_diff[4];       // diffuse color of the spot light
        GLfloat spot_spec[4];       // specular color of the spot light
        GLfloat spot_cutoff;        // cutoff angle of the spot light
        GLfloat spot_exponent;      // exponent of the spot light
        GLfloat spot_atc;           // constant attenuation of the spot light
        GLfloat spot_atl;           // linear attenuation of the spot light
        GLfloat spot_atq;           // quadratic attenuation of the spot light

        Lights();
        ~Lights();        
        void draw_light(int light_type, int dimension);
};

// end of Lights.h