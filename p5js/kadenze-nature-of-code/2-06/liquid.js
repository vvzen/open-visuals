// Adding mass to our physics world

function Liquid(x, y, w, h) {
  this.pos = createVector(x, y);
  this.vel = createVector(0, 0);
  this.acc = createVector(0, 0);
  this.dragConstant = 0.2;

  this.update = function(){
    this.vel.add(this.acc);
    this.pos.add(this.vel);
    this.acc.mult(0);
  }

  this.render = function(){
    fill(20, 40, 120, 90);
    noStroke();
    rect(this.pos.x, this.pos.y, w, h);
  }

  // Used to calculate the drag on a particle
  this.calculateDrag = function(particle){
    var speed = particle.vel.mag();
    var dragForce = particle.vel.copy();
    // Fdrag = Some generic constant * v^2
    var dragMagnitude = this.dragConstant * Math.pow(speed, 2);
    
    // Direction of drag is inverse of velocity
    dragForce.mult(-1);
    // Get only the direction
    dragForce.normalize();
    // Scale according to some calculations
    dragForce.mult(dragMagnitude);
    return dragForce;
  }
}
