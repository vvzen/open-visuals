//
//  FlowField.cpp
//  drawingBoids_01
//
//  Created by Valerio Viperino on 28/06/16.
//
//

#include "FlowField.hpp"

FlowField::FlowField(){};

void FlowField::setup(float mCellSize){
    
    cellSize = mCellSize;
    cols = ofGetWidth()/cellSize;
    rows = ofGetHeight()/cellSize;
    
    cellsCount = 0;
    float yoff = 0;
    // Create a 2d grid of vectors using a perlin noise
    for(int y = 0; y <= rows; y++){
        float xoff = 0;
        for(int x = 0; x <= cols; x++){
            xoff+=NOISE_X_OFFSET;
            float theta = ofNoise(xoff, yoff) * 360;
            //cout << theta << endl;
            anglesGrid.push_back(theta);
            cellsCount++;
            
            // Store the 2d vector
            // Convert polar coordinates to cartesian
            ofVec2f attraction(cos(theta), sin(theta));
            attraction.scale(2);
            attractionsGrid.push_back(attraction);
        }
        yoff+=NOISE_Y_OFFSET;
    }
    
    cout << "Cells count: " << cellsCount << endl;
    cout << "Cols: " << cols << " Rows: " << rows << endl;
}

//------------------
ofVec2f FlowField::computeAttraction(Boid * boid, vector<Boid *> boids){
    ofVec2f attraction(0,0);
    
    // Get cell for boid given its x and y
    for(int y = 0; y < rows; y++){
        for(int x = 0; x < cols; x++){
            
            ofVec2f leftBound(x*cellSize - cellSize/2, y*cellSize - cellSize/2);
            ofVec2f rightBound(x*cellSize + cellSize/2, y*cellSize + cellSize/2);
            
            if(boid->pos.x > leftBound.x && boid->pos.x < rightBound.x){
                if(boid->pos.y > leftBound.y && boid->pos.y < rightBound.y){
                    
                    int position = x + y * cols;
                    ofVec2f attraction = attractionsGrid[position];
                    attraction.scale(0.5);
                    
                    return attraction;
                }
            }
        }
    }
}

// Displays the vector field with a bunch of lines
void FlowField::render(){
    ofPushStyle();
    ofNoFill();
    for(int y = 0; y <= rows; y++){
        for(int x = 0; x <= cols; x++){
            //float theta = ofRandom(360);
            int position = (int) x + (y * cols);
            float theta = anglesGrid[position];
            ofPushMatrix();
            // Draw a line rotated depending on the noise value
            ofTranslate(x*cellSize, y*cellSize);
            ofRotate(theta);
            ofSetColor(255, 0, 0);
            ofDrawLine(0, 0-cellSize/2, 0, 0+cellSize/2);
            // Draw boundaries
//            ofSetColor(ofColor::blueViolet);
//            ofDrawRectangle(0, 0, cellSize, cellSize);
            ofPopMatrix();
        }
    }
    ofPopStyle();
}
