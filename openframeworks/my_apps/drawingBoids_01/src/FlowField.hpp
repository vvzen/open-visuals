//
//  FlowField.hpp
//  drawingBoids_01
//
//  Created by Valerio Viperino on 28/06/16.
//
//

#ifndef FlowField_hpp
#define FlowField_hpp

#include <stdio.h>
#include "ofMain.h" // reference to the OF library
#include "Boid.hpp"

#define NOISE_X_OFFSET 0.0003f
#define NOISE_Y_OFFSET 0.0003f

class FlowField {
    public:
    
    // Vars
    int cols;
    int rows;
    float cellSize;
    vector<float> anglesGrid;
    vector<ofVec2f> attractionsGrid;
    
    // Constructor
    FlowField();
    
    // Methods
    void setup(float mCellSize);
    ofVec2f computeAttraction(Boid * boid, vector<Boid *> boids);
    void render();
    int cellsCount;
};

#endif /* FlowField_hpp */
