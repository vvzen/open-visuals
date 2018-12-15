#pragma once

#include "ofMain.h"
#include "FlowField.hpp"
#include "Boid.hpp"
#include "FearArea.hpp"
#include "ofxGui.h"
#include "ofxXmlSettings.h"
#include "ofxJSON.h"

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();
    
        void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void saveFBOImage(string path);
    
        // command line arguments
        void updateGUIWithJSONArgs(ofxJSONElement jsonFile);
        vector<string> arguments;
        string argumentsAsString;
    
        // GUI
        ofxPanel gui;
        ofParameter<float> noiseXSpeedGUI;
        ofParameter<float> noiseYSpeedGUI;
        ofParameter<float> cellSizeGUI;
        ofParameter<float> alignmentAmountGUI, separationAmountGUI;
        ofParameter<float> separationThresholdDistanceGUI;
        ofParameter<float> maxForceGUI, maxSteerGUI, maxSpeedGUI;
        ofParameter<float> fieldAttractionGUI;
        ofParameter<bool> shouldRenderGridGUI;
        ofParameter<bool> shouldRenderFearAreasGUI;
        ofParameter<bool> shouldClearCanvasGUI;
    
        // Colors
        const float BG_COLOR = 12;
        float startingHue;
    
        // Listeners
        void changedFieldParams(float & value);
        void changedBoolParam(bool & value);
    
        // Flow field
        FlowField flowField;
    
        // Boids
        vector<Boid *> boids;
        int BOIDS_NUM;
        ofFbo boidsFbo;
    
        // Boids areas of fear
        vector <FearArea *> fearAreas;
        int FEAR_AREAS_NUM;
    
        // XML
        // Used for drawing settings
        ofxXmlSettings xmlSettings;
        // Used to store cmd line args
        ofxXmlSettings argsXml;
    
        // JSON
        ofxJSONElement result;
    
        // Time
        float startTime;
        float elapsedTime;
        float APP_RUNTIME_MS;
    
        // Debug
        const bool DEBUG_JSON = false;
        const bool DEBUG_BOIDS = false;
        const bool SHOW_FEAR_AREAS = false;
        float timeLived = 0.0f;
        
        bool mouseIsPressed = false;
};
