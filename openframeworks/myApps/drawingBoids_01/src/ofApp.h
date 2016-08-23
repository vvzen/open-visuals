#pragma once

#include "ofMain.h"
#include "Boid.hpp"
#include "FlowField.hpp"

#define BOIDS_NUMBER 128
#define BOIDS_SIZE 10
#define CELL_SIZE 16

class ofApp : public ofBaseApp{

	public:
    
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void dragEvent(ofDragInfo dragInfo);
    
        void switchColors();
    
        ofImage inputImage;
        ofFile imageFile;
    
        ofVec2f testReference(Boid &boid);
    
        // Our list of boids
        vector<Boid*> boids;
        FlowField flowField;
        ofColor boidsColor, bgColor;
        bool imageDragged;
        // Used to save image
        bool shouldSaveImage;
        // Used to show flow field
        bool showFlowField;
        // Fonts
        ofTrueTypeFont firaSansR, firaSansRSmall;
    
        ofFbo testFbo;
};
