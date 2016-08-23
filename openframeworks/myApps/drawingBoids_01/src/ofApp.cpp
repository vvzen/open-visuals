#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // Init colors
    boidsColor = ofColor::blueViolet;
    bgColor = 81;
    
    // Init booleans
    shouldSaveImage = false;
    imageDragged = false;
    
    // Init Flow Field
    flowField.setup(CELL_SIZE);
    
    // Setting bg for the first time
    ofBackground(bgColor);
    ofSetBackgroundAuto(false);
    
    // Init font
    ofTrueTypeFont::setGlobalDpi(72);
    firaSansRSmall.load("fonts/FiraSans-Regular.ttf", 16, true);
    firaSansR.load("fonts/FiraSans-Regular.ttf", 32, true);
    firaSansR.setLineHeight(18.0f);
    
    // Loading the image
//    inputImage.load("images/natalie.jpg");
//    inputImage.setImageType(OF_IMAGE_GRAYSCALE);
//    imageFile.open(ofToDataPath("images/natalie.jpg"));
    
    // Setting windows size as image size
//    int xPixels = inputImage.getWidth();
//    int yPixels = inputImage.getHeight();
//    ofSetWindowShape(xPixels, yPixels);
//    ofSetWindowTitle(imageFile.getBaseName());
    
    // Init boids object in the heap
    for(int i = 0; i < BOIDS_NUMBER; i++){
        boids.push_back(new Boid());
        boids[i]->setup(ofGetWidth()/2, ofGetHeight()/2, BOIDS_SIZE, 1);
        //boids.at(i)->setup(ofGetWidth()/2, ofGetHeight()/2, 10, 1);
    }
    
    //cout << "Size of boids: " << boids.size() << endl;
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // A 2dvector for storing mouse positions
    ofVec2f mouse(ofGetMouseX(), ofGetMouseY());
    
    // Loop into boids
    for(int i = 0; i < boids.size(); i++){
        boids[i]->edges();
        //boids[i]->arrive(mouse);
        boids[i]->flock(boids);
        ofVec2f ffForce = flowField.computeAttraction(boids[i], boids);
        boids[i]->applyForce(ffForce);
        boids[i]->update();
    }
    
    // Save image if user requests it
    if(shouldSaveImage){
        
        cout << "Saving image.." << endl;
        
        ofImage screen;
        //screen.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR);
        screen.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
        string imageName = "FINAL_TEST/"+ofToString(ofGetFrameNum())+".png";
        screen.save(imageName);
        
        shouldSaveImage = false;
//        testFbo.end();
//        // Save fbo to image
//        ofPixels pixels;
//        pixels.allocate(inputImage.getWidth(), inputImage.getHeight(), OF_IMAGE_COLOR);
//        testFbo.readToPixels(pixels);
//        
//        ofSaveImage(pixels, "FBO_TEST/image.png", OF_IMAGE_QUALITY_BEST);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if(imageDragged && !showFlowField){
        ofPixels * imgPixels = &inputImage.getPixels();
        for(int i = 0; i < boids.size(); i++){
            ofColor pixelColor = imgPixels->getColor(boids[i]->pos.x, boids[i]->pos.y);
            boids[i]->render(pixelColor);
        }
    }
    else if(showFlowField && imageDragged){
        flowField.render();
    }
    else {
        ofPushStyle();
        ofBackground(51);
        ofSetColor(ofColor::white);
        ofNoFill();
        firaSansRSmall.drawString("by VVZen", ofGetWidth() - ofGetWidth()/6, ofGetHeight() - ofGetHeight()/8);
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofDrawRectangle(ofGetWidth()/2, ofGetHeight()/2, ofGetWidth()-(ofGetWidth()/8), ofGetHeight()-(ofGetHeight()/8));
        firaSansR.drawString("Drop an image!", ofGetWidth()/2 - 110, ofGetHeight()/2 + 16);
        ofPopStyle();
    }
}


//--------------------------------------------------------------
void ofApp::switchColors(){
    cout << "Switched colors!" << endl;
    if(boidsColor != ofColor::blueViolet){
        boidsColor = ofColor::blueViolet;
        bgColor = ofColor::white;
    }
    else {
        boidsColor = ofColor::white;
        bgColor = ofColor(51,255);
    }
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    string filePath = "";
    for(int i = 0; i < dragInfo.files.size(); i++){
        cout << "Loading image: " << dragInfo.files.at(i) << endl;
        filePath = dragInfo.files.at(i);
    }
    imageDragged = true;
    inputImage.load(filePath);
    
    int next;
    float aspectRatio = inputImage.getWidth()/inputImage.getHeight();
    cout << "aspect ratio: " << aspectRatio << endl;
    
    // Init new width and height of image
    int newWidth = inputImage.getWidth();
    int newHeight = inputImage.getHeight();
    
    // Find the nearest pow of 2 of biggest image dimension (width or height)
    if(inputImage.getWidth() >= inputImage.getHeight()){
        //newWidth = pow(2, ceil(log(inputImage.getWidth())/log(2)));
        newWidth = ofNextPow2(ofGetWidth());
        newHeight = newWidth / aspectRatio;
    }
    else {
        newHeight = ofNextPow2(ofGetHeight());
        newWidth = newHeight * aspectRatio;
    }
    
    // Resize image according to new pow of 2 size
    inputImage.resize(newWidth, newHeight);
    
    // Open the user image
    imageFile.open(ofToDataPath(filePath));
    // Use image name to save resized version of input image
    string resizedImagePath = "inputs/"+imageFile.getBaseName()+"_POW_2.jpg";
    inputImage.setImageType(OF_IMAGE_COLOR);
    inputImage.save(resizedImagePath);
    
    // Use correctly resized and saved image as input
    inputImage.load(resizedImagePath);
    
    cout << "New image dimensions: " << inputImage.getWidth() << " " << inputImage.getHeight() << endl;
    
    // Setting windows size as image size
    ofSetWindowShape(inputImage.getWidth(), inputImage.getHeight());
    ofSetWindowTitle(imageFile.getBaseName());
    
    // New test: number must be divisible f
    
    cout << "Window size: " << ofGetWidth() << " " << ofGetHeight() << endl;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'c'){
        switchColors();
    }
    else if(key == 's'){
        shouldSaveImage = true;
    }
    else if(key == 'r'){
        showFlowField = true;
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