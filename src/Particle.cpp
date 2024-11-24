/*
*	Author:	Rodrigo Miguel Santos Rodrigues
*	Nº:		2022233032
*/

#include "Particle.h"

void Particle::setup(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax){
    float x = ofRandom(xmin, xmax);
    float y = ofRandom(ymin, ymax);
    float z = ofRandom(zmin, zmax);

    pos = ofVec3f(x, y, z);
    
    age = 0.;
    lifeTime = int(ofRandom(5, 30));

    speed = ofVec3f(ofRandom(-2, 2.), ofRandom(-2, 2.), ofRandom(-2, 2.));

    size = gw()*0.01;
    dead = false;
}

void Particle::update(){
    pos += speed;
    age++;
    if(age > lifeTime){
        dead = true;
    }
}

void Particle::draw(){
    if(!dead){
        glColor3f(0., 0.3f, 0.);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glPushMatrix();
        glTranslatef(pos.x, pos.y, pos.z);
        glScalef(size, size, size);
        cube_unit();
        glPopMatrix();
    }
}