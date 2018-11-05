# Topic: 2D Graphics Editor

[![Build Status](https://travis-ci.com/AngelaJubeJudy/2D-Graphics-Editor.svg?branch=master)](https://travis-ci.com/AngelaJubeJudy/2D-Vector-Graphics-Editor)

## Compilation Instructions

```bash
git clone --recursive https://github.com/AngelaJubeJudy/2D-Vector-Graphics-Editor
cd 2D-Vector-Graphics-Editor
mkdir build
cd build
cmake ../ 
make
```

## Contents
### *1.1 Triangle Editor*

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
* ENABLED: Every mouse click will [select](/screenshots/1.3_tri1-color9-v2.PNG) the vertex [closer](/screenshots/1.3_tri1-color9-v3.PNG) to the current mouse position. 
After a vertex is [selected](/screenshots/1.3_tri2-color9-v3.PNG), pressing a key from '1' to '9' will change its color. 

1 for [Maroon](/screenshots/1.3_tri1-color1.PNG), 2 for [Firebrick](/screenshots/1.3_tri1-color2.PNG), 3 for [Tomato](/screenshots/1.3_tri1-color3.PNG), 
4 for [Orange](/screenshots/1.3_tri1-color4.PNG), 5 for [Gold](/screenshots/1.3_tri1-color5.PNG), 6 for [Lawn](/screenshots/1.3_tri1-color6.PNG), 
7 for [Spring](/screenshots/1.3_tri1-color7.PNG), 8 for [Dodger](/screenshots/1.3_tri1-color8.PNG), and 9 for [Rosy](/screenshots/1.3_tri1-color9.PNG).  
           
### *1.4 View Control*

key "+" and "-": Scene Zooming Mode
* ENABLED: Increase Decrease the zoom by 20% zooming [in](/screenshots/1.4_ZoomIn.PNG)/[out](/screenshots/1.4_ZoomOut.PNG) in the center of the screen. 

key "w", "a", "s", "d": Scene Translation Mode
* ENABLED: Translate the entire scene respectively [down](/screenshots/1.4_ViewControl_w-2.PNG), [right](/screenshots/1.4_ViewControl_a-2.PNG), [up](/screenshots/1.4_ViewControl_s-2.PNG), [left](/screenshots/1.4_ViewControl_d-2.PNG), by 20% of the window size.

### *1.5 Keyframing*

key "f": Keyframing Mode
* ENABLED: Animates the properties (size, position, or rotation) of objects using keyframe. 
Next frame is stored in a new VBO. 
A timer is used to automate the animation. 

### *1.6 Translation/Scaling/Rotation in Shader*

key "h", "j", "k", "l": Triangle Translation/Scaling/Rotation Mode

* ENABLED: Upload triangles in a single VBO using offsets for drawing them one by one. 
Upload to the GPU the transformation model (as a uniform matrix) that transforms the selected triangle from its canonical position to the current position (obtained by combining translations, scaling and rotations). 

```bash
if(mode.modeH == true){
    rotation << cos(clockwise), -sin(clockwise), 0, 0,
                sin(clockwise), cos(clockwise),  0, 0,
                0,              0,               1, 0,
                0,              0,               0, 1;
    trs <<  trsBack * rotation * trsToOri;
}
...
glUniformMatrix4fv(program.uniform("Translation"), 1, GL_FALSE, trs.data());
```

The transformation has been executed in the vertex shader, and the content of the VBO storing the vertex positions never updated.

```bash
const GLchar* vertex_shader =
            "#version 150 core\n"
                    "in vec2 position;"
                    "in vec3 color;"
                    "uniform mat4 view;"
                    "uniform mat4 Translation;"
                    "out vec3 f_color;"
                    "void main()"
                    "{"
                    "    gl_Position = view * Translation * vec4(position, 0.0, 1.0);"
                    "    f_color = color;"
                    "}";
```
