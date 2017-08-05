/** FLOW FIELD */

var vehicles;
var flowField;
var VEHICLES_NUM = 32;
var ITALY = 0.2;
var GERMANY = 0.9;
var JAPAN = 1.6;
var STATE;

function setup() {
  createCanvas(640, 320);
  vehicles = [];
  STATE = GERMANY;
  for(var i = 0; i < VEHICLES_NUM; i++){
    vehicles.push(new Vehicle(random(width), random(height), 4, 2));
  }
  
  flowField = new FlowField(width, height, 20);
  flowField.generate();
}

function draw() {
  background(51);
  var target = createVector(mouseX, mouseY);
  for(var i = 0; i < vehicles.length; i++){
    vehicles[i].arrive(target);
    var attraction = flowField.calculateAttraction(vehicles[i]);
    
    attraction.mult(STATE);
    vehicles[i].applyForce(attraction);
    vehicles[i].edges();
    vehicles[i].update();
    
    var col = color(255);
    vehicles[i].render(col);
  }
  if(keyIsPressed){
    if(key == 'd'){
      flowField.render();
    }
  }
  drawState();
}

function drawState(){
  textFont("Open Sans");
  textSize(24);
  switch(STATE){
    case ITALY:
      text("ITALY", width/12, height/8);
      break;
    case GERMANY:
      text("GERMANY", width/12, height/8);
      break;
    case JAPAN:
      text("JAPAN", width/12, height/8);
      break;
  }
}