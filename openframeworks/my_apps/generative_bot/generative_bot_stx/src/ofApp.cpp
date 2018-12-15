#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    
    if(!DEBUG_BOIDS) ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    // Add listeners to gui parameters
    noiseXSpeedGUI.addListener(this, &ofApp::changedFieldParams);
    noiseYSpeedGUI.addListener(this, &ofApp::changedFieldParams);
    cellSizeGUI.addListener(this, &ofApp::changedFieldParams);
    //shouldRenderGridGUI.addListener(this, &ofApp::changedBoolParam);
    
    // Setup gui
    gui.setup();
    gui.add(cellSizeGUI.set("Cell Size", 64.0f, 2.0f, 256.0f));
    gui.add(noiseXSpeedGUI.set("Noise X Offset", 0.1f, 0.00001f, 0.1f));
    gui.add(noiseYSpeedGUI.set("Noise Y Offset", 0.001f, 0.00001f, 0.06f));
    gui.add(alignmentAmountGUI.set("Alignment Amount", 1.0f, 0.0f, 16.0f));
    gui.add(separationAmountGUI.set("Separation Amount", 1.0f, 0.0f, 8.0f));
    gui.add(separationThresholdDistanceGUI.set("Separation Dist", 20.0f, 0.0f, 200.0f));
    gui.add(fieldAttractionGUI.set("Field Attraction", 4.0f, 0.0f, 16.f));
    gui.add(maxForceGUI.set("MAX FORCE", 5.0f, 1.0f, 32.0f));
    gui.add(maxSteerGUI.set("MAX STEER", 5.0f, 1.0f, 32.0f));
    gui.add(maxSpeedGUI.set("MAX SPEED", 5.0f, 1.0f, 32.0f));
    gui.add(shouldRenderGridGUI.set("Show Grid", false));
    gui.add(shouldClearCanvasGUI.set("Clear Canvas", true));
    gui.add(shouldRenderFearAreasGUI.set("Show Fear Areas", false));
    gui.add(shouldShowFearTextGUI.set("Show Fear Text", false));
    gui.add(startingHueGUI.set("Starting Hue", 0, 0, 255));
    
    // Setup flowfield
    flowField.initGrid(0.008f, 0.014f, ofVec2f(32.0f, 32.0f));
    
    // Generate a random numbers of Boids
    BOIDS_NUM = (int) ofRandom(30,70);
    
    // Setup boids
    for(int i = 0; i < BOIDS_NUM; i++){
        Boid * newBoid = new Boid();
        ofVec2f newPos = ofVec2f(ofRandom(0, ofGetWidth()), ofRandom(0, ofGetHeight()));
        newBoid->setup(newPos, ofVec2f(1, 1));
        boids.push_back(newBoid);
    }
    boidsFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGB, 4);
    boidsFbo.begin();
    ofClear(BG_COLOR, 255);
    boidsFbo.end();
    
    // Generate a small random number of areas of fear
    if(USE_FEAR_AREAS){
        
        FEAR_AREAS_NUM = (int) ofRandom(2,4);
        for(int i = 0; i < FEAR_AREAS_NUM; i++){
            // Create an enemy at a random location
            FearArea * newFearArea = new FearArea();
            ofVec2f newPosition(ofVec2f(ofRandom(ofGetWidth()), ofRandom(ofGetHeight())));
            newFearArea->setup(newPosition, ofRandom(16,128));
            fearAreas.push_back(newFearArea);
        }
    }
    
    // Generate a random runtime
    APP_RUNTIME_MS = 1 * int(ofRandom(180, 240)) * 1000; // ms * 1000 = seconds
    
    // Parse the json with the args
    if(jsonArguments.open("args.json")){
        
        // Read the cmd args from the json file
        // Set the gui with those args
        updateGUIWithJSONArgs(jsonArguments);
        
        if(DEBUG_JSON){
            ofLog(OF_LOG_NOTICE, "args.json : "+jsonArguments.getRawString());
            ofLog(OF_LOG_NOTICE, "args size : "+ ofToString(jsonArguments["args"].size()));
            ofLog(OF_LOG_NOTICE, "args as array elements : ");
            for(int i = 0; i < jsonArguments["args"].size(); i++){
                ofLog(OF_LOG_NOTICE, ofToString(jsonArguments["args"].operator[](i)));
            }
        }
    }
    else {
        ofLog(OF_LOG_ERROR, "Failed to parse json");
    }
    
    // Parse the fear words from the json file
    vector<string> fearWords;
    if(jsonFearWords.open("words.json")){
        
        for(int i = 0; i < jsonFearWords["fear_words"].size(); i++){
            //cout << jsonFearWords["fear_words"].operator[](i).asString() << endl;
            fearWords.push_back(jsonFearWords["fear_words"].operator[](i).asString());
        }
    }
    
    // Init font and text
    openSansFont.load("fonts/OpenSans-Semibold.ttf", 230, true);
    fearText = fearWords[ofRandom(0, fearWords.size())];
    fearTextFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA, 0);
    
    // Generate a random starting hue
    startingHue = startingHueGUI.get();
    
    startingHue = 150;
    
    cout << "Starting hue: " << startingHue << endl;
    
    ofBackground(BG_COLOR);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    timeLived += 0.01f;
    startingHue += 0.001f;
    
    // Update static constants for boids
    Boid::MAX_FORCE = maxForceGUI.get();
    Boid::MAX_STEER = maxSteerGUI.get();
    Boid::MAX_SPEED = maxSpeedGUI.get();
    
    // Pixels from the fear text fbo
    ofPixels fearTextPixels;
    fearTextFbo.getTexture().readToPixels(fearTextPixels);
    
    // Update boids
    for(int i = 0; i < boids.size(); i++){
        
        // The boids have fear to go in some areas
        for(int e = 0; e < fearAreas.size(); e++){
            boids[i]->fear(fearAreas[e]->getPosition(), fearAreas[e]->getSize());
        }
        
        // The boids have fear of the white pixels
        ofColor currentTextFearColor = fearTextPixels.getColor(boids[i]->getPosition().x, boids[i]->getPosition().y);
        
        if(currentTextFearColor.r > 230){
            boids[i]->fear(boids[i]->getPosition(), 80);
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
    
    // get bb of fear text
    fearTextSize = openSansFont.getStringBoundingBox(fearText, 0, 0);
    
    // draw fear text in a separate framebuffer
    fearTextFbo.begin();
        ofSetColor(255);
        ofFill();
        ofBackground(0);
        openSansFont.drawString(fearText, ofGetWidth()/2 - fearTextSize.width/2, ofGetHeight()/2 + fearTextSize.height/4);
    fearTextFbo.end();
    
    // If app has been running for the requested time
    if(ofGetElapsedTimeMillis() >= APP_RUNTIME_MS){
        cout << "Closing app" << endl;
        saveFBOImage("output.jpg");
        ofExit();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
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
        
        ofColor targetColor;
        
        
        // Set color
        
        float maxDistance = ofDist(ofGetWidth()/2, ofGetHeight()/2, ofGetWidth(), ofGetHeight());
        float mappedColor = ofMap(ofDist(boids[i]->getPosition().x,
                                         boids[i]->getPosition().y,
                                         ofGetWidth()/2,
                                         ofGetHeight()/2),
                                  0, maxDistance, 255, 100);
        
        //mappedColor = ofMap(sin(timeLived), -1, 1, 0, 100);
        
        if(startingHue >= 255.0f){
            startingHue = 0.0f;
        }
        
        targetColor.setHsb(startingHue, mappedColor, 200, 30);
        
        //targetColor.set(mappedColor, 0, 1 - mappedColor, 30);
        
        boids[i]->render(targetColor, &boidsFbo);
    }
    
    if(shouldClearCanvasGUI.get()){
        boidsFbo.begin();
        ofClear(BG_COLOR, 255);
        boidsFbo.end();
    }
    
    shouldClearCanvasGUI.set(false);
    
    boidsFbo.draw(0, 0);
    
    // Render grid
    if(shouldRenderGridGUI.get()){
        flowField.drawGrid();
    }
    
    // Visualize fear text
    if(shouldShowFearTextGUI){
        fearTextFbo.draw(0, 0);
    }
    
    // Visualize fear areas
    if(shouldRenderFearAreasGUI.get() || SHOW_FEAR_AREAS){
        for(int fa = 0; fa < fearAreas.size(); fa++){
            fearAreas[fa]->render();
        }
    }
    
    gui.draw();
    
    
    // DEBUG
    ofDrawBitmapString("Starting hue: " + ofToString(startingHue), ofGetWidth()-200, 20);
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
    // Save fear text as image
    if(key == 't'){
        ofPixels pixels;
        fearTextFbo.getTexture().readToPixels(pixels);
        ofSaveImage(pixels, "fear_text.png");
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
    for(int i = 0; i < jsonArguments["args"].size(); i++){
        currentValue = jsonArguments["args"].operator[](i).asFloat();
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
            case 10:
                startingHueGUI.set(currentValue);
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
