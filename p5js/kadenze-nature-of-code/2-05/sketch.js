var particles;
var nOfParticles = 1;
var attractor;
// FORCES
var gravity;
var wind;
var slider;

function setup() {
  createCanvas(640, 360);
  particles = [];
  for(var c = 0; c < nOfParticles; c++){
    particles.push(new Particle(random(0,width),random(0,height), random(1,5)));
  }
  gravity = createVector(0, 0.9);
  attractor = new Attractor(width/2, height/2, 10);
  //slider = createSlider(0, 100, 0);
  //slider.position(10, 10);
}

function draw() {
  background(51);
  for(var i = 0; i < particles.length; i++){
    particles[i].update();
    //particles[i].edges();
    //particles[i].addFriction();
    //particles[i].applyForce(gravity);
    var attractionForce = attractor.calculateAttraction(particles[i]);
    particles[i].applyForce(attractionForce);
    // Render
    particles[i].render();
    attractor.update();
    attractor.render();
  }
}
