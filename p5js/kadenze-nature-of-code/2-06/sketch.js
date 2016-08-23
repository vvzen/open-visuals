/* FRICTION **/

var particles;
var nOfParticles = 4;
var liquid;

// FORCES
var gravity;
var wind;
var slider;

function setup() {
  createCanvas(640, 360);
  particles = [];
  for(var c = 0; c < nOfParticles; c++){
    particles.push(new Particle(random(0,width),random(0,height/4), random(1,5)));
  }
  gravity = createVector(0, 2);
  liquid = new Liquid(0, height/2, width, height/2);
  //slider = createSlider(0, 100, 0);
  //slider.position(10, 10);
}

function draw() {
  background(51);
  for(var i = 0; i < particles.length; i++){
    particles[i].edges();
    particles[i].applyForce(gravity);
    if(particles[i].pos.y >= liquid.pos.y){
      var dragForce = liquid.calculateDrag(particles[i]);
      particles[i].applyForce(dragForce);
    }
    particles[i].update();
    // Render
    particles[i].render();
    liquid.update();
    liquid.render();
  }
}
