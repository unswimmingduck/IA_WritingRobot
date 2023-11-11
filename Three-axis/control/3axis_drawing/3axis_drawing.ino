#define EN        8       //步进电机使能端，低电平有效
#define X_DIR     5       //X轴 步进电机方向控制
#define Y_DIR     6       //y轴 步进电机方向控制
#define Z_DIR     7       //z轴 步进电机方向控制
#define X_STP     2       //x轴 步进控制
#define Y_STP     3       //y轴 步进控制
#define Z_STP     4       //z轴 步进控制
boolean DIR;              //boolean类型变量 DIR，控制步进电机方向，true为正向，false为反向，根据接线做调整
int stepper_delay = 40;   //定义步进电机脉冲发送的时间间隔

#define LINE_BUFFER_LENGTH 512  

const int SENSOR_X = 14;  //定义X,Y,Z轴复位传感器引脚
const int SENSOR_Y = 18;
const int SENSOR_Z = 16;

const int stepsPerRevolution = 3200;  //定义步进电机每圈转动的步数，细分为16

float LEAD = 8;  //定义丝杠导程，即步进电机转动一圈，丝杠前进8cm

// Current position of plothead
struct point actuatorPos;

float Xmin = -40;   //定义绘图区域范围
float Xmax = 40;
float Ymin = -40;
float Ymax = 40;

float Xpos = 0;
float Ypos = 0;

boolean verbose = false;


void setup()
 
{
 
Serial.begin(9600);                             //开启串口通信，波特率为9600
 
  pinMode(X_DIR, OUTPUT); pinMode(X_STP, OUTPUT);
 
  pinMode(Y_DIR, OUTPUT); pinMode(Y_STP, OUTPUT);
 
  pinMode(EN, OUTPUT);
 
  digitalWrite(EN, LOW);       
 
resetStepper();
 
}
 
 
void loop()
 
{           
 
    while(digitalRead(SENSOR_TOUCH))
 
     delay(10);
 
 
    step(Y_DIR,Y_STP,-40000);
 
    step(X_DIR, X_STP, 24000);

    step(Z_DIR, Z_STP, 1000);

    // drwaing rectangle
    drawLine(50, 0);
    
    drawLine(0, -25);
    
    drawLine(-50, 0);

    drawLine(0, 25);


    resetStepper();
 
}
 
 
//直线插补函数，参数为点坐标值
void drawLine(float x1, float y1)
{
  int dx, dy, n, k, i, f, stepInc;
  
  if (x1 >= Xmax) { 
    x1 = Xmax; 
  }
  if (x1 <= Xmin) { 
    x1 = Xmin; 
  }
  if (y1 >= Ymax) { 
    y1 = Ymax; 
  }
  if (y1 <= Ymin) { 
    y1 = Ymin; 
  }
  
  x1 = (int)(x1/LEAD*stepsPerRevolution);
  y1 = (int)(y1/LEAD*stepsPerRevolution);
  float x0 = Xpos;
  float y0 = Ypos;
  
  Serial.println(Xpos);
  Serial.println(Ypos);
  
  dx = abs(x1-x0);
  dy = abs(y1-y0);
  n = abs(dx+dy);
  if(dx==0||dy==0)
  {
    stepper_delay = 40;
    stepInc = 10;
  }
  else
  {
    stepper_delay = 220;
    stepInc = 20;
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
  
  for(i=0,f=0;i<n;i+=stepInc)
  {
    if(f>=0)
    {
      switch(k)
      {
         case 1:
            step(X_DIR, X_STP, stepInc);
            x0 += (stepInc/stepsPerRevolution)*LEAD;
            f = x1*y0 - y1*x0;
         break;

         case 2:
            step(X_DIR, X_STP, -stepInc);
            x0 -= (stepInc/stepsPerRevolution)*LEAD;
            f = x1*y0 - y1*x0;
         break;

         case 3:
            step(X_DIR, X_STP, -stepInc);
            x0 -= (stepInc/stepsPerRevolution)*LEAD;
            f = x1*y0 - y1*x0;
         break;

         case 4:
            step(X_DIR, X_STP, stepInc);
            x0 += (stepInc/stepsPerRevolution)*LEAD;
            f = x1*y0 - y1*x0;
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
          y0 += (stepInc/stepsPerRevolution)*LEAD;
          f = x1*Ypos - y1*Xpos; 
        break;

        case 2:
          step(Y_DIR, Y_STP, stepInc);
          y0 += (stepInc/stepsPerRevolution)*LEAD;
          f = x1*Ypos - y1*Xpos; 
        break;

        case 3:
          step(Y_DIR, Y_STP, -stepInc);
          y0 -= (stepInc/stepsPerRevolution)*LEAD;
          f = x1*Ypos - y1*Xpos; 
        break;

        case 4:
          step(Y_DIR, Y_STP, -stepInc);
          y0 -= (stepInc/stepsPerRevolution)*LEAD;
          f = x1*Ypos - y1*Xpos; 
        break;

        default:break;
      }
    }
  }
  stepper_delay = 40;
}

/*
//函数：step    功能：控制步进电机方向，步数。
//参数：dirPin对应步进电机的DIR引脚，stepperPin 对应步进电机的step引脚， steps 步进的步数
//无返回值
*/
void step(byte dirPin, byte stepperPin, int steps)
{
  boolean DIR = steps>0 ? 1 : 0;  
  digitalWrite(dirPin,DIR);
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
		step(Y_DIR,Y_STP,10);
        step(Y_DIR,Y_STP,-15);
        while(digitalRead(SENSOR_Z))
                step(Z_DIR,Z_STP,10);
        step(Z_DIR,Z_STP,-15);
        step(X_DIR, X_STP, 28000);
        step(Y_DIR, Y_STP, -16000);
        step(Z_DIR, Z_STP, -30000);
}