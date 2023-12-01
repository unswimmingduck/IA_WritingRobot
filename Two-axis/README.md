# Two-axis Writing Robot
  This type of robot relies on screws for transmission and stepper motors for drive. This robor use Arduino uno to control the motor and other transducers to achieve drawing and writing. The following shows how this robot looks like. If you want to know its 3D structure, you could find its structure in .step format in [here](Two-axis/display).
![](https://github.com/unswimmingduck/IA_WritingRobot/blob/main/Two-axis/display/model.png)

## Drawing
&emsp;&emsp; As we introduce in the first page, this robot apply point-to-point-comparasion interpolation algorithm. You can set some points in the code to allow the robot to draw some pictures you want. Users can use the code in [./control](https://github.com/unswimmingduck/IA_WritingRobot/tree/main/Two-axis/control) to know how we achieve point-to-point-comparasion interpolation algorithm.  
&emsp;&emsp; In the file [./control](https://github.com/unswimmingduck/IA_WritingRobot/tree/main/Two-axis/control), we provide two examples: 
* [two-axis-writing-robot-drawing.ino](https://github.com/unswimmingduck/IA_WritingRobot/blob/main/Two-axis/control/two-axis-writing-robot-drawing.ino)
* [drawing_sin_cos.ino](https://github.com/unswimmingduck/IA_WritingRobot/blob/main/Two-axis/control/drawing_sin_cos.ino)
   
&emsp;&emsp; [two-axis-writing-robot-drawing.ino](https://github.com/unswimmingduck/IA_WritingRobot/blob/main/Two-axis/control/two-axis-writing-robot-drawing.ino) shows how we use point-to-point-comparasion interpolation algorithm to draw rectangle. In this example, we define a struct named Point which save the data of the target point. We define 4 points in the example to drawing rectangle in 50 * 100. So, if you want to draw more complex pictures or other things, you can add or change the points to let the robot drawing the picture you want.
```
struct Point{
    float x;
    float y;
};
```
&emsp;&emsp; [drawing_sin_cos.ino](https://github.com/unswimmingduck/IA_WritingRobot/blob/main/Two-axis/control/drawing_sin_cos.ino) how we use point-to-point-comparasion interpolation algorithm to draw cos/sin function. This example could perfectly show point-to-point-comparasion interpolation algorithm can drawing some complex picture.

## Display
The following shows the robot drwaing some pictures.
![](https://github.com/unswimmingduck/IA_WritingRobot/blob/main/Two-axis/display/writing_show.mp4)
