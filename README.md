# IA_WritingRobot
&emsp;&emsp; In this project, with the help of my teacher, we used some machine parts to build four kinds of writing robot: two-axis, three-axis, four-wheel, and three-wheel. Then, I based on interpolation algorithm to devised suitable control algorithms to allow those four robots to drawing and writing some complex pictures and charcters.  

![Static Badge](https://img.shields.io/badge/Arduino-make?style=for-the-badge&logo=arduino&logoColor=white&labelColor=rgb(45%2C219%2C207)&color=rgb(45%2C219%2C207)) &emsp;&emsp; 
![Static Badge](https://img.shields.io/badge/inkscape-make?style=for-the-badge&logo=inkscape&logoColor=white&labelColor=black&color=black)&emsp;&emsp; 
![Static Badge](https://img.shields.io/badge/Processing-make?style=for-the-badge&logo=Processing&logoColor=white&labelColor=rgb(108%2C156%2C154)&color=rgb(108%2C156%2C154))

###  There have four kinks of writting robot: 
* Two-axis 
* Three-axis 
* Three-wheels
* Four-wheels
  
Although they have different mechanism structures, they all apply interpolation algorithm to control the drawing and writting behavior.
## 1. Dynamic Algorithm
&emsp;&emsp; The writing robot can be analyzed mathematically to know the relationship between the motion of the actuator and the driver. The idea of the algorithm is: first establish a plane coordinate system, we need to draw the polygonal graphic placed in the coordinate system, so that we can determine the coordinates of each vertex of the graphic, two adjacent vertices to determine a straight line between the coordinates of the points on the straight line through the interpolation calculations, and then the paintbrush in turn along these coordinates to move, complete the drawing.  
&emsp;&emsp; The writing robot realizing the trajectory movement of a straight line requires the X-axis and Y-axis to work together, but there is always a time gap between the X-axis and Y-axis in the movement, so when drawing a diagonal straight line, it actually consists of countless zigzag segments, and when these zigzag segments are infinite, these zigzag segments appear to be a straight line.  
&emsp;&emsp; In order to solve the problem of time gap, we apply the curve machining method used in CNC machining - the point-by-point-comparison interpolation algorithm: point-by-point comparison method for interpolation. Each step of the drawing pen is compared with the coordinate value on the given trajectory, and the direction of deviation is judged according to this comparison, and the direction of the next step is determined.  
&emsp;&emsp; **So in this process, we need to know how to determine the direction of deviation (deviation discrimination), and how to feed the corresponding robot platform. The conventional method is to introduce a deviation quantity Fm, and judge the feed direction by judging the value of Fm and the end point of the quadrant, when Fm≥0 and the end point is in the first and fourth quadrant, feed along the "X" direction, in the second and third quadrant, feed along the -X direction; when Fm<0 and the end point is in the first and second quadrant, feed along the "Y" direction, in the third and fourth quadrant, feed along the -Y direction. The following graph shows how the point-by-point comparison interpolation works:**


```mermaid
graph TD
A(Start) --> B[Deviation Discrimination]
B --> |Fm>=0| C[End Point Direcrtion Discrimination]
C --> |in quadrant one or four| D(One step toward +X)
C --> |in quadrant two or three| E(One step toward -X)
B --> |Fm<0| F[End Point Direcrtion Discrimination]
F --> |in quadrant one or two|I(One step toward +Y)
F --> |in quadrant three or four| J(One step toward -Y)
D --> k[Update deviation]
E --> k
I --> k
J --> k
k --> H[End judgement]
H --> |Yes| l(End)
H --> |No|B
```
## 2. Motor Control according to the Dynamic Algorithm
&emsp;&emsp; Because the four kinds writing robot have different mechanical structure, there should design different motor control algorithms to acomplish the point-by-point-comparison interpolation algorithm. If you want to know special algorithm in special robot, you can choose the special type and see the details of how it realizes its function and point-by-point-comparison interpolation algorithm. 

|Transmission type|Specific types|
|  ----  | ----  |
|screw drive|Two-axis|
