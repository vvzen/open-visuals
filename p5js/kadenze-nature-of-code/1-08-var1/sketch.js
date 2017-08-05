var walker;
var time = 0;

function setup() {
  createCanvas(1024, 1024);
  background(21);
  colorMode(HSB, 360, 100, 100, 100);
  blendMode(ADD);
  walker = new Walker();
}

function draw() {
  //text(frameCount, width/4, height/4);
  walker.update();
  walker.render();
}

// A walker object
function Walker() {
  this.pos = createVector(width / 2, height / 2);
  this.vel = createVector(0, 0);

  this.update = function() {
    if (frameCount % 4 == 1) {
      this.acc = createVector(random(-0.1, 0.1), random(-0.1, 0.1));
    } else {
      // Follow mouse code
      target = createVector(width / 2, height / 2);
      this.acc = p5.Vector.sub(target, this.pos);
      this.acc.normalize();
      this.acc.mult(0.10);
    }
    //this.acc = createVector(random(-1,1),random(-1,1));
    this.vel.add(this.acc);
    this.pos.add(this.vel);

  }

  this.render = function() {
    noStroke();
    time+=0.02;
    if(time > 260){
      time = 220;
    }
    // Size is inversely proportional to distance to center of canvas
    this.size = map(sqrt(p5.Vector.sub(createVector(width/2,height/2), this.pos).magSq()), 0, width/2, 6, 2);
    fill(time, map(this.size,2,6,50,100), 30, 15);
    //print(time);
    ellipse(this.pos.x, this.pos.y, this.size, this.size);
  }
}

// Save image on mouse click
function mouseClicked() {
  saveCanvas('p5js_generator_' + frameCount + ' .png');
}
