//
//  XmlParseThread.cpp
//  svgMapsParser
//
//  Created by Valerio Viperino on 12/08/16.
//
//

#include "XmlParseTask.hpp"
#include "Poco/TaskNotification.h"
#include "ofUtils.h"

// Constructor
XmlParseTask::XmlParseTask(string& name, float target)
:Poco::Task(name),_targetNumber(target),_currentNumber(0){
    rng.seed();
}

// Destructor
XmlParseTask::~XmlParseTask(){}

// Run the actual xml parsing thread
void XmlParseTask::runTask(){
    
    taskHasEnded = false;
    
    string xmlFileName = "roma.xml";
    
    cout << "Loading " << xmlFileName << endl;
    
    // A list of the attributes that we want to remove together with the path
    // text, rects with roads names inside, purple airplanes
    vector<string> attributesToBeRemoved {
        // text
        "stroke-opacity:0.6",
        // white stroke around text
        "stroke:rgb(82.745098%,61.568627%,64.705882%)",
        "stroke:rgb(99.607843%,92.54902%,83.529412%)",
        // rects
        "fill:rgb(83.137255%,66.666667%,66.666667%)", "fill:rgb(95.294118%,89.019608%,81.176471%)",
        "fill:rgb(92.54902%,80.392157%,81.960784%)", "fill:rgb(94.901961%,84.313725%,80.784314%)",
        "fill:rgb(92.156863%,85.882353%,90.980392%)", "fill:rgb(95.294118%,89.019608%,81.176471%)",
        // purple airplanes
        "fill:rgb(51.764706%,38.039216%,76.862745%)"
    };
    
    // ATTEMPT 2 : a more conscious one
    if(settings.loadFile(xmlFileName)){
        
        cout << xmlFileName << " loaded!" << endl;
        
        settings.pushTag("svg");
        // STEP 1: set the paths beneath the glyph to 0.0
        int numOfTargetPaths = 0;
        int currentPathTagNum = 0;
        float totalPathsNum = 0;
        string targetPathAttrToFind = "stroke-opacity:0.6";
        string newAttributeValue = "stroke-opacity:0.0";
        
        currentOperation = "Searching and removing <path>s with inside <defs>";
        
        // Look for target attributes inside svg->defs->g->symbol
        
        for(int d = 0; d < settings.getNumTags("defs"); d++){
            settings.pushTag("defs", d);
            for(int g = 0; g < settings.getNumTags("g"); g++){
                // Enter in g
                settings.pushTag("g", g);
                int numofsymbols = settings.getNumTags("symbol");
                cout << "Num of <symbol>s: " << numofsymbols << endl;
                // Loop inside symbols and remove paths if they match with our target attributes
                for(int snum = 0; snum < numofsymbols; snum++){
                    
                    // Enter in symbol
                    settings.pushTag("symbol", snum);
                    
                    string pathStyle = settings.getAttribute("path", "style", "");
                    
                    //cout << "<symbol overflow=" << settings.getAttribute("symbol", "overflow", "") << ">" << endl;
                    //cout << "<path style=" << pathStyle << ">" << endl;

                    // If the path has the attribute value we're looking for
                    // then remove the attribute
                    if(pathStyle.find("stroke:none") != string::npos){
                        //cout << "Removed: " << pathStyle << endl;
                        totalPathsNum++;
                        settings.clear();
                    }
                    
                    currentCounter = snum;
                    currentMaximum = numofsymbols;
                    
                    settings.popTag(); // g level
                }
                settings.popTag(); // defs level
                settings.clearTagContents("g", g);
            }
            settings.popTag(); // svg level
        }
        
        cout << "Removed " << totalPathsNum << " paths" << endl;
        currentOperation = "Searching and removing <path>s with " + targetPathAttrToFind;
        currentOperation += ", removing orangish/pinkish rects, airplanes, stroked gray text";
        // TODO: remove paths where style contains rgb(82.4,61.2,64.3)
        
        // Search for <g>s under main <svg>
        cout << "Num of <g>: " << settings.getNumTags("g") << endl;
        // Loop inside this <g>s
        for(int gnums = 0; gnums < settings.getNumTags("g"); gnums++){
            settings.pushTag("g", gnums);
            totalPathsNum = settings.getNumTags("path");
            cout << "Num of paths: " << totalPathsNum << endl;
            // Search only with paths with stroke-opacity:0.6
            for(int pnum = 0; pnum < totalPathsNum; pnum++){
                
                if(SKIP_OPERATION_1) { cout << "Skipping search of " << targetPathAttrToFind << " paths" << endl; break; }
                
                float pnumf = (float) pnum;
                // In our custom tasks, we must regularly see if this task is cancelled.
                if (isCancelled()) goto taskCancelled;
                
                // If cancelled, sleep will also return true, require us to break.
                if (sleep(10)) goto taskCancelled;
                
                // Inform the task of the progress
                if(totalPathsNum != 0){
                    //cout << "Path num: " << pnum << ", progress: " << pnum / totalPathsNum << endl;
                    setProgress(pnumf / totalPathsNum);
                }
                
                string pathStyle = settings.getAttribute("path", "style", "", pnum);
                
                // If the path has the attribute value we're looking for
                // then remove the attribute
                for(int a = 0; a < attributesToBeRemoved.size(); a++){
                    if(pathStyle.find(attributesToBeRemoved[a]) != string::npos){
                        numOfTargetPaths++;
                        settings.pushTag("path", pnum);
                        settings.clear();
                        settings.popTag();
                    }
                    
                }
                /** OLD CODE
                if(pathStyle.find(targetPathAttrToFind) != string::npos){
                    //cout << "FOUND A MATCH:" << endl;
                    //cout << pathStyle << endl;
                    numOfTargetPaths++;
                    // replace "stroke-opacity:0.6" with "stroke-opacity:0.0"
                    // pathStyle.replace(pathStyle.find(targetPathAttrToFind), newAttributeValue.length(), newAttributeValue);
                    // cout << "Modified string: " << pathStyle << endl << endl;
                    // settings.addAttribute("path", "style", pathStyle, pnum);
                    settings.removeTag("path", pnum);
                }
                 */
                
                currentCounter = pnum;
                currentMaximum = totalPathsNum;
            } // end of path search
            
            // TODO: set visibility of text (glyphs) to hidden
            // svg -> g -> g -> use
            // Remove all use tags where xlink:href contains the word #glyph
            
            currentOperation = "Searching for <use> with " + targetPathAttrToFind + " value for xlink:href";
            
            float totalNumOfSubG = settings.getNumTags("g");
            cout << "Num of g children: " << totalNumOfSubG << endl;
            // Loop inside every g child
            for(int subgnum = 0; subgnum < totalNumOfSubG; subgnum++){
                
                if(SKIP_OPERATION_2) { cout << "Skipping search of use tags" << endl; break; }
                
                // In our custom tasks, we must regularly see if this task is cancelled.
                if (isCancelled()) goto taskCancelled;
                
                // If cancelled, sleep will also return true, require us to break.
                if (sleep(10)) goto taskCancelled;
                
                float subgnumf = (float) subgnum;
                // Inform the task of the progress
                if(totalNumOfSubG != 0) setProgress(subgnumf / totalNumOfSubG);
                
                settings.pushTag("g", subgnum);
                
                // Check if g tag has a <use> child
                int numOfUse = settings.getNumTags("use");
                // cout << "Num of <use>: " << numOfUse << endl;
                // Loop inside each use tag and remove them
                // if their xlink:href contains the word glyph
                if(numOfUse > 0){
                    for(int usenum = 0; usenum < numOfUse; usenum++){
                        if(settings.getAttribute("use", "xlink:href", "").find("glyph") != string::npos){
                            settings.pushTag("use", usenum);
                            settings.clear();
                            settings.popTag();
                        }
                    }
                }
                
                currentCounter = subgnum;
                currentMaximum = totalNumOfSubG;
                
                settings.popTag();
            }
            settings.popTag();
        }
        
        cout << "Num of paths removed: " << numOfTargetPaths << endl;
        
        settings.popTag();
    }
    
    settings.saveFile("output.xml");
    settings.saveFile("output.svg");
    
    taskCancelled:
    cout << "Task cancelled" << endl;
    
    cout << "Done!" << endl;
    
    taskHasEnded = true;
    
    while (_currentNumber < _targetNumber){
        // Generate a random increment to add.
        _currentNumber = std::min(_currentNumber + ofRandom(1), _targetNumber);
        
        //setProgress(_currentNumber / _targetNumber); // report progress
        
        // We occasionally post a data notification, using a string.
        // Our TaskQueue can receive and process the
        // corresponding Poco::TaskCustomNotification, e.g.
        //
        // Poco::TaskCustomNotification<std::string> or
        // Poco::TaskCustomNotification<int> or
        
//        float r = rng.nextFloat();
//        
//        if (r > 0.999){
//            std::string txt = "Here's a random number: " + ofToString(r);
//            postNotification(new Poco::TaskCustomNotification<std::string>(this, txt));
//        }
//        else if (r > 0.998){
//            // Send a task notification that is not handled by the onTaskData event.
//            postNotification(new Poco::TaskCustomNotification<int>(this, _currentNumber));
//        
//        }
//        else if (r > 0.997){
//            // We occasionally throw an exception to demonstrate error recovery.
//            throw Poco::Exception("Random Exception " + ofToString(r));
//        }
    }
    // Finished.
}

// Get information on the current operation performed
string XmlParseTask::getCurrentOperation(){
    return currentOperation;
}
// Get the current progress ratio
vector<int> XmlParseTask::getCurrentProgressRatio(){
    vector<int> ratio;
    ratio.push_back(currentCounter);
    ratio.push_back(currentMaximum);
    return ratio;
}

// Get the status of the task
bool XmlParseTask::hasEnded(){
    return taskHasEnded;
}
