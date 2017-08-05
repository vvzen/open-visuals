//
//  Boid.hpp
//  boidsSimulation_base
//
//  Created by Valerio Viperino on 21/06/16.
//
//

#ifndef Boid_hpp
#define Boid_hpp

#include <stdio.h>

#include "ofMain.h" // reference to the OF library

#define SEPARATION_THRESHOLD 20
#define SEPARATION_MULTIPLIER 200.0f

class Boid {
    
    public:
    
    // Constructor
    Boid();
    
    // Methods
    void setup(float x, float y, float size, float mass);
    void update();
    void render(ofColor color);
    ofVec2f align(vector<Boid*> boids);
    void arrive(ofVec2f target);
    void applyForce(ofVec2f force);
    void edges();
    void flock(vector<Boid*> boids);
    ofVec2f separate(vector<Boid*> boids);
    
    // Getters
    float getMaxSpeed();
    float getMaxSteer();
    ofVec2f getVel();
    
    // Vars
    ofVec2f pos;
    ofVec2f previousPos;
    
    private:
    
    // Vars
    ofVec2f acc;
    ofVec2f vel;
    ofVec2f desiredForce;
    
    float size;
    float mass;
    const float maxSpeed = 10;
    const float maxSteer = 0.8;
    
};
#endif /* Boid_hpp */
