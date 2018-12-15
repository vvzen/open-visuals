#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    
    if(!DEBUG_BOIDS) ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    // Add listeners to gui parameters
    noiseXSpeedGUI.addListener(this, &ofApp::changedFieldParams);
    noiseYSpeedGUI.addListener(this, &ofApp::changedFieldParams);
    cellSizeGUI.addListener(this, &ofApp::changedFieldParams);
    shouldRenderGridGUI.addListener(this, &ofApp::changedBoolParam);
    
    // Setup gui
    gui.setup();
    gui.add(cellSizeGUI.set("Cell Size", 64.0f, 2.0f, 256.0f));
    gui.add(noiseXSpeedGUI.set("Noise X Offset", 0.1f, 0.00001f, 0.1f));
    gui.add(noiseYSpeedGUI.set("Noise Y Offset", 0.001f, 0.00001f, 0.06f));
    gui.add(alignmentAmountGUI.set("Alignment Amount", 1.0f, 0.0f, 8.0f));
    gui.add(separationAmountGUI.set("Separation Amount", 1.0f, 0.0f, 8.0f));
    gui.add(separationThresholdDistanceGUI.set("Separation Dist", 20.0f, 0.0f, 200.0f));
    gui.add(fieldAttractionGUI.set("Field Attraction", 2.0f, 0.0f, 10.f));
    gui.add(maxForceGUI.set("MAX FORCE", 10.0f, 1.0f, 16.0f));
    gui.add(maxSteerGUI.set("MAX STEER", 2.0f, 1.0f, 4.0f));
    gui.add(maxSpeedGUI.set("MAX SPEED", 5.0f, 1.0f, 16.0f));
    gui.add(shouldRenderGridGUI.set("Show Grid", false));
    gui.add(shouldClearCanvasGUI.set("Clear Canvas", true));
    gui.add(shouldRenderFearAreasGUI.set("Show Fear Areas", false));
    
    // Setup flowfield
    flowField.initGrid(0.008f, 0.014f, ofVec2f(32.0f, 32.0f));
    
    // Generate a random numbers of Boids
    BOIDS_NUM = (int) ofRandom(20,40);
    
    // Setup boids
    for(int i = 0; i < BOIDS_NUM; i++){
        Boid * newBoid = new Boid();
        newBoid->setup(ofVec2f(ofGetWidth()/2, ofGetHeight()/2), ofVec2f(4, 4));
        boids.push_back(newBoid);
    }
    boidsFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGB, 4);
    boidsFbo.begin();
    ofClear(BG_COLOR, 255);
    boidsFbo.end();
    
    // Generate a small random number of areas of fear
    FEAR_AREAS_NUM = (int) ofRandom(2,32);
    for(int i = 0; i < FEAR_AREAS_NUM; i++){
        // Create an enemy at a random location
        FearArea * newFearArea = new FearArea();
        ofVec2f newPosition(ofVec2f(ofRandom(ofGetWidth()), ofRandom(ofGetHeight())));
        newFearArea->setup(newPosition, ofRandom(16,128));
        fearAreas.push_back(newFearArea);
    }
    
    // Generate a random runtime
    APP_RUNTIME_MS = 1 * int(ofRandom(90, 120)) * 1000; // ms * 1000 = seconds
    
    // Parse the json with the args
    string jsonArgs = "args.json";
    bool parsingSuccessful = result.open(jsonArgs);
    if(parsingSuccessful){
        
        // Read the cmd args from the json file
        // Set the gui with those args
        updateGUIWithJSONArgs(result);
        
        if(DEBUG_JSON){
            ofLog(OF_LOG_NOTICE, "args.json : "+result.getRawString());
            ofLog(OF_LOG_NOTICE, "args size : "+ ofToString(result["args"].size()));
            ofLog(OF_LOG_NOTICE, "args as array elements : ");
            for(int i = 0; i < result["args"].size(); i++){
                ofLog(OF_LOG_NOTICE, ofToString(result["args"].operator[](i)));
            }
        }
    }
    else {
        ofLog(OF_LOG_ERROR, "Failed to parse json");
    }
    
    // Generate a random starting hue
    startingHue = ofRandom(360);
    
    cout << "Starting hue: " << startingHue << endl;
    
    ofBackground(BG_COLOR);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    timeLived += 0.01f;
    startingHue += 0.01f;
    
    // Update static constants for boids
    Boid::MAX_FORCE = maxForceGUI.get();
    Boid::MAX_STEER = maxSteerGUI.get();
    Boid::MAX_SPEED = maxSpeedGUI.get();
    
    // Update boids
    for(int i = 0; i < boids.size(); i++){
        
        // The boids have fear to go in some areas
        for(int e = 0; e < fearAreas.size(); e++){
            boids[i]->fear(fearAreas[e]->getPosition(), fearAreas[e]->getSize());
        }
        
        //boids[i]->fear(ofVec2f(ofGetMouseX(), ofGetMouseY()));
        //boids[i]->follow(ofVec2f(ofGetMouseX(), ofGetMouseY()));
        boids[i]->flock(boids, alignmentAmountGUI.get(), separationAmountGUI.get(), separationThresholdDistanceGUI.get());
        ofVec2f fieldAttraction = flowField.computeAttraction(boids[i]);
        fieldAttraction.scale(fieldAttractionGUI.get());
        //cout << "Field attraction: " << ofToString(fieldAttraction) << endl;
        //cout << "Boid acceleration: " << boids[i]->getAcceleration() << ", velocity:" << boids[i]->getVelocity() << endl;
        boids[i]->addForce(fieldAttraction);
        boids[i]->update(true);
    }
    
    if(ofGetFrameNum() % 10 == 0) saveFBOImage("showreel/generative_boids_" + ofToString(ofGetFrameNum()) + ".jpg");
    
    // If app has been running for the requested time
    if(ofGetElapsedTimeMillis() >= APP_RUNTIME_MS){
        cout << "Closing app" << endl;
        saveFBOImage("output.jpg");
        ofExit();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
//    ofBackground(0);
//    ofSetColor(0,255,0);
//    ofDrawBitmapString("App arguments: ", 20.0f, 20.0f);
//    ofSetColor(255);
//    ofSaveScreen(ofToString(ofGetFrameNum())+".png");
    
    // Render boids
    for(int i = 0; i < boids.size(); i++){
        
        // Render cells in which boid resides
//        for(int y = 0; y < flowField.grid.size(); y++){
//            for(int x = 0; x < flowField.grid[y].size(); x++){
//                
//                if(boids[i]->getPosition().x > x * flowField.getCellSize().x &&
//                   boids[i]->getPosition().x <= (x +1) * flowField.getCellSize().x){
//                    if(boids[i]->getPosition().y > y * flowField.getCellSize().y &&
//                       boids[i]->getPosition().y <= (y +1) * flowField.getCellSize().y){
//                        ofSetColor(255, 0, 0);
//                        ofNoFill();
//                        ofPushMatrix();
//                        ofTranslate(x * flowField.getCellSize().x, y * flowField.getCellSize().y);
//                        ofDrawRectangle(0, 0, flowField.getCellSize().x, flowField.getCellSize().y);
//                        ofPopMatrix();
//                    }
//                }
//            }
//        }
        
        //float r = mappedColor;
        //float b = mappedColor;
        ofColor targetColor;
        
        float maxDistance = ofDist(ofGetWidth()/2, ofGetHeight()/2, ofGetWidth(), ofGetHeight());
        /*
        float mappedColor = ofMap(ofDist(boids[i]->getPosition().x,
                                           boids[i]->getPosition().y,
                                           ofGetWidth()/2,
                                           ofGetHeight()/2), 0, maxDistance, 0, 200);
         */
        
        float mappedColor = ofMap(sin(timeLived), -1, 1, 0, 100);
        
        //targetColor.set(ofMap(sin(timeLived), -1, 1, 0, 60), mappedColor);
        
        //targetColor.set(30.0f, mappedColor);
        if(startingHue >= 255.0f){
            startingHue = 0.0f;
        }
        
        targetColor.set(mappedColor, 30);
        
        /* targetColor.setHsb(startingHue, 100, 16, 100); */
        
        //cout << "Starting hue: " << startingHue << "\nTarget color: " << ofToString(targetColor) << endl;
        
        //if (DEBUG_BOIDS) targetColor.set(255, 255, 255, 255);
        
        boids[i]->render(targetColor, &boidsFbo);
    }
    
    if(shouldClearCanvasGUI.get()){
        boidsFbo.begin();
        ofClear(BG_COLOR, 255);
        boidsFbo.end();
    }
    
    shouldClearCanvasGUI.set(false);
    
    boidsFbo.draw(0, 0);
    
    if(shouldRenderGridGUI.get()){
        flowField.drawGrid();
    }
    
    // Visualize fear areas
    if(shouldRenderFearAreasGUI.get() || SHOW_FEAR_AREAS){
        for(int fa = 0; fa < fearAreas.size(); fa++){
            fearAreas[fa]->render();
        }
    }
    
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::changedFieldParams(float & value){
    // Update grid
    if(cellSizeGUI.get() != 0) flowField.initGrid(noiseXSpeedGUI.get(), noiseYSpeedGUI.get(), ofVec2f(cellSizeGUI.get(), cellSizeGUI.get()));
}
//--------------------------------------------------------------
void ofApp::changedBoolParam(bool &value){
    // Update grid
    
}

//--------------------------------------------------------------
void ofApp::exit(){
    noiseXSpeedGUI.removeListener(this, &ofApp::changedFieldParams);
    noiseYSpeedGUI.removeListener(this, &ofApp::changedFieldParams);
    cellSizeGUI.removeListener(this, &ofApp::changedFieldParams);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    // Save settings to xml file
    if(key == 's'){
        
        // Clear old file
        xmlSettings.clear();
        if(xmlSettings.saveFile("settings.xml")){
            cout << "Saving settings.xml.." << endl;
            
            xmlSettings.addTag("settings");
            xmlSettings.pushTag("settings");
            // Save general settings
            xmlSettings.addValue("max_force", maxForceGUI.get());
            xmlSettings.addValue("max_speed", maxSpeedGUI.get());
            xmlSettings.addValue("max_steer", maxSteerGUI.get());
            xmlSettings.addValue("flock_align", alignmentAmountGUI.get());
            xmlSettings.addValue("field_attraction", fieldAttractionGUI.get());
            xmlSettings.addValue("separation_amount", separationAmountGUI.get());
            xmlSettings.addValue("separation_threshold", separationThresholdDistanceGUI.get());
            // Save noise settings
            xmlSettings.addTag("noise_speed");
            xmlSettings.pushTag("noise_speed");
            xmlSettings.addValue("X", noiseXSpeedGUI.get());
            xmlSettings.addValue("Y", noiseYSpeedGUI.get());
            xmlSettings.popTag();
            // Save cell size
            xmlSettings.addTag("cell_size");
            xmlSettings.pushTag("cell_size");
            xmlSettings.addValue("X", cellSizeGUI.get());
            xmlSettings.addValue("Y", cellSizeGUI.get());
            xmlSettings.popTag();
            xmlSettings.popTag();
            xmlSettings.saveFile("settings.xml");
            cout << "Saved!" << endl;
        }
    }
    // Load previous settings
    if(key == 'r'){
        if(xmlSettings.loadFile("settings.xml")){
            cout << "Reading previous settings.xml.." << endl;
            xmlSettings.pushTag("settings");
            maxForceGUI.set(xmlSettings.getValue("max_force", 0.0f));
            maxSpeedGUI.set(xmlSettings.getValue("max_speed", 0.0f));
            maxSteerGUI.set(xmlSettings.getValue("max_steer", 0.0f));
            alignmentAmountGUI.set(xmlSettings.getValue("flock_align", 0.0f));
            fieldAttractionGUI.set(xmlSettings.getValue("field_attraction", 0.0f));
            separationAmountGUI.set(xmlSettings.getValue("separation_amount", 0.0f));
            separationThresholdDistanceGUI.set(xmlSettings.getValue("separation_threshold", 0.0f));
            xmlSettings.pushTag("noise_speed");
            noiseXSpeedGUI.set(xmlSettings.getValue("X", 0.008f));
            noiseYSpeedGUI.set(xmlSettings.getValue("Y", 0.014f));
            xmlSettings.popTag();
            xmlSettings.pushTag("cell_size");
            cellSizeGUI.set(xmlSettings.getValue("X", 64.0f));
            xmlSettings.popTag();
            xmlSettings.popTag();
        }
        else {
            ofLog(OF_LOG_ERROR, "settings.xml does not exist");
        }
    }
    // Save image
    if(key == 'w'){
        
        saveFBOImage("output.jpg");
    }
}

// Save FBO image to disk
void ofApp::saveFBOImage(string path){
    string imageName = ofToString(path);
    //cout << "Saving image " << imageName<< endl;
    // Get fbo texture pixels and save to image
    ofPixels pixels;
    ofImage outImage;
    boidsFbo.getTexture().readToPixels(pixels);
    outImage.setFromPixels(pixels);
    outImage.save(imageName);
}

//--------------------------------------------------------------
void ofApp::updateGUIWithJSONArgs(ofxJSONElement jsonFile){
    
    if (DEBUG_JSON) ofLog(OF_LOG_NOTICE, "Reading GUI parameters from args.json");
    
    float currentValue = 0.0f;
    for(int i = 0; i < result["args"].size(); i++){
        currentValue = result["args"].operator[](i).asFloat();
        //if(DEBUG_JSON) ofLog(OF_LOG_NOTICE, ofToString(currentValue) +"\n");
        // Set GUI parameters based on index of appearance
        string logString;
        switch (i) {
            case 0:
                maxForceGUI.set(currentValue);
                break;
            case 1:
                maxSpeedGUI.set(currentValue);
                break;
            case 2:
                maxSteerGUI.set(currentValue);
                break;
            case 3:
                alignmentAmountGUI.set(currentValue);
                break;
            case 4:
                fieldAttractionGUI.set(currentValue);
                break;
            case 5:
                separationAmountGUI.set(currentValue);
                break;
            case 6:
                separationThresholdDistanceGUI.set(currentValue);
                break;
            case 7:
                noiseXSpeedGUI.set(currentValue);
                break;
            case 8:
                noiseYSpeedGUI.set(currentValue);
                break;
            case 9:
                cellSizeGUI.set(currentValue);
                break;
                
            default:
                break;
        }
    }
    
    if (DEBUG_JSON) ofLog(OF_LOG_NOTICE, "Done!");
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
