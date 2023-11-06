#define EN        8       //步进电机使能端，低电平有效
 
#define X_DIR     5       //X轴 步进电机方向控制
 
#define Y_DIR     6       //y轴 步进电机方向控制
 
#define X_STP     2       //x轴 步进控制
 
#define Y_STP     3       //y轴 步进控制
 
boolean DIR;              //boolean类型变量 DIR，控制步进电机方向，true为正向，false为反向，根据接线做调整
 
int stepper_delay = 40;   //定义步进电机脉冲发送的时间间隔
 
 
const int SENSOR_X = 18;     //定义X方向复位传感器引脚
 
const int SENSOR_Y = 14;     //定义y方向复位传感器引脚
 
const int SENSOR_TOUCH = 16; //定义电机运动的触发传感器引脚
 
 
 
const int stepsPerRevolution = 3200;   //定义步进电机每圈转动的步数，细分为16
 
 
 
float LEAD = 8.0;   //定义丝杠导程，即步进电机转动一圈，丝杠前进8mm
 
 
float DIAGONAL_A = 20;   //定义多边形两条对角线长度
 
float DIAGONAL_B = 30;
 
 
float Xmin = 0;   
 
float Xmax = sqrt((DIAGONAL_A/2)*(DIAGONAL_A/2)+(DIAGONAL_B/2)*(DIAGONAL_B/2));   //多边形X方向边长
 
float Ymin = 0;
 
float Ymax = (2*DIAGONAL_A/2*DIAGONAL_B/2)/Xmax;     //多边形Y方向的高
  
float x1 = 100;
float y1 = 0;

float x2 = 195;
float y2 = 69;

float x3 = 195;
float y3 = 181;

float x4 = 41;
float y4 = 181;

float x5 = 5;
float y5 = 69;

struct Point{
    float x;
    float y;
};


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
 
    struct Point p1, p2, p3, p4, p5;
    
    p1.x = 100;
    p1.y = 0;

    p2.x = 195;
    p2.y = 69;

    p3.x = 195;
    p3.y = 181;

    p4.x = 41;
    p4.y = 181;

    p5.x = 5;
    p5.y = 69;

    runIn(p1, p2);
 
   
 
    runIn(p2, p3);
 
   
 
    runIn(p3, p4);
 
   
 
    runIn(p4, p5);

    runIn(p5, p1);
 
   
 
    resetStepper();
 
}
 
 
//四象限直线插补函数，参数为两个点的坐标值
 
void runIn(struct Point P1, struct Point P2)
 
{
 
  /*
   x_step：X轴两坐标间步数值
   y_step：Y轴两坐标间步数值
   x_i: 当前位置的X坐标值
   y_i: 当前位置的y坐标值
   total_step ：两坐标X轴和Y轴总步数
   k ：象限值
   f ：偏差计算值
   stepInc：步进电机转动步数
   */
 
  int x_step, y_step, x_i=x1, y_i=y1, total_step, k, i, f, stepInc;
 
  x_step = abs((P2.x - P1.x)/LEAD*stepsPerRevolution);  
 
  y_step = abs((P2.y - P1.y)/LEAD*stepsPerRevolution);
 
  total_step = abs(x_step + y_step);                           
 
  if(x_step ==0||y_step==0)
  {
    stepper_delay = 40;
    stepInc = 10;
  }
  else
  {
    stepper_delay = 200;
    stepInc = 100;
  }
 
  
  if(P2.x >= P1.x)
  {
    if( P2.y >= P1.y )
      k = 1;
    else
      k = 4;
  }
  else
  {
    if( P2.y >= P1.y )
      k = 2;
    else
      k = 3;
  }
 
 
  for(i=0,f=0;i<total_step;i+=stepInc)
  {
 
    if(f>=0)
    {
      switch(k)
      {
         case 1: 
            step(X_DIR, X_STP, stepInc);
            x_i += (stepInc/stepsPerRevolution)*LEAD;
            f = x2*y_i - y2*x_i;
         break;
 
         case 2: 
            step(X_DIR, X_STP, -stepInc);
            x_i -= (stepInc/stepsPerRevolution)*LEAD;
            f = x2*y_i - y2*x_i;
         break;
 
         case 3:
            step(X_DIR, X_STP, -stepInc);
            x_i -= (stepInc/stepsPerRevolution)*LEAD;
            f = x2*y_i - y2*x_i;
         break;
 
         case 4:
            step(X_DIR, X_STP, stepInc);
            x_i += (stepInc/stepsPerRevolution)*LEAD;
            f = x2*y_i - y2*x_i; 
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
          y_i += (stepInc/stepsPerRevolution)*LEAD;
          f = x2*y_i - y2*x_i; 
        break;
 
        case 2:
          step(Y_DIR, Y_STP, stepInc); 
          y_i += (stepInc/stepsPerRevolution)*LEAD;
          f = x2*y_i - y2*x_i; 
        break;
 
        case 3: 
          step(Y_DIR, Y_STP, -stepInc);
          y_i -= (stepInc/stepsPerRevolution)*LEAD;
          f = x2*y_i - y2*x_i; 
        break;
 
        case 4:
          step(Y_DIR, Y_STP, -stepInc);
          y_i -= (stepInc/stepsPerRevolution)*LEAD;
          f = x2*y_i - y2*x_i;  
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