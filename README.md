# CSCI-GA 2270-001 Fall2018 Computer Graphics Assignment_2: 2D Vector Graphics Editor

[![Build Status](https://travis-ci.com/NYUGraphics/assignment2-AngelaJubeJudy.svg?token=sppqB6eFKVufzLrtS82k&branch=master)](https://travis-ci.com/NYUGraphics/assignment2-AngelaJubeJudy)

## Compilation Instructions

```bash
git clone --recursive https://github.com/NYUGraphics/assignment2-AngelaJubeJudy
cd assignment2-AngelaJubeJudy
mkdir build
cd build
cmake ../ 
make
```

## Contents
### *1.1 Triangle Soup Editor*

key "i": Triangle Insertion Mode
* ENABLED: Every triple of subsequent mouse clicks will create a triangle. 
The first click will create [the starting point](/screenshots/1.1_preview_StartingPoint.png) of the segment, which will be immediately visualized. 
 
As the mouse is moved, [a preview of a segment](/screenshots/1.1_preview_Segment.png) will appear.

After the second mouse click, [a preview of the triangle](/screenshots/1.1_preview_Triangle.png) will appear.

After the third click, the current preview will transform into [the final triangle](/screenshots/1.1_TriangleInsertionMode.PNG).

key "o": Triangle Translation Mode
* ENABLED: Each mouse click will selected the triangle below the cursor (which will be [highlighted](/screenshots/1.1_TriangleTranslationMode_highlight.PNG)), 
and every movement of the mouse (while keeping the button pressed) will result in a corresponding [translation of the triangle](/screenshots/1.1_TriangleTranslationMode_translate.PNG). 

key "p": Triangle Deletion Mode
*  ENABLED: When the mouse is pressed, [the triangle](/screenshots/1.1_TriangleDeletionMode_1.PNG) below the cursor is [deleted](/screenshots/1.1_TriangleDeletionMode_2.PNG).

### *1.2 Rotation / Scale*

key "h" and " "j": Triangle Rotation Mode
* ENABLED: The triangle selected will rotate around its barycenter by 10 degress [clockwise](/screenshots/1.2_TriangleRotationClockwise.PNG) or [counter-clockwise](/screenshots/1.2_TriangleRotationCounterclockwise.PNG). 

key "k" and " "l": Triangle Scaling Mode
* ENABLED: The triangle selected will scale [up](/screenshots/1.2_TriangleScaleUp.PNG) or [down](/screenshots/1.2_TriangleScaleDown.PNG) by 25% around its barycenter. 

### *1.3 Color*

key "c": Triangle Coloring Mode
* ENABLED: Every mouse click will select the vertex closer to the current mouse position. 
After a vertex is selected, pressing a key from '1' to '9' will change its color. 
           
### *1.4 View Control*

key "+" and "-": Triangle Zooming Mode
* ENABLED: Increase/Decrease the zoom by 20% zooming in/out in the center of the screen. 

key "w", "a", "s", "d": Triangle Zooming Mode
* ENABLED: Translate the entire scene respectively down, right, up, left, by 20% of the window size.

### *1.5 Add Keyframing*

key "f": Triangle Zooming Mode
* ENABLED: Animates the properties (size, position, or rotation) of objects using keyframe. 

### *1.8 Shader Translation/Scaling/Rotation*

key "h", "j", "k", "l": Triangle Scaling/Rotation Mode

* ENABLED: Upload triangles in a single VBO using offsets for drawing them one by one. 
Upload to the GPU the transformation model (as a uniform matrix) that transforms the selected triangle from its canonical position to the current position (obtained by combining translations, scaling and rotations). 
The transformation has been executed in the vertex shader, and the content of the VBO storing the vertex positions never updated.
