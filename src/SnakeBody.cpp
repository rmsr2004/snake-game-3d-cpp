/*
*	Author:	Rodrigo Miguel Santos Rodrigues
*	Nº:		2022233032
*/

#include <iostream>
#include "SnakeBody.h"

#define NULL nullptr

SnakeBody::SnakeBody() : head(NULL) {}

SnakeBody::~SnakeBody() {
    Segment* current = head;
    while(current != NULL){
        Segment* next = current->next;
        delete current;
        current = next;
    }
}

void SnakeBody::insert(int x, int y, int z){
    Segment* new_segment = new Segment(x, y, z);
    if (head == nullptr) {
        head = new_segment;
    } else {
        Segment* temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_segment;
    }
}

void SnakeBody::remove(){
    if(head == NULL) return;

    Segment* current = head;
    Segment* previous = NULL;
    while(current->next != NULL){
        previous = current;
        current = current->next;
    }
    delete current;
    
    if(previous != NULL) previous->next = NULL; else head = NULL;
}

// end of SnakeBody.cpp