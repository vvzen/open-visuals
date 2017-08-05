
import controlP5.*;
import ddf.minim.*;
import ddf.minim.analysis.*;

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
// Audio analysis
Minim minim;
AudioInput in;
BeatDetect beat;
float currentPeakVolume = 0;
float oldPeakVolume = 0;

void setup(){
  size(1024,720,P3D);
  columns = w / scale;
  rows = h / scale;
  plane = new float[columns][rows];
  // Audio
  minim = new Minim(this);
  // use the getLineIn method of the Minim object to get an AudioInput
  in = minim.getLineIn(Minim.STEREO, 1024);
  beat = new BeatDetect(in.bufferSize(), in.sampleRate());
}

void draw(){
  oldPeakVolume = in.mix.get(0) * 256;
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
  noiseTimeY = flyingSpeed;
  float peakMapped = lerp(oldPeakVolume, map(currentPeakVolume, 0, 256, 150, 200), 0.6);
  //float lightIntensity = lerp(oldPeakVolume, map(currentPeakVolume, 0, 256, 0, width/2), 0.4);
  println(peakMapped);
  for(int y = 0; y < rows; y++){
    noiseTimeX = 0;
    for(int x = 0; x < columns; x++){
      plane[x][y] = map(noise(noiseTimeX, noiseTimeY), 0, 1, 0, peakMapped);
      noiseTimeX += 0.1;
    }
    noiseTimeY += 0.1;
  }
  // LIGHTS
  //angle += 0.1; // used to test position of the light with sin()
  //pointLight(0, 0, 255, width, 120, width/4);
  directionalLight(255, 255, 255, 0, 240, 0);
  //directionalLight(255, 255, 255, 0, 240, 0);
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
  currentPeakVolume = in.mix.get(0) * 256;
}