#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"

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
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void drawPointCloud();
        void exportPlyCloud(ofMesh pcMesh, string filename);
    
        ofxKinect kinect;
    
        ofMesh pointCloudMesh;
    
        ofxCvGrayscaleImage grayImage; // grayscale depth image
        ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
        ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
    
        ofxCvContourFinder contourFinder;
    
        bool bThreshWithOpenCV;
        bool bDrawPointCloud;
        bool bExportPlyCloud;
    
        int nearThreshold;
        int farThreshold;
    
        int angle;
    
        // used for viewing the point cloud
        ofEasyCam easyCam;
};
