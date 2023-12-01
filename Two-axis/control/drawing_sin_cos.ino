#define EN        8       // Stepper motor enable terminal, active low
 
#define X_DIR     5       //X-axis Stepper motor direction control
 
#define Y_DIR     6       //y-axis Stepper motor direction control
 
#define X_STP     2       //x-axis Stepping control
 
#define Y_STP     3       //y-axis Stepping control
 
boolean DIR;              //boolean类型变量 DIR，控制步进电机方向，true为正向，false为反向，根据接线做调整
 
int stepper_delay = 40;   // Define the time interval at which stepper motor pulses are sent.
 
 
const int SENSOR_X = 18;     //Define X-direction reset sensor pin
 
const int SENSOR_Y = 14;     //Define y-direction reset sensor pin
 
const int SENSOR_TOUCH = 16; // Define trigger sensor pins for motor movement
 
 
 
const int stepsPerRevolution = 3200;   //Define the number of steps per revolution of the stepper motor, subdivided into 16
 
 
 
float LEAD = 8.0;   //Define the screw lead, i.e. one revolution of the stepper motor advances the screw by 8mm.
 
 
float DIAGONAL_A = 20;   // Define the lengths of the two diagonals of the polygon.
 
float DIAGONAL_B = 30;
 
 
float Xmin = 0;   
 
float Xmax = sqrt((DIAGONAL_A/2)*(DIAGONAL_A/2)+(DIAGONAL_B/2)*(DIAGONAL_B/2));   //多边形X方向边长
 
float Ymin = 0;
 
float Ymax = (2*DIAGONAL_A/2*DIAGONAL_B/2)/Xmax;     
  
float A = 2.0;//Define sin function amplitude
float W = 1.0;//Definition sin function angular frequency
float P = 0.0;//Define the initial phase of the sin function
float Xmin = 0;
float Ymin = 0;
float Xpos = Xmin;
float Ypos = Ymin;

void setup()
{
    Serial.begin(9600);//Enable serial communication at 9600 baud rate.
    pinMode(X_DIR, OUTPUT); 
    pinMode(X_STP, OUTPUT);
    pinMode(Y_DIR, OUTPUT); 
    pinMode(Y_STP, OUTPUT);
    pinMode(EN, OUTPUT);
    digitalWrite(EN, LOW);
    resetStepper();
}


void loop()
 
{           
 
    while(digitalRead(SENSOR_TOUCH))
 
     delay(10);
 
     Xpos = 0;
     Ypos = 0;

     step(Y_DIR,Y_STP,-40000);

     for(float x=0;x<=2*PI/W;x+=0.1)
     {
        float y = A*sin(W*x+P);
        drawLine(x,y);
      }

      18step(X_DIR, X_STP, 8000);
      resetStepper()
}


/*
//Function: drawLine Function: According to the drawing method of interpolation algorithm, control the motor to realize the drawing function.
//Parameters: x1: x-axis coordinate of the target point to be reached, y1: y-axis coordinate of the target point to be reached 
//No return value
*/
void drawLine(float x1, float y1)
{
  int dx, dy, total_step, k, i, f, stepInc;
  x1 = (int)(x1/LEAD*stepsPerRevolution);
  y1 = (int)(y1/LEAD*stepsPerRevolution);
  float x0 = Xpos;
  float y0 = Ypos;
  Serial.println(Xpos);
  Serial.println(Ypos);
  dx = abs(x1-x0);
  dy = abs(y1-y0);
  total_step = abs(dx+dy);
  if(dx==0||dy==0)
  {
    stepper_delay = 40;
    stepInc = 10;
  }
  else
  {
      stepper_delay = 200;
      stepInc = 100;
  }

  if(x1 >= x0)
  {
    if(y1 >= y0)
      k = 1;
    else
      k = 2;
  }
  else
  {
    if(y1 >= y0)
      k = 3;
    else
      k = 4;
  }


  for(i=0,f=0;i<total_step;i+=stepInc)
  {
 
    if(f>=0)
    {
      switch(k)
      {
         case 1: 
            step(X_DIR, X_STP, stepInc);
            Xpos += (stepInc/stepsPerRevolution)*LEAD;
            f = x2*Ypos - y2*Xpos;
         break;
 
         case 2: 
            step(X_DIR, X_STP, -stepInc);
            Xpos -= (stepInc/stepsPerRevolution)*LEAD;
            f = x2*Ypos - y2*Xpos;
         break;
 
         case 3:
            step(X_DIR, X_STP, -stepInc);
            Xpos -= (stepInc/stepsPerRevolution)*LEAD;
            f = x2*Ypos - y2*Xpos;
         break;
 
         case 4:
            step(X_DIR, X_STP, stepInc);
            Xpos += (stepInc/stepsPerRevolution)*LEAD;
            f = x2*Ypos - y2*Xpos; 
         break;
 
         default:break;
      }
    }
 
    else
    { 
      switch(k)
      {
 
        case 1:
          step(Y_DIR, Y_STP, stepInc);
          Ypos += (stepInc/stepsPerRevolution)*LEAD;
          f = x2*Ypos - y2*Xpos; 
        break;
 
        case 2:
          step(Y_DIR, Y_STP, stepInc); 
          Ypos += (stepInc/stepsPerRevolution)*LEAD;
          f = x2*Ypos - y2*Xpos; 
        break;
 
        case 3: 
          step(Y_DIR, Y_STP, -stepInc);
          Ypos -= (stepInc/stepsPerRevolution)*LEAD;
          f = x2*Ypos - y2*Xpos; 
        break;
 
        case 4:
          step(Y_DIR, Y_STP, -stepInc);
          Ypos -= (stepInc/stepsPerRevolution)*LEAD;
          f = x2*Ypos - y2*Xpos;  
        break;
 
        default:break;
      }
    }
  
  }
}

/*
//函数：step    功能：控制步进电机方向，步数。
//参数：dirPin对应步进电机的DIR引脚，stepperPin 对应步进电机的step引脚， steps 步进的步数
//无返回值
*/
void step(byte dirPin, byte stepperPin, int steps)
 
{
 
  int Dir_flag = steps>0 ? 1 : 0;  
 
  digitalWrite(dirPin,Dir_flag);
 
  for(int i=0;i<abs(steps); i++)
 
  {
 
    digitalWrite(stepperPin, HIGH);
 
    delayMicroseconds(stepper_delay);
 
    digitalWrite(stepperPin, LOW);
 
    delayMicroseconds(stepper_delay);
 
  }
 
}
 
 
//步进电机复位函数
 
void resetStepper()
 
{
 
        stepper_delay = 40;
 
while(digitalRead(SENSOR_X))
 
step(X_DIR,X_STP,-10);
 
        step(X_DIR,X_STP,15);
 
while(digitalRead(SENSOR_Y))
 
step(Y_DIR,Y_STP,-10);
 
        step(Y_DIR,Y_STP,15);
 
}