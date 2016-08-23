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
  
  this.edges = function(){
    if(this.pos.x > width){
      this.pos.x = 0;
    }
    else if(this.pos.x < 0){
      this.pos.x = width;
    }
    if(this.pos.y < 0){
      this.pos.y = height;
    }
    else if(this.pos.y > height){
      this.pos.y = 0;
    }
  }
  
  // Used to apply a force since f = ma; a = f/m
  this.applyForce = function(force){
    // Make a copy of the force passed
    var f = force.copy();
    f.div(this.mass);
    this.acc.add(f);
  }
  
  // steering = desired - velocity
  this.arrive = function(target){
    var desired = p5.Vector.sub(target, this.pos);
    
    // Arrival behaviour
    // Distance between the vehicle and the target itself
    var distance = desired.mag();
    // If the vehicle is within a certain distance
    if(distance < 100){
      // Set the magnitude based on the distance from the target
      // If distance = 100 then mag is maxSpeed
      // If distance = 50 then mag is maxSpeed * 0.50
      // When distance = 0 then mag is maxSpeed * 0 --> 0
      // and so on..
      var m = map(distance, 0, 100, 0, this.maxSpeed);
      desired.setMag(m);
    }
    else {
      // Limit the mag of the desired force
      desired.setMag(this.maxSpeed);
    }
    // Calculate the steering force the normal way
    var steeringForce = p5.Vector.sub(desired, this.vel);
    steeringForce.limit(this.maxSteer);  
    this.desiredForce = desired;
    this.applyForce(steeringForce);
  }
  
  this.render = function(col){
    noStroke();
    fill(col);
    
    /** MY OLD WAY
    push();
    translate(this.pos.x, this.pos.y);
    // Rotate according to velocity direction
    angleMode(RADIANS);
    rotate(this.desiredForce.heading() + PI/2);
    triangle(-1*this.size, this.size, 0,-2*this.size, this.size, this.size);
    //triangle(this.size, this.size*2.5, this.size*1.93, this.size*0.66, this.size*2.86, this.size*2.5);
    pop();
    */
    
    /** SHIFFMAN'S WAY */
    var theta = this.vel.heading() + PI/2;
    push();
    translate(this.pos.x, this.pos.y);
    rotate(theta);
    beginShape();
    vertex(0, -this.size*2);
    vertex(-this.size, this.size*2);
    vertex(this.size, this.size*2);
    endShape(CLOSE);
    pop();
  }
}