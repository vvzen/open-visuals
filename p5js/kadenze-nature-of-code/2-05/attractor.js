// Adding mass to our physics world

function Attractor(x, y, m) {
  this.pos = createVector(x, y);
  this.vel = createVector(0, 0);
  this.acc = createVector(0, 0);
  this.mass = m;
  this.attractionConstant = 0.1;

  this.update = function(){
    this.vel.add(this.acc);
    this.pos.add(this.vel);
    this.acc.mult(0);
  }

  this.render = function(){
    noStroke();
    fill(255,255,0);
    ellipse(this.pos.x, this.pos.y, this.mass*10, this.mass*10);
  }

  // Used to attract particles
  this.calculateAttraction = function(particle){
    // Fg = m1*m2*G / distance^2
    var force = p5.Vector.sub(this.pos, particle.pos);
    // constrain(5,25) --> Limiting the distance to eliminate extreme results
    var distanceSquared = Math.pow(constrain(force.mag(), 3, 15), 2);
    var strength = (this.mass * particle.mass * this.attractionConstant)/distanceSquared;
    // Get only the dir
    force.normalize();
    // Scale based on the gravitational formula
    force.mult(strength);
    //var gVectorForce = createVector(force.x, force.y).mult(strength);
    return force;

  }
}
