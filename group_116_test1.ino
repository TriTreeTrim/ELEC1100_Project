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

const int pinL_PWM = 9;          //pin D9: left motor speed
const int pinL_DIR = 10;         //pin D10: left motor direction

const int pinR_PWM = 11;         //pin D11: right motor speed
const int pinR_DIR = 12;         //pin D12: right motor direction

//define variables to be used in script

int bumperSensor = 1;  // not sensing white
int leftSensor = 1;    // not sensing white
int rightSensor = 1;   // not sensing white

int countBumper = 0;   // bumper sensor not triggered yet
int countTJunc = 0;  // count no. of T-junctions encountered

int startTime = 350;   // Time for car to move a bit after start line
int turn90Time = 500; // Time for car to turn 90 degrees, forward dir pwm: 200, backward dir pwm: 100
                      // value TO BE TESTED
int go30cmTime = 2000;  // TASK15: Time for car to go backwards >= 30cm
                        // value TO BE TESTED


// the setup function runs once when you press reset or power the board
void setup ()
{
  // define pins as input and output
  pinMode(pinB_Sensor, INPUT);
  pinMode(pinL_Sensor, INPUT);
  pinMode(pinR_Sensor, INPUT);
  
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
  analogWrite(pinL_PWM, 100);
  analogWrite(pinR_PWM, 200);
  digitalWrite(pinL_DIR, 0);
  digitalWrite(pinR_DIR, 1); 
}

void turnRight() 
{
  analogWrite(pinL_PWM, 200);
  analogWrite(pinR_PWM, 100);
  digitalWrite(pinL_DIR, 1);
  digitalWrite(pinR_DIR, 0);  
}

void goForward() 
{
  analogWrite(pinL_PWM, 200);
  analogWrite(pinR_PWM, 200);
  digitalWrite(pinL_DIR, 1);
  digitalWrite(pinR_DIR, 1);  
}

void goBackward()
{
  analogWrite(pinL_PWM, 200);
  analogWrite(pinR_PWM, 200);
  digitalWrite(pinL_DIR, 0);
  digitalWrite(pinR_DIR, 0);
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
    countBumper += 1;
    delay(startTime);     //to let the car leave the start position with no miscount
  }
  
  // TASK4-13: car is tracking on the white line
  else if ( bumperSensor && countBumper == 1) 
  { 
    // when encounter T-junction
    if ( !leftSensor && !rightSensor ) 
    {
      switch (countTJunc)
      {
        case 0:  // TASK5
          turnLeft();
          delay(turn90Time);  // turns left 90 degrees
          break;
        case 1:  // TASK6
          goForward();   
          delay(startTime);
          break;
        case 2:  // TASK7 - enter loop entrance
          turnLeft();
          delay(turn90Time);
          break;
        case 3:  // TASK7 - leave loop entrance
          goForward();
          delay(startTime);
          break;
        case 4:  // TASK8
          turnLeft();
          delay(turn90Time);
          break;
        case 5:  // TASK9
          goForward();
          delay(startTime);
          break;
        case 6:  // TASK11
          turnLeft();
          delay(turn90Time);
          break;
        case 7: // TASK12 - 360 degrees turn, then go forward a bit
          turnLeft();
          delay(turn90Time * 4);
          goForward();
          delay(startTime);
          break;
        default:
          goForward();
      }

      countTJunc += 1;
    }
    
    if ( !leftSensor && rightSensor ) 
    {
      turnLeft();
    }
    
    if ( leftSensor && !rightSensor ) 
    {
      turnRight();
    }
    
    if ( leftSensor && rightSensor ) 
    {
      goForward();
    }

  }

  // TASK14 - encounter white wall
  else if ( !bumperSensor && countBumper == 1 )
  {
    // TASK15 - go backward for >= 30cm
    goBackward();
    delay(go30cmTime);
    countBumper += 1;
  }

  // TASK16 - stop after finishing TASK1-15
  else if ( bumperSensor && countBumper == 2 )
  {
    stop(); 
  }

}
