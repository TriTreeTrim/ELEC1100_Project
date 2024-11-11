/*
  ELEC1100 Your Lab#06 & Project Template

  To program the car tracking the white line on a dark mat

  Group No. 116:  
  Group Member 1 Wong Hoi Ying & 21179294 :
  Group Member 2 Mak Ho Yin & :
  
*/

// assign meaningful names to those pins that will be used

const int pinL_Sensor = A5;      //pin A5: left sensor 
const int pinB_Sensor = A4;      //pin A4: bumper sensor
const int pinR_Sensor = A3;      //pin A3: right sensor 
const int pinM_Sensor = A2;      //pin A2: middle sensor

const int pinL_PWM = 9;          //pin D9: left motor speed
const int pinL_DIR = 10;         //pin D10: left motor direction

const int pinR_PWM = 11;         //pin D11: right motor speed
const int pinR_DIR = 12;         //pin D12: right motor direction

//define variables to be used in script

int bumperSensor = 1;  // not sensing white
int leftSensor = 1;    // not sensing white
int rightSensor = 1;   // not sensing white
int middleSensor = 0;  // sensing white

int countBumper = 0;   // bumper sensor not triggered yet
int countTJunc = 0;  // count no. of T-junctions encountered

int startTime = 300;   // Time for car to move a bit after start line
int turn360Time = 1500; // Time for car to turn 360 degrees
                      // value TO BE TESTED
int go30cmTime = 300;  // TASK15: Time for car to go backwards >= 30cm
                        // value TO BE TESTED

int inLoop = 0; // if enter loop, change pwn values
int turn360 = 0; // faster pwn when 360 turn
int afterCuv = 0; // change turn pwn values
int after360 = 0; // stop before turning
int backwards = 0;

// the setup function runs once when you press reset or power the board
void setup ()
{
  // define pins as input and output
  pinMode(pinB_Sensor, INPUT);
  pinMode(pinL_Sensor, INPUT);
  pinMode(pinR_Sensor, INPUT);
  pinMode(pinM_Sensor, INPUT);
  
  pinMode(pinL_DIR, OUTPUT);
  pinMode(pinR_DIR, OUTPUT);
  
  pinMode(pinL_PWM, OUTPUT);
  pinMode(pinR_PWM, OUTPUT);
  
  // initialize output pins
  digitalWrite(pinL_DIR, HIGH);   //forward direction    
  digitalWrite(pinR_DIR, HIGH);   //forward direction
  analogWrite(pinL_PWM, 0);       //stop at the start position 
  analogWrite(pinR_PWM, 0);       //stop at the start position 
}

// define Car Turning & Moving Functions
void turnLeft() 
{

  if (after360)
  {
    stop();
    delay(20);
  }
  if (turn360)
  {
    analogWrite(pinL_PWM, 400);
    analogWrite(pinR_PWM, 400);
    digitalWrite(pinL_DIR, 0);
    digitalWrite(pinR_DIR, 1);
  }
  else if (afterCuv)
  {
     analogWrite(pinL_PWM, 0);
     analogWrite(pinR_PWM, 250);
     digitalWrite(pinL_DIR, 0);
     digitalWrite(pinR_DIR, 1);
  }
  else if (inLoop)
  {
    analogWrite(pinL_PWM, 130);
    analogWrite(pinR_PWM, 100);
    digitalWrite(pinL_DIR, 0);
    digitalWrite(pinR_DIR, 1);
    //delay(20);
  }
  else
  {
     //stop();
     //delay(5);
     analogWrite(pinL_PWM, 105);
     analogWrite(pinR_PWM, 140);
     digitalWrite(pinL_DIR, 0);
     digitalWrite(pinR_DIR, 1);
     //delay(10);
  }
 
   
}

void turnRight() 
{
  if (after360)
  {
    stop();
    delay(20);
  }
  if (turn360)
  {
    //stop();
    //delay(5);
    analogWrite(pinL_PWM, 400);
    analogWrite(pinR_PWM, 400);
    digitalWrite(pinL_DIR, 1);
    digitalWrite(pinR_DIR, 0);
    //delay(10);
  }
  else if (inLoop)
  {
    analogWrite(pinL_PWM, 130);
    analogWrite(pinR_PWM, 100);
    digitalWrite(pinL_DIR, 1);
    digitalWrite(pinR_DIR, 0);  
    //delay(20);
  }
  else
  {
    //stop();
    //delay(5);
    analogWrite(pinL_PWM, 140);
    analogWrite(pinR_PWM, 105);
    digitalWrite(pinL_DIR, 1);
    digitalWrite(pinR_DIR, 0);  
    //delay(10);
  }
  
}

