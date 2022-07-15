# Practical Math

Snippets of reusable math knowledge/formulas that I come back over and over again to revisit.. (since I suck at math!).
I write them in either pseudo code, python or p5js or whatever it's easier and more useful (to copy/paste);

## Algebra

### Find function for data points
```python
# Find a polynomial that describes these data points
from numpy.polynomial import Polynomial

# The degree of the resulting polynomial
degree = 4

x = np.array([0, 1, 2, 3, 4,  5,  6,  7,  8,  9, 10, 11, 12])
y = np.array([0, 2, 4, 6, 8, 18, 20, 22, 24, 26, 36, 38, 40])

fit_fn = Polynomial.fit(x, y, deg=degree)

# Now fit_fn can be used to 'predict' y based on an input x
```

To plot this function with matplotlib:

```ipython
%matplotlib inline
import matplotlib
import matplotlib.pyplot as plt

fig, ax = plt.subplots(figsize=(12, 5))

# Real values
ax.plot(x, y, 'b')

# Fitted
ax.plot(x, fit_fn(x), 'r')

ax.legend(["Real Data", "Fitted Function"])

# Avoid scientific notation (I'm a layman)
ax.yaxis.set_major_formatter(matplotlib.ticker.FuncFormatter(lambda x, p: format(int(x), ',')))

# Rotate the x labels
plt.setp(ax.get_xticklabels(), rotation=90, horizontalalignment='right')

plt.show()
```

## Geometry

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