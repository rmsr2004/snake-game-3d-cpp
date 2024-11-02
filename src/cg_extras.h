/*
CG_EXTRAS
collection of functions to help speed up programming in OF

Developed for the Computer Graphics course @:
Informatics Engineering Department
University of Coimbra
by
André Perrotta & Evgheni Polisciuc

Coimbra, 09/2023
*/

#ifndef CG_EXTRAS_H
#define CG_EXTRAS_H

#include "ofMain.h"

// short named function to get screen width
inline float gw() {
	return ofGetWidth();
}

// short named function to get screen height
inline float gh() {
	return ofGetHeight();
}

#endif