/*
*	Author:	Rodrigo Miguel Santos Rodrigues
*	Nº:		2022233032
*/

#include "ofApp.h"

void ofApp::setup(){}

void ofApp::update(){}

void ofApp::draw(){}

void ofApp::keyPressed(int key){
    // Switch between fullscreen and windowed mode
    if(key == OF_KEY_F12){
		ofToggleFullscreen();
		return;
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