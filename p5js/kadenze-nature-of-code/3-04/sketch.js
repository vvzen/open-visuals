/** ARRIVE */

var vehicles;
var VEHICLES_NUM = 1;

function setup() {
  createCanvas(640, 360);
  //vehicle = new Vehicle(width/2, height/2, 8, 2);
  vehicles = [];
  for(var i = 0; i < VEHICLES_NUM; i++){
    vehicles.push(new Vehicle(random(width), random(height), 8, 2));
  }
}

function draw() {
  background(51);
  var target = createVector(mouseX, mouseY);
  for(var i = 0; i < vehicles.length; i++){
    vehicles[0].applyForce(createVector(noise(i), noise(i)));
    vehicles[0].arrive(target);
    vehicles[0].update();
    vehicles[0].render(); 
  }
}