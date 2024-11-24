/*
*	Author:	Rodrigo Miguel Santos Rodrigues
*	Nº:		2022233032
*/

#ifndef PARTICLE_H
#define PARTICLE_H

#pragma once

#include "ofMain.h"
#include "cg_extras.h"
#include "cg_drawing_extras.h"
#include "cg_cam_extras.h"

class Particle{
    public:

    void setup(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);
    void update();
    void draw();

    ofVec3f pos;
    ofVec3f speed;
    int lifeTime;
    int age;

    float size;

    bool dead;

};

#endif // PARTICLE_H

// end of Particle.h