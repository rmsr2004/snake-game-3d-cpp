/*
*	Author:	Rodrigo Miguel Santos Rodrigues
*	Nº:		2022233032
*/

#include "ofMain.h"
#include "ofApp.h"

int main(){
	ofGLWindowSettings settings;
	settings.setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	settings.windowMode = OF_WINDOW;

	auto window = ofCreateWindow(settings);

	ofRunApp(window, make_shared<ofApp>());
	ofRunMainLoop();
}

// end of main.cpp