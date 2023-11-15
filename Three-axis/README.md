# Three-axis Writing Robot
  This type of robot relies on screws for transmission and stepper motors for drive. This robor use Arduino uno to control the motor and other transducers to achieve drawing and writing. The following shows how this robot looks like. If you want to know its 3D structure, you could find its structure in .step format in [here](Three-axis/display).
![](https://github.com/unswimmingduck/IA_WritingRobot/blob/main/Three-axis/model/Three-axis%20writing%20robot.jpg)

## Drawing
&emsp;&emsp; As we introduce in the first page, this robot apply point-to-point-comparasion interpolation algorithm. You can set some points in the code to allow the robot to draw some pictures you want. Users can use the code in [./control](https://github.com/unswimmingduck/IA_WritingRobot/tree/main/Two-axis/control) to know how we achieve point-to-point-comparasion interpolation algorithm.  
&emsp;&emsp; In the file [./control](https://github.com/unswimmingduck/IA_WritingRobot/tree/main/Two-axis/control), we provide two examples: 
* 3axis_drawing
* gcode_drawing
   
&emsp;&emsp; [3axis_drawing](https://github.com/unswimmingduck/IA_WritingRobot/blob/main/Three-axis/control/3axis_drawing/3axis_drawing.ino) shows how we use point-to-point-comparasion interpolation algorithm to draw rectangle. In this example, we define a struct named Point which save the data of the target point. We define 4 points in the example to drawing rectangle in 25 * 50. 
So, if you want to draw more complex pictures or other things, you can add or change the points to let the robot drawing the picture you want. Different from Two-axis writing robot, this type robot can hold up the pen. So this robot could drawing more complex picture or character.
```
struct Point{
    float x;
    float y;
};
```
&emsp;&emsp; In **[gcode_drawing](https://github.com/unswimmingduck/IA_WritingRobot/blob/main/Three-axis/control/gocde_drawing)**, user could drawing any picture or character they want in this robot after **following belowing steps**:   
&emsp;&emsp; Firstly, user can use **Inkscape** to convert the picture or character into penstroke paths and save those penstroke paths in .gcode format. If you donot want to generate, there is a gcode file that you could use in [control/gocde_drawing/gcode](https://github.com/unswimmingduck/IA_WritingRobot/tree/main/Three-axis/control/gocde_drawing/gcode). This is a gcode file could drwaing "Hello". 

&emsp;&emsp; Secondly, using USB to conenect the Arduino uno board and upload the init file: [control/gocde_drawing/gcode_init/](https://github.com/unswimmingduck/IA_WritingRobot/blob/main/Three-axis/control/gocde_drawing/gcode_init/) to Arduino uno. 

&emsp;&emsp; Thirdly, keeping the connection to the Arduino uno board and open [Processing_code/gctrl.pde](https://github.com/unswimmingduck/IA_WritingRobot/blob/main/Processing_code/gctrl.pde) in **Processing**.
Then in the processing, you could type P to choose the port in your computer that connect to the Arduino board. After that, you could type G to choose the path of your .gcode file that you generate or download in step 1, so after choosing gcode file, the robot will start to draw.
if you want the robot to stop, you can type X to let it stop. 



## Display
The following shows the robot drwaing some pictures.
![](https://github.com/unswimmingduck/IA_WritingRobot/blob/main/Two-axis/display/writing_show.mp4)
