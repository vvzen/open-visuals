//
//  XmlParseThread.hpp
//  svgMapsParser
//
//  Created by Valerio Viperino on 12/08/16.
//
//

#ifndef XmlParseTask_hpp
#define XmlParseTask_hpp

#pragma once

#include <stdio.h>
#include <ofMain.h>
#include "ofxXmlSettings.h"
#include "Poco/Task.h"
#include "Poco/Notification.h"
#include "Poco/Random.h"


class XmlParseTask: public Poco::Task {
    
public:
    XmlParseTask(string& name, float target);
    
    virtual ~XmlParseTask();
    
    // Parse xml and replace different tag attributes
    void runTask();
    
    // Getters
    string getCurrentOperation();
    vector<int> getCurrentProgressRatio();
    bool hasEnded();
    
private:
    float _targetNumber;
    float _currentNumber;
    
    string currentOperation;
    int currentCounter, currentMaximum;
    bool taskHasEnded;
    
    const bool SKIP_OPERATION_1 = false;
    const bool SKIP_OPERATION_2 = false;
    
    Poco::Random rng;
    ofxXmlSettings settings;
};


#endif /* XmlParseTask_hpp */
