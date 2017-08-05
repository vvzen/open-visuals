function Vehicle(x, y, s, m) {
  this.acc = createVector(0, 0);
  this.vel = createVector(0, 0);
  this.pos = createVector(x, y);
  // Global var for storing the desired force
  this.desiredForce = createVector(0, 0);
  this.size = s;
  this.mass = m;
  this.maxSpeed = 10;
  this.maxSteer = 5;
  
  this.update = function(){
    this.vel.add(this.acc);
    this.vel.limit(this.maxSpeed);
    this.pos.add(this.vel);
    this.acc.mult(0);
  }
  
  // Used to apply a force since f = ma; a = f/m
  this.applyForce = function(force){
    // Make a copy of the force passed
    var f = force.copy();
    f.div(this.mass);
    this.acc.add(f);
  }
  
  // steering = desired - velocity
  this.seek = function(target){
    var desired = p5.Vector.sub(target, this.pos);
    desired.setMag(this.maxSpeed);
    var steeringForce = p5.Vector.sub(desired, this.vel);
    steeringForce.limit(this.maxSteer);
    this.desiredForce = desired;
    this.applyForce(steeringForce);
  }
  
  this.render = function(){
    noStroke();
    fill(255);
    push();
    translate(this.pos.x, this.pos.y);
    // Rotate according to velocity direction
    angleMode(RADIANS);
    rotate(this.desiredForce.heading() + PI/2);
    triangle(-1*this.size, this.size, 0,-2*this.size, this.size, this.size);
    //triangle(this.size, this.size*2.5, this.size*1.93, this.size*0.66, this.size*2.86, this.size*2.5);
    pop();
  }
}