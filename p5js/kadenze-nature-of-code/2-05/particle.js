// Adding mass to our physics world

function Particle(x, y, m) {
  this.pos = createVector(x, y);
  this.vel = createVector(0, 0);
  this.acc = createVector(0, 0);
  this.mass = m;

  this.update = function(){
    this.vel.add(this.acc);
    this.pos.add(this.vel);
    this.acc.mult(0);
  }

  // Function to go after edges
  this.edges = function(){
    if(this.pos.y > height){
      this.vel.y *= -1;
      this.pos.y = height;
    }
    if(this.pos.x > width){
      this.vel.x *= -1;
      this.pos.x = width;
    }
    else if(this.pos.x < 0){
      this.vel.x *= -1;
      this.pos.x = 0;
    }
  }

  this.render = function(){
    fill(255);
    noStroke();
    ellipse(this.pos.x, this.pos.y, this.mass*10, this.mass*10);
  }

  // Used to calculate the addition a force to the particle
  this.applyForce = function(force){
    var f = force.copy();
    f.div(this.mass);
    this.acc.add(f);
  }

  /** This is just a preliminary attempt */
  this.addFriction = function(){
    var friction = this.vel.copy();
    friction.mult(-0.01);
    //console.log("Velocity: " + this.vel.y);
    //console.log("Friction: " + friction.y);
    this.acc.add(friction);
  }
}
