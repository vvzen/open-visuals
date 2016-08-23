#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxTaskQueue.h"
#include "XmlParseTask.hpp"

/*
 
 addTag() adds a tag by tag + which or by path with an optional value
 setValue() sets a value by tag + which or by path if the tag is present
 getValue() gets a value by tag + which or by path if the tag is present
 clearTagContents() clears the value of a tag AND any children
 removeTag() removes a tag by tag + which or by path
 tagExists() tells whether a tag exists by tag + which or by path
 
 addAttribute() adds an attribute to a tag by tag + which or by path
 removeAttribute() removes an attribute from a tag by tag + which or by path
 setAttribute() sets an attribute on a tag if that tag exists by tag + which or by path
 clearTagAttributes() removes all attibutes from a tag by tag + which or by path
 
 */

class ofApp : public ofBaseApp{
    
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
    
    ofxXmlSettings settings;
    // Fonts
    ofTrueTypeFont TTF;
    float textSize;
    
    // Input xml
    const string xmlFileName = "new-york.xml";
    const string xmlDebugFileName = "test.xml";
    
    // Xml parse tasks queue
    ofx::TaskQueue queue;
    XmlParseTask * xmlParseTask;
    
    bool customNotificationReceived;
    string customNotificationMessage;
    
    void onTaskQueued(const ofx::TaskQueueEventArgs& args);
    void onTaskStarted(const ofx::TaskQueueEventArgs& args);
    void onTaskCancelled(const ofx::TaskQueueEventArgs& args);
    void onTaskFinished(const ofx::TaskQueueEventArgs& args);
    void onTaskFailed(const ofx::TaskFailedEventArgs& args);
    void onTaskProgress(const ofx::TaskProgressEventArgs& args);
    
    void onTaskCustomNotification(const ofx::TaskCustomNotificationEventArgs& args);
};

