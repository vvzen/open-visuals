var walker;
var time = 0;

function setup() {
  createCanvas(1024, 1024);
  background(35);
  walker = new Walker();
}

function draw() {
  //text(frameCount, width/4, height/4);
  walker.update();
  walker.render();
}

function Walker() {
  this.pos = createVector(width / 2, height / 2);
  this.vel = createVector(0, 0);

  this.update = function() {
    if (frameCount % 3 == 1) {
      this.acc = createVector(random(-0.1, 0.1), random(-0.1, 0.1));
    } else {
      // Follow mouse code
      target = createVector(width / 2, height / 2);
      this.acc = p5.Vector.sub(target, this.pos);
      this.acc.normalize();
      this.acc.mult(0.05);
    }
    //this.acc = createVector(random(-1,1),random(-1,1));
    this.vel.add(this.acc);
    this.pos.add(this.vel);

  }

  this.render = function() {
    noStroke();
    fill(255, 20);
    // Size is inversely proportional to distance to center of canvas 
    this.size = map(sqrt(p5.Vector.sub(createVector(width/2,height/2), this.pos).magSq()), 0, width/2, 2, 8);
    //print(this.size);
    ellipse(this.pos.x, this.pos.y, this.size, this.size);
  }
}

function mouseClicked() {
  saveCanvas('p5js_generator_' + frameCount + ' .png');
}