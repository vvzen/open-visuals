var particles;
var nOfParticles;
var gravity;
var wind;
var slider;

function setup() {
  createCanvas(640, 360);
  nOfParticles = 2;
  particles = [];
  for(var c = 0; c < nOfParticles; c++){
    particles.push(new Particle(random(0,width),random(0,height), random(1,5)));
  }
  gravity = createVector(0, 0.9);
  slider = createSlider(0, 100, 0);
  slider.position(10, 10);
}

function draw() {
  background(51);
  for(var i = 0; i < particles.length; i++){
    particles[i].update();
    particles[i].edges();
    particles[i].addFriction();
    particles[i].applyForce(gravity);
    particles[i].render();
  }
}

function mousePressed(){
  wind = createVector(12, 0.0);
  console.log("Pushing wind");
  for(var i = 0; i < particles.length; i++){
    particles[i].update();
    particles[i].edges();
    particles[i].applyForce(wind);
    particles[i].addFriction();
    particles[i].render();
  }
}