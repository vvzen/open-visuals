import controlP5.*;

// Size of the "quads"
int scale = 10;
int columns, rows;
// Size of the landscape
int w = 1980;
int h = 1400;
float[][] plane;
// X offset of Perlin noise
float noiseTimeX = 0;
// Y offset of Perlin noise
float noiseTimeY = 0;
float angle = 0;
float flyingSpeed = 0;

void setup(){
  size(1024,720,P3D);
  columns = w / scale;
  rows = h / scale;
  plane = new float[columns][rows];
  // name, default value (float), x, y, width, height
}

void draw(){
  background(21);
  // Visualize Framerate
  fill(255);
  text((int)frameRate+" fps", width/8, height/12);
  fill(50);
  //noFill();
  noStroke();
  //stroke(51);
  pushMatrix();
  // Scrolling the y axis of the perlin noise
  // to simulate the camera movement
  float flyingVel = map(mouseX, 0, width, 0.001, 0.1);
  flyingSpeed -= 0.1;
  println(flyingVel);
  noiseTimeY = flyingSpeed;
  for(int y = 0; y < rows; y++){
    noiseTimeX = 0;
    for(int x = 0; x < columns; x++){
      plane[x][y] = map(noise(noiseTimeX, noiseTimeY), 0, 1, 0, mouseX);
      noiseTimeX += 0.1;
    }
    noiseTimeY += 0.1;
  }
  // LIGHTS
  //angle += 0.1; // used to test position of the light with sin()
  //pointLight(0, 0, 255, width, 120, width/4);
  directionalLight(255, 255, 255, 0, 240, 0);
  // Getting in right position for drawing
  translate(width/2, height/2, 0);
  rotateX(PI/3);
  translate(-w/2, -h/2, 0);
  // Drawing the triangles strip
  for(int y = 0; y < rows-1; y++){
    beginShape(TRIANGLE_STRIP);
    for(int x = 0; x < columns; x++){
      vertex(x*scale, y*scale,     plane[x][y]);
      vertex(x*scale, (y+1)*scale, plane[x][y+1]);
      //rect(x*scale, y*scale, scale, scale);
    }
    endShape();
  }
  popMatrix();
}