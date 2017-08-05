function FlowField(w, h, cSize) {
  this.cellSize = cSize;
  this.columns = w/this.cellSize;
  this.rows = h/this.cellSize;
  this.grid = [[]];
  
  this.generate = function(){
    print("Cols: " + this.columns + " Rows: " + this.rows);
    var yoff = 0;
    for(var y = 0; y <= this.rows; y++){
      this.grid[y] = [];
      var xoff = 0;
      for(var x = 0; x <= this.columns; x++){
        xoff += 0.1;
        //noiseSeed(random(2));
        var theta = map(noise(yoff,xoff), 0, 1, 0, TWO_PI);
        //this.field.push(p5.Vector.fromAngle(theta));
        //this.grid[y][x] = createVector(cos(theta, sin(theta)));
        this.grid[y][x] = p5.Vector.fromAngle(theta);
      }
      yoff += 0.1;
    }
  }
  
  this.calculateAttraction = function(vehicle){
    var attraction = 0;
    for(var y = 0; y <= this.rows; y++){
      for(var x = 0; x <= this.columns; x++){
        
        var leftBound = createVector((x*this.cellSize) - (this.cellSize/2), (y*this.cellSize) - (this.cellSize/2));
        var rightBound = createVector((x*this.cellSize) + (this.cellSize/2), (y*this.cellSize) + (this.cellSize/2));
        
        //print(
        //"Vehicle pos: " + 
        //vehicle.pos.x + " " + vehicle.pos.y + " : " +
        //leftBound.x + " " + leftBound.y + ", " + rightBound.x + " " + rightBound.y
        //);
        
        if(vehicle.pos.x >= leftBound.x && vehicle.pos.x <= rightBound.x){
          if(vehicle.pos.y >= leftBound.y && vehicle.pos.y <= rightBound.y){
            // Visualise the boundary
            //rect(x*this.cellSize, y*this.cellSize, this.cellSize, this.cellSize);
            /*
            noStroke();
            fill(255, 0, 0, 80);
            ellipse(leftBound.x, leftBound.y, 8, 8);
            fill(0, 0, 255, 80);
            ellipse(rightBound.x, rightBound.y, 8, 8);
            */
            
            attraction = this.grid[y][x];  
          }
        }
      }
    }
    return attraction;
  }
  
  this.render = function(){
    noFill();
    for(var y = 0; y < this.rows; y++){
      for(var x = 0; x < this.columns; x++){
        /* Visualize each single cell
        stroke(0);
        rect(x*this.cellSize, y*this.cellSize, this.cellSize, this.cellSize);
        */
        push();
        translate(x*this.cellSize + (this.cellSize/2), y*this.cellSize + (this.cellSize/2));
        rotate(this.grid[y][x].heading());
        var vtxStart = createVector(-this.cellSize/2, 0);
        var vtxEnd = createVector(this.cellSize/2, 0);
        
        // FOR DEBUGGING
        // Visualize  the translation matrix origin
        /**
        stroke(0);
        fill(255,0,0);
        ellipse(0, 0, 12, 12);
        // Circles used for finding start and end of the lines
        // START
        noStroke();
        fill(0,0,255,128);
        ellipse(vtxStart.x, vtxStart.y, 12, 12);
        // END
        fill(0,255,0);
        noStroke();
        ellipse(vtxEnd.x, vtxEnd.y, 12, 12);
        */
        // LINE
        stroke(255);
        line(vtxStart.x, vtxStart.y, vtxEnd.x, vtxEnd.y);
        pop();
      }
    }
  }
}