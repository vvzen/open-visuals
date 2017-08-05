#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    kinect.setRegistration(true);
    kinect.init(false, false); // disable video image (faster fps)
    
    kinect.open(); // open the first available kinect
    
    
    // print kinect info
    if(kinect.isConnected()){
        ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
        ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
        ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
        ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
        ofLogNotice() << "resolution: " << std::to_string(kinect.width) << "x" << std::to_string(kinect.height) << "px";
    }
    
    grayImage.allocate(kinect.width, kinect.height);
    grayThreshNear.allocate(kinect.width, kinect.height);
    grayThreshFar.allocate(kinect.width, kinect.height);
    
    ofSetFrameRate(60);
    
    // init tilt on startup
    angle = 15;
    kinect.setCameraTiltAngle(angle);
    
    bDrawPointCloud = true;
    bExportPlyCloud = true;
}

//--------------------------------------------------------------
void ofApp::update(){
    
    kinect.update();
    
    // if there's a new frame and we're connected
    if(kinect.isFrameNew()){
        
        // load grayscale depth image from the kinect source
        grayImage.setFromPixels(kinect.getDepthPixels());
        
        if(bExportPlyCloud){
            string export_name = "kinect-export-v2/kinect_export_" + ofToString(ofGetFrameNum()) + ".ply";
            exportPlyCloud(pointCloudMesh, export_name);
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(50);
    
    
    if(bDrawPointCloud){
        
        easyCam.begin();
        drawPointCloud();
        easyCam.end();
        
    }
    else {
        // draw from the live kinect
        kinect.drawDepth(0, 0, kinect.width, kinect.height);
    }
}

//--------------------------------------------------------------
void ofApp::drawPointCloud(){
    
    pointCloudMesh.clear();
    
    // construct point cloud from kinect data
    pointCloudMesh.setMode(OF_PRIMITIVE_POINTS);
    int step = 2;
    int point_size = 2;
    for(int y = 0; y < kinect.height; y+=step){
        for(int x = 0; x < kinect.width; x+=step){
            if(kinect.getDistanceAt(x,y) > 0){
                pointCloudMesh.addColor(kinect.getColorAt(x, y));
                pointCloudMesh.addVertex(kinect.getWorldCoordinateAt(x, y));
            }
        }
    }
    glPointSize(point_size);
    ofPushMatrix();
    
    // the projected points are upside down and backwards
    ofScale(1, -1, -1);
    ofTranslate(0, 0, -1000); // center the points
    //ofRotateZ(1);
    ofEnableDepthTest();
    pointCloudMesh.drawVertices();
    ofDisableDepthTest();
    
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::exportPlyCloud(ofMesh pcMesh, string filename){

    ofFile ply;
    if(ply.open(filename, ofFile::WriteOnly)){
        // write the header
        ply << "ply" << endl;
        ply << "format binary_little_endian 1.0" << endl;
        ply << "element vertex " << pcMesh.getVertices().size() << endl;
        ply << "property float x" << endl;
        ply << "property float y" << endl;
        ply << "property float z" << endl;
        ply << "end_header" << endl;
        
        // write all the vertices
        vector<ofVec3f>& surface = pcMesh.getVertices();
        for(int i = 0; i < surface.size(); i++){
            if(surface[i].z != 0){
                // write the raw data as if it were a stream of bytes
                ply.write((char *) &surface[i], sizeof(ofVec3f));
            }
        }
    }
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    
    switch (key) {
        
        // export point cloud
        case 'e':
            if(bExportPlyCloud) {
                ofLog(OF_LOG_VERBOSE) << "started saving of point cloud" << endl;
            }
            else {
                ofLog(OF_LOG_VERBOSE) << "ending point cloud export" << endl;
            }
            bExportPlyCloud = !bExportPlyCloud;
            break;
        // draw point cloud
        case 'p':
            bDrawPointCloud = !bDrawPointCloud;
            break;
        // tilt kinect using up and down key
        case OF_KEY_UP:
            angle++;
            if(angle>30) angle=30;
            kinect.setCameraTiltAngle(angle);
            break;
    
        case OF_KEY_DOWN:
            angle--;
            if(angle<-30) angle=-30;
            kinect.setCameraTiltAngle(angle);
            break;
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
