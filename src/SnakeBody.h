/*
*	Author:	Rodrigo Miguel Santos Rodrigues
*	Nº:		2022233032
*/

#ifndef SNAKEBODY_H
#define SNAKEBODY_H

#include <iostream>
using namespace std;

#define NULL nullptr

struct Segment {
    int x, y, z;
    Segment* next;

    // Segment Constructor
    Segment(int posX, int posY, int posZ) : x(posX), y(posY), z(posZ), next(NULL) {}
};

class SnakeBody {
    Segment* head;  // pointer to the first segment of the snake
    
    public:
        SnakeBody();    // constructor
        ~SnakeBody();   // destructor

        void insert(int x, int y, int z);   // insert a new segment at the beginning of the snake
        void remove();                      // remove the last segment of the snake

};

#endif // SNAKEBODY_H

// end of SnakeBody.h