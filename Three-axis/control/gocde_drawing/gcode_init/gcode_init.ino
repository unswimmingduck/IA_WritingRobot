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
  Serial.begin(9600);      //开启串口通信，波特率为9600
  pinMode(X_DIR, OUTPUT); pinMode(X_STP, OUTPUT);
  pinMode(Y_DIR, OUTPUT); pinMode(Y_STP, OUTPUT);
  pinMode(Z_DIR, OUTPUT); pinMode(Z_STP, OUTPUT);
  pinMode(EN, OUTPUT);
  digitalWrite(EN, LOW);	  	
  resetStepper();
  delay(1000);
}

void loop()
{            
  delay(200);
  char line[ LINE_BUFFER_LENGTH ];
  char c;
  int lineIndex;
  bool lineIsComment, lineSemiColon;

  lineIndex = 0;
  lineSemiColon = false;
  lineIsComment = false;

  while (1) {

    // 接受来自Grbl的串口数据
    while ( Serial.available()>0 ) {
      c = Serial.read();
      if (( c == '\n') || (c == '\r') ) {             // End of line reached
        if ( lineIndex > 0 ) {                        // Line is complete. Then execute!
          line[ lineIndex ] = '\0';                   // Terminate string
          if (verbose) { 
            Serial.print( "Received : "); 
            Serial.println( line ); 
          }
          processIncomingLine( line, lineIndex );
          lineIndex = 0;
        } 
        else { 
          // Empty or comment line. Skip block.
        }
        lineIsComment = false;
        lineSemiColon = false;
        Serial.println("ok");    
      } 
      else {
        if ( (lineIsComment) || (lineSemiColon) ) {   // Throw away all comment characters
          if ( c == ')' )  lineIsComment = false;     // End of comment. Resume line.
        } 
        else {
          if ( c <= ' ' ) {                           // Throw away whitepace and control characters
          } 
          else if ( c == '/' ) {                    // Block delete not supported. Ignore character.
          } 
          else if ( c == '(' ) {                    // Enable comments flag and ignore all characters until ')' or EOL.
            lineIsComment = true;
          } 
          else if ( c == ';' ) {
            lineSemiColon = true;
          } 
          else if ( lineIndex >= LINE_BUFFER_LENGTH-1 ) {
            Serial.println( "ERROR - lineBuffer overflow" );
            lineIsComment = false;
            lineSemiColon = false;
          } 
          else if ( c >= 'a' && c <= 'z' ) {        // Upcase lowercase
            line[ lineIndex++ ] = c-'a'+'A';
          } 
          else {
            line[ lineIndex++ ] = c;
          }
        }
      }
    }
  }
}

void processIncomingLine( char* line, int charNB ) {
  int currentIndex = 0;
  char buffer[ 64 ];                                 // Hope that 64 is enough for 1 parameter
  struct point newPos;

  newPos.x = 0.0;
  newPos.y = 0.0;

  //  Needs to interpret 
  //  G1 for moving
  //  G4 P300 (wait 150ms)
  //  G1 X60 Y30
  //  G1 X30 Y50
  //  M300 S30 (pen down)
  //  M300 S50 (pen up)
  //  Discard anything with a (
  //  Discard any other command!

  while( currentIndex < charNB ) {
    switch ( line[ currentIndex++ ] ) {              // Select command, if any
    case 'U':
      step(Z_DIR, Z_STP, 2000); 
      break;
    case 'D':
      step(Z_DIR, Z_STP, -2000); 
      break;
    case 'G':
      buffer[0] = line[ currentIndex++ ];          // /!\ Dirty - Only works with 2 digit commands
      //      buffer[1] = line[ currentIndex++ ];
      //      buffer[2] = '\0';
      buffer[1] = '\0';

      switch ( atoi( buffer ) ){                   // Select G command
      case 0:                                   // G00 & G01 - Movement or fast movement. Same here
      case 1:
        // /!\ Dirty - Suppose that X is before Y
        char* indexX = strchr( line+currentIndex, 'X' );  // Get X/Y position in the string (if any)
        char* indexY = strchr( line+currentIndex, 'Y' );
        if ( indexY <= 0 ) {
          newPos.x = atof( indexX + 1); 
          newPos.y = actuatorPos.y;
        } 
        else if ( indexX <= 0 ) {
          newPos.y = atof( indexY + 1);
          newPos.x = actuatorPos.x;
        } 
        else {
          newPos.y = atof( indexY + 1);
          indexY = '\0';
          newPos.x = atof( indexX + 1);
        }
        drawLine(newPos.x, newPos.y );
        //        Serial.println("ok");
        actuatorPos.x = newPos.x;
        actuatorPos.y = newPos.y;
        break;
      }
      break;
    case 'M':
      buffer[0] = line[ currentIndex++ ];        // /!\ Dirty - Only works with 3 digit commands
      buffer[1] = line[ currentIndex++ ];
      buffer[2] = line[ currentIndex++ ];
      buffer[3] = '\0';
      switch ( atoi( buffer ) ){
      case 300:
        {
          char* indexS = strchr( line+currentIndex, 'S' );
          float Spos = atof( indexS + 1);
          //          Serial.println("ok");
          if (Spos == 30) { 
            step(Z_DIR, Z_STP, -2000); 
          }
          if (Spos == 50) { 
            step(Z_DIR, Z_STP, 2000); 
          }
          break;
        }
      case 114:                                // M114 - Repport position
        Serial.print( "Absolute position : X = " );
        Serial.print( actuatorPos.x );
        Serial.print( "  -  Y = " );
        Serial.println( actuatorPos.y );
        break;
      default:
        Serial.print( "Command not recognized : M");
        Serial.println( buffer );
      }
    }
  }
  
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
