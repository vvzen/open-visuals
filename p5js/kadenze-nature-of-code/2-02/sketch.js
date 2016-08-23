var particle;
var gravity;
var wind;
var slider;

function setup() {
  createCanvas(640, 360);
  particle = new Particle();
  gravity = createVector(0, 0.9);
  slider = createSlider(0, 100, 0);
  slider.position(10, 10);
}

function draw() {
  wind = createVector(random(-(slider.value()/10), slider.value()/10), 0);
  background(51);
  particle.update();
  particle.edges();
  particle.applyForce(wind);
  particle.applyForce(gravity);
  particle.render();
}