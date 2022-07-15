# Practical Math

Snippets of reusable math knowledge/formulas that I come back over and over again to revisit.. (since I suck at math!).
I write them in either pseudo code, python or p5js or whatever it's easier and more useful (to copy/paste);

## Indices

### XY pos from index
```javascript
function XYFromIndex(i, numColumns){

  let x = Math.floor(i / numColumns) ;
  let y = i % numColumns;

  return createVector(x, y);
}
```

### Index from XY
```javascript
function indexFromXY(x, y, numColumns){
  return (numColumns * x) + y;
}
```

### Find index once grid is subdivided

```javascript 
function remapIndex(x, y, numColumns, subdivisionLevel){
  let numNewCols = (numColumns * subdivisionLevel) - (subdivisionLevel - 1);

  console.log(`numColumns: ${numColumns}`);
  console.log(`numNewCols: ${numNewCols}`);
  let newX = x * subdivisionLevel;
  let newY = y * subdivisionLevel;
  
  console.log(`x: ${x}, y: ${y}`);
  console.log(`newX: ${newX}, newY: ${newY}`);
  
  return indexFromXY(newX, newY, numNewCols);
}
```

For example:

```javascript
const xSize = 4;
const ySize = 3;
const numColumns = xSize + 1;

let oldIndex = 8;
// Subdivide the grid 4 times
const newSub = 4;

let xy = XYFromIndex(oldIndex, numColumns);
console.log(`index: ${oldIndex}, x: ${xy.x} y: ${xy.y}`);

let newIndex = remapIndex(xy.x, xy.y, numColumns, newSub);
console.log(`index ${oldIndex} in a ${xSize}x${ySize} grid is index  ${newIndex} in a ${xSize * newSub}x${ySize * newSub} grid`);
```