/** STEERING BEHAVIOURS : SEPARATION */

var vehicles;
var flowField;
// Maximum number of vehicles
var VEHICLES_NUM = 32;

function setup() {
  createCanvas(640, 320);
  vehicles = [];
  /*
  for(var i = 0; i < VEHICLES_NUM; i++){
    vehicles.push(new Vehicle(random(width), random(height), 4, 2));
  }
  */
}

function draw() {
  background(51);
  var target = createVector(mouseX, mouseY);
  // For each vehicle, use the chosen steering behaviours
  for(var i = 0; i < vehicles.length; i++){
    vehicles[i].arrive(target);
    vehicles[i].separate(vehicles);
    vehicles[i].edges();
    vehicles[i].update();

    var col = color(255);
    vehicles[i].render(col);
  }
}

// Create vehicles on mouse dragged
function mouseDragged(){
  vehicles.push(new Vehicle(mouseX, mouseY, random(3,6), 2));
}
