//
//  Boid.cpp
//  boidsSimulation_base
//
//  Created by Valerio Viperino on 21/06/16.
//
//

#include "Boid.hpp"

Boid::Boid(){
}

// GETTERS
float Boid::getMaxSpeed(){
    return maxSpeed;
}

float Boid::getMaxSteer(){
    return maxSteer;
}

ofVec2f Boid::getVel(){
    return vel;
}

// Initialise object
void Boid::setup(float x, float y, float mSize, float mMass){
    
    // Vectors
    pos.set(0,0);
    previousPos.set(0, 0);
    vel.set(0,0);
    acc.set(0,0);
    // Based on Craig Reynolds boids
    desiredForce.set(0,0);
    
    // Floats
    size = mSize;
    mass = mMass;
}

// Basic behoviour
void Boid::update(){
    // Store previous position
    previousPos = pos;
    vel += acc;
    vel.limit(maxSpeed);
    pos += vel;
    acc *= 0;
}
// Used to apply a force
// since f = ma, a = f/m
void Boid::applyForce(ofVec2f force){
    force /= mass;
    acc += force;
}


void Boid::edges(){
    if(pos.x > ofGetWidth()){
        pos.x = 0;
    }
    else if(pos.x < 0){
        pos.x = ofGetWidth();
    }
    if(pos.y > ofGetHeight()){
        pos.y = 0;
    }
    else if(pos.y < 0){
        pos.y = ofGetHeight();
    }
}

/** STEERING BEHAVIOURS */

void Boid::flock(vector<Boid*> boids){
    ofVec2f separation = Boid::separate(boids);
    ofVec2f alignment = Boid::align(boids);
    // Arbitrarily weight the forces
    separation.scale(0.2f);
    alignment.scale(4);
    applyForce(separation);
    applyForce(alignment);
}

// Group behaviours: alignment
ofVec2f Boid::align(vector<Boid *> boids){
    float maxDistance = 40.0f;
    ofVec2f totalAlignment(0,0);
    int boidsCount = 0;
    // For every boids, check if it's within distance threshold
    for(unsigned int i = 0; i < boids.size(); i++){
        float distance = ofDist(boids[i]->pos.x, boids[i]->pos.y, pos.x, pos.y);
        if(distance > 0 && distance < maxDistance){
            // Sum every velocity
            totalAlignment += boids[i]->vel;
        }
    }
    if(boidsCount > 0){
        // Find an average velocity
        totalAlignment /= boidsCount;
        totalAlignment.scale(maxSpeed);
        ofVec2f steering = totalAlignment - vel;
        steering.limit(maxSteer);
        return steering;
    }
    else {
        return ofVec2f(0,0);
    }
}

// Group behaviours: separation
ofVec2f Boid::separate(vector<Boid*> boids){
    ofVec2f totalRepulsion(0,0);
    float repulsionMag = 0;
    int boidsCount = 0;
    // For every boid, check if there are others within a radius treshold
    for(int i = 0; i < boids.size(); i++){
        float distance = ofDist(pos.x, pos.y, boids[i]->pos.x, boids[i]->pos.y);
        // If boid is whitin range
        if(distance > 0 && distance < SEPARATION_THRESHOLD){
            // Calculate vector pointing away from current boid
            ofVec2f newRepulsion = boids[i]->pos - pos;
            // Normalize direction and reverse it
            newRepulsion.normalize();
            newRepulsion *= -1;
            // Magnitude is inversely proportional to distance
            // + distance - mag
            // - distance + mag
            repulsionMag += distance;
            newRepulsion /= distance;
            // Multiply the mag for better aestethic result
            newRepulsion *= SEPARATION_MULTIPLIER;
            totalRepulsion += newRepulsion;
            boidsCount++;
        }
    }
    // Apply repulsion force
    if(boidsCount > 0){
        ofVec2f steering(0,0);
        totalRepulsion /= boidsCount;
        totalRepulsion.limit(maxSpeed);
        steering = totalRepulsion - vel;
        steering.limit(maxSteer);
        applyForce(steering);
    }
    else {
        return ofVec2f(0,0);
    }
}

// Arrive at a target
void Boid::arrive(ofVec2f target){
    ofVec2f desired = target - pos;
    float distance = desired.length();
    // Smooth arrive at the target
    // If the boid is within a certain distance form the target,
    // set the magnitude based on the distance from the target
    // If distance = 100 then mag is maxSpeed
    // If distance = 50 then mag is maxSpeed * 0.5
    // If distance = 25 then mag is maxSpeed * 0.25
    if(distance < 100){;
        float m = ofMap(distance, 0, 100, 0, maxSpeed);
        desired.normalize();
        desired *= m;
    }
    else {
        desired.normalize();
        desired *= maxSpeed;
    }
    // Using Reynolds formula
    // Steer = desire - velocity
    ofVec2f steeringForce = desired - vel;
    steeringForce.limit(maxSteer);
    applyForce(steeringForce);
}

void Boid::render(ofColor c){
    
    ofSetColor(c);
    ofFill();
    // Find the angle between X axis and the velocity vector
    float theta = atan2(vel.y * 10, vel.x * 10);
    theta = ofRadToDeg(theta);
    
    ofPushStyle();
    //ofSetLineWidth(ofMap(sin(theta), -1, 1, 1, 2));
    ofDrawLine(previousPos.x, previousPos.y, pos.x, pos.y);
    ofPopStyle();
    
    /**
    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
    // Rotate based on heading of velocity vector
    ofRotate(90);
    ofRotate(theta);
    // Draw the triangle
    ofBeginShape();
    ofVertex(0, -size*2);
    ofVertex(-size, size*2);
    ofVertex(size, size*2);
    ofEndShape();
    ofPopMatrix();
     */
}

