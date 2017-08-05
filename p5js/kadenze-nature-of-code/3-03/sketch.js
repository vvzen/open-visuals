var vehicle;

function setup() {
  createCanvas(640, 360);
  vehicle = new Vehicle(width/2, height/2, 8, 2);
}

function draw() {
  background(51);
  
  var target = createVector(mouseX, mouseY);
  vehicle.seek(target);
  
  vehicle.update();
  vehicle.render();
}