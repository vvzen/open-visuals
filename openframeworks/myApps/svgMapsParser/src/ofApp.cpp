#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(255,255,255);

    textSize = 14;
    
    // Optionally listen for task custom notifications.
    ofAddListener(queue.onTaskCustomNotification, this, &ofApp::onTaskCustomNotification);
    
    string name = "XmlParseTask #1";
    xmlParseTask = new XmlParseTask(name, 100);
    queue.start(xmlParseTask);
    
//    // Learning how to tokenize
//    char myString[] = "fill:rgb(70.980392%,81.568627%,81.568627%);fill-opacity:1;stroke:none;";
//    // Create a pointer to the char[]
//    // strtok() is from <cstring>
//    char *p = strtok(myString, ";");
//    // While there's still a token
//    while (p != NULL) {
//        cout << "Token: " << p << endl;
//        //printf ("Token: %s\n", p);
//        // NULL is for saying use the same string
//        // and continue searching for tokens inside it
//        p = strtok(NULL, ";");
//    }

}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    
    std::stringstream ss;
    
    ss.str("");
    ss << "Total Active Tasks: " << queue.getActiveCount() << std::endl;
    ss << "Total Queued Tasks: " << queue.getQueuedCount();
    
    ofDrawBitmapStringHighlight(ss.str(), ofPoint(4, textSize));
    
    int height = 20;
    int y = height * 3;
    
    ofx::TaskQueue::ProgressMap progress = queue.getTaskProgress();
    ofx::TaskQueue::ProgressMap::const_iterator iter = progress.begin();
    
    while (iter != progress.end() && y < ofGetHeight()){
        
        const ofx::TaskProgressEventArgs& progressInfo = iter->second;
        
        float progress = progressInfo.getProgress();
        string taskId = progressInfo.getTaskId();
        string name = progressInfo.getTaskName();
        string statusString;
        
        ofColor color;
        
        switch (progressInfo.getState()){
            case Poco::Task::TASK_IDLE:
                color = ofColor(127);
                statusString = "idle";
                break;
            case Poco::Task::TASK_STARTING:
                color = ofColor(0, 0, 255);
                statusString = "starting";
                break;
            case Poco::Task::TASK_RUNNING:
                color = ofColor(255, 0, 255);
                statusString = "running";
                break;
            case Poco::Task::TASK_CANCELLING:
                color = ofColor(255, 0, 0);
                statusString = "cancelling";
                break;
            case Poco::Task::TASK_FINISHED:
                color = ofColor(0, 255, 0);
                statusString = "finished";
                break;
        }
        
        std::stringstream ss;
        
        ss << taskId << ": " << statusString << " : " << name << endl;
        
        ofPushMatrix();
        ofTranslate(0, y);
        
        ofFill();
        ofSetColor(color, 127);
        ofDrawRectangle(0, 0, ofGetWidth() * progress, height - 2);
        
        ofNoFill();
        ofSetColor(color);
        ofDrawRectangle(0, 0, ofGetWidth() * progress, height - 2);
        
        ofFill();
        color+=200;
        ofSetColor(color);
        ofDrawBitmapString(ss.str(), 4, textSize);
        
        // Show the current operation and progress ratio
        ofSetColor(255);
        ofDrawBitmapString(xmlParseTask->getCurrentOperation(), 4, textSize*3);
        string ratio = ofToString(xmlParseTask->getCurrentProgressRatio()[0]) + " / " + ofToString(xmlParseTask->getCurrentProgressRatio()[1]);
        ofDrawBitmapString(ratio, 4, textSize*4);
        
        ofPopMatrix();
        
        y += height;
        
        ++iter;
    }
    
    customNotificationReceived = false;
    customNotificationMessage = "";
    
    if(xmlParseTask->hasEnded()) std::exit(0);
}
//--------------------------------------------------------------

void ofApp::onTaskCustomNotification(const ofx::TaskCustomNotificationEventArgs& args){
    
    int i = 0;
    string message;
    args.extract(message);
    
//    if (args.extract(i)){
//        ofLogVerbose("ofApp::onTaskCustomNotification") << "Parsed a custom notification task " << args.getTaskId() << " with int = " << i << std::endl;
//    }
//    else if (args.extract(message)){
//        taskProgress[args.getTaskId()].data = message;
//    }
//    else {
//        std::cout << "Got an unknown custom notification! Name: " << args.getNotification()->name() << std::endl;
//    }
}

//--------------------------------------------------------------
void ofApp::exit(){
    // It's a good practice to unregister the events.
    queue.unregisterTaskProgressEvents(this);
    
    // Unregister the optional notification listener.
    ofRemoveListener(queue.onTaskCustomNotification, this, &ofApp::onTaskCustomNotification);
}

void ofApp::onTaskQueued(const ofx::TaskQueueEventArgs& args){}
void ofApp::onTaskStarted(const ofx::TaskQueueEventArgs& args){}
void ofApp::onTaskCancelled(const ofx::TaskQueueEventArgs& args){}
void ofApp::onTaskFinished(const ofx::TaskQueueEventArgs& args){}
void ofApp::onTaskFailed(const ofx::TaskFailedEventArgs& args){}
void ofApp::onTaskProgress(const ofx::TaskProgressEventArgs& args){}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
    
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