void goForward() 
{
  if (inLoop)
  {
    analogWrite(pinL_PWM, 100);
    analogWrite(pinR_PWM, 100);
    digitalWrite(pinL_DIR, 1);
    digitalWrite(pinR_DIR, 1);  
  }
  else if (after360)
  {
    analogWrite(pinL_PWM, 105);
    analogWrite(pinR_PWM, 105);
    digitalWrite(pinL_DIR, 1);
    digitalWrite(pinR_DIR, 1);  
  }
  else
  {
    analogWrite(pinL_PWM, 115);
    analogWrite(pinR_PWM, 115);
    digitalWrite(pinL_DIR, 1);
    digitalWrite(pinR_DIR, 1);  
  }
}

void goBackward()
{

  if (backwards)
  {
    analogWrite(pinL_PWM, 400);
    analogWrite(pinR_PWM, 400);
    digitalWrite(pinL_DIR, 0);
    digitalWrite(pinR_DIR, 0);
  }
  else
  {
    analogWrite(pinL_PWM, 250);
    analogWrite(pinR_PWM, 250);
    digitalWrite(pinL_DIR, 0);
    digitalWrite(pinR_DIR, 0);
  }
  
}

void stop()
{
  analogWrite(pinL_PWM, 0);
  analogWrite(pinR_PWM, 0);
}


// the loop function runs over and over again forever
void loop() 
{

  // Arduino is reading the sensor measurements
  bumperSensor = digitalRead(pinB_Sensor);
  leftSensor = digitalRead(pinL_Sensor);
  rightSensor = digitalRead(pinR_Sensor);
  
  // TASK1-2: car stops at the start position when bumper sensor no trigger
  if ( bumperSensor && countBumper == 0 ) 
  {
    stop();
  }

  // TASK3: bumper sensor is triggered at the start position for the 1st time
  else if ( !bumperSensor && countBumper == 0) 
  {
    goForward();
    countBumper++;
    delay(startTime);     //to let the car leave the start position with no miscount
  }
  
  // TASK4-13: car is tracking on the white line
  else if ( bumperSensor && countBumper == 1) 
  { 
    // when encounter T-junction
    if ( ( !leftSensor && !middleSensor && !rightSensor ) || (!leftSensor && middleSensor && !rightSensor) )  // 000 or 010
    {
      stop();
      delay(100);
      switch (countTJunc)
      {
        case 0:  // TASK5
          turnLeft();
          delay(400);
          break;
        case 1:  // TASK6
          stop();
          delay(30);
          turnLeft();
          delay(250);
          goBackward();
          delay(40);
          break;
        case 2:  // TASK7 - enter loop entrance
          stop();
          delay(30);
          turnLeft();
          delay(350);
          inLoop = 1;
          break;
        case 3:  // TASK7 - leave loop entrance
          stop();
          delay(30);
          turnLeft();
          delay(250);
          inLoop = 0;
          break;
        case 4:  // TASK8
          turnLeft();
          delay(400);
          break;
        case 5:  // TASK9
          turnRight();
          delay(250);
          break;
        case 6:  // TASK11
          afterCuv = 1;
          turnLeft();
          delay(200);
          afterCuv = 0;
          break;
        case 7: // TASK12 - 360 degrees turn, then go forward a bit
          turn360 = 1;
          turnLeft();
          delay(turn360Time);
          goForward();
          delay(350);
          turn360 = 0;
          after360 = 1;
          break;
        default:
          turnRight();
          delay(450);
      }

 
      countTJunc++;
    }
    
    else if ( !leftSensor && !middleSensor && rightSensor )  // 001 
    {
      turnLeft();
    }
    else if ( !leftSensor && middleSensor && rightSensor )  // 011
    {
      turnLeft();
    }
    else if ( leftSensor && !middleSensor && !rightSensor ) // 100
    {
      turnRight();
    }
    else if ( leftSensor && !middleSensor && rightSensor ) // 101
    {
      goForward();
    }
    else if ( leftSensor && middleSensor && !rightSensor ) // 110
    {
      turnRight();
    }
    else if ( leftSensor && middleSensor && rightSensor )  // 111
    {
      goForward();
    }

  }

  // TASK14 - encounter white wall
  else if ( !bumperSensor && countBumper == 1 )
  {
    // TASK15 - go backward for >= 30cm
    backwards = 1;
    goBackward();
    delay(go30cmTime);
    countBumper++;
  }

  // TASK16 - stop after finishing TASK1-15
  else if ( bumperSensor && countBumper == 2 )
  {
    stop(); 
  }

}
