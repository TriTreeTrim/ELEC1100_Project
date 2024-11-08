/*
  ELEC1100 Your Lab#06 & Project Template

  To program the car tracking the white line on a dark mat

  Group No. (number of your project box):  
  Group Member 1 (name & SID):
  Group Member 2 (name & SID):
  
*/

// assign meaningful names to those pins that will be used

// Pin definitions
const int pinL_Sensor = A5;      // Pin A5: left sensor 
const int pinB_Sensor = A4;      // Pin A4: bumper sensor
const int pinR_Sensor = A3;      // Pin A3: right sensor 

const int pinL_PWM = 9;          // Pin D9: left motor speed
const int pinL_DIR = 10;         // Pin D10: left motor direction

const int pinR_PWM = 11;         // Pin D11: right motor speed
const int pinR_DIR = 12; 
// Pin D12: right motor direction
int st=0;
// State definitions
enum State {
  STOPPED,
  START,
  FOL0_PATH,
  TURN_LEFT,
  IN_LOOP,
  EXIT_LOOP,
  TURN_Lefttocruve,
  TURN_360,
  BACKWARD
};
State currentState=0;


unsigned long actionStartTime = 0;
int countBumper = 0;   // For bumper sensor count
int loopCounter = 0;   // For loop navigation
int bothwhite=0;

void setup() {
  pinMode(pinB_Sensor, INPUT);
  pinMode(pinL_Sensor, INPUT);
  pinMode(pinR_Sensor, INPUT);
  
  pinMode(pinL_DIR, OUTPUT);
  pinMode(pinR_DIR, OUTPUT);
  
  pinMode(pinL_PWM, OUTPUT);
  pinMode(pinR_PWM, OUTPUT);
  
  // Initialize output pins
  digitalWrite(pinL_DIR, 1);   // Forward direction    
  digitalWrite(pinR_DIR, 1);   // Forward direction
  analogWrite(pinL_PWM, 0);       // Stop at the start position 
  analogWrite(pinR_PWM, 0);       // Stop at the start position 
}
int firsttrigger=0;
void stop(){
  analogWrite(pinL_PWM, 0);
      analogWrite(pinR_PWM, 0);
      delay(100);
}
void loop() {
  // Read sensor values
  int bumperSensor = digitalRead(pinB_Sensor);
  int leftSensor = digitalRead(pinL_Sensor);
  int rightSensor = digitalRead(pinR_Sensor);
  if ( bumperSensor && countBumper == 0 ) {
    analogWrite(pinL_PWM, 0);
    analogWrite(pinR_PWM, 0);
  }
  else if ( !bumperSensor && countBumper == 0){
        analogWrite(pinL_PWM, 200);
        analogWrite(pinR_PWM, 200);
        digitalWrite(pinL_DIR, 1);
        digitalWrite(pinR_DIR, 1);
        delay(200);
        currentState=FOL0_PATH;
        countBumper++;
        
  }
  else if ( bumperSensor && countBumper == 1) {
        
       
  switch (currentState) {
     

    case FOL0_PATH:
      if (!leftSensor && !rightSensor) {
        switch (bothwhite) {
          case 0:
             currentState = TURN_LEFT;
             break;
          case 1:
          stop();
            currentState = IN_LOOP;
            break;
          
          case 3:
            currentState = TURN_Lefttocruve;
            break;
          case 4:
            currentState = TURN_360;
            break;
          case 5:
            currentState = BACKWARD;
            break;
          default:
            break;
        }
      } else if (leftSensor && rightSensor) {
        // Continue forward
        
        analogWrite(pinL_PWM, 150);
        analogWrite(pinR_PWM, 150);
        digitalWrite(pinL_DIR, 1);
        digitalWrite(pinR_DIR, 1);
      } else if (leftSensor && !rightSensor) {
        // Turn right
        analogWrite(pinL_PWM, 100);
        analogWrite(pinR_PWM, 100);
        digitalWrite(pinL_DIR, 1);
        digitalWrite(pinR_DIR, 0);
      } else if (!leftSensor && rightSensor) {
        // Turn left
        analogWrite(pinL_PWM, 150);
        analogWrite(pinR_PWM, 150);
        digitalWrite(pinL_DIR, 0);
        digitalWrite(pinR_DIR, 1);
      }
      break;

    case TURN_LEFT:
      // Turn left
       delay(1500); 
      analogWrite(pinL_PWM, 150);
      analogWrite(pinR_PWM, 150);
      digitalWrite(pinL_DIR, 0);
      digitalWrite(pinR_DIR, 1);
      delay(400); // Adjust for timing of the turn
      bothwhite++;
      currentState = FOL0_PATH;
      break;

    case IN_LOOP:
      analogWrite(pinL_PWM, 150);
      analogWrite(pinR_PWM, 150);
      digitalWrite(pinL_DIR, 0);
      digitalWrite(pinR_DIR, 1);
      delay(50);
      if (leftSensor && rightSensor) {
        // Continue forward
        analogWrite(pinL_PWM, 150);
        analogWrite(pinR_PWM, 150);
        digitalWrite(pinL_DIR, 1);
        digitalWrite(pinR_DIR, 1);
      } else if (leftSensor && !rightSensor) {
        // Turn right
        analogWrite(pinL_PWM, 100);
        analogWrite(pinR_PWM, 100);
        digitalWrite(pinL_DIR, 1);
        digitalWrite(pinR_DIR, 0);
      } else if (!leftSensor && rightSensor) {
        // Turn left
        analogWrite(pinL_PWM, 150);
        analogWrite(pinR_PWM, 150);
        digitalWrite(pinL_DIR, 0);
        digitalWrite(pinR_DIR, 1);
      }
      else if (!leftSensor && !rightSensor){
        stop();
        analogWrite(pinL_PWM, 150);
        analogWrite(pinR_PWM, 150);
        digitalWrite(pinL_DIR, 0);
        digitalWrite(pinR_DIR, 1);
        delay(400);
            if (leftSensor && rightSensor) {
         // Continue forward
        analogWrite(pinL_PWM, 150);
        analogWrite(pinR_PWM, 150);
        digitalWrite(pinL_DIR, 1);
        digitalWrite(pinR_DIR, 1);
        } else if (leftSensor && !rightSensor) {
        // Turn right
        analogWrite(pinL_PWM, 100);
        analogWrite(pinR_PWM, 100);
        digitalWrite(pinL_DIR, 1);
        digitalWrite(pinR_DIR, 0);
        } else if (!leftSensor && rightSensor) {
        // Turn left
        analogWrite(pinL_PWM, 150);
        analogWrite(pinR_PWM, 150);
        digitalWrite(pinL_DIR, 0);
        digitalWrite(pinR_DIR, 1);
        }
        else if (!leftSensor && !rightSensor ){
              
              currentState= EXIT_LOOP;
        }

      }

      

    case EXIT_LOOP:
      analogWrite(pinL_PWM, 150);
      analogWrite(pinR_PWM, 150);
      digitalWrite(pinL_DIR, 0);
      digitalWrite(pinR_DIR, 1);
      bothwhite++;
      delay(100); // Adjust for exit timing
      stop();
      if (leftSensor && rightSensor) {
        // Continue forward
        analogWrite(pinL_PWM, 150);
        analogWrite(pinR_PWM, 150);
        digitalWrite(pinL_DIR, 1);
        digitalWrite(pinR_DIR, 1);
      } else if (leftSensor && !rightSensor) {
        // Turn right
        analogWrite(pinL_PWM, 100);
        analogWrite(pinR_PWM, 100);
        digitalWrite(pinL_DIR, 1);
        digitalWrite(pinR_DIR, 0);
      } else if (!leftSensor && rightSensor) {
        // Turn left
        analogWrite(pinL_PWM, 150);
        analogWrite(pinR_PWM, 150);
        digitalWrite(pinL_DIR, 0);
        digitalWrite(pinR_DIR, 1);
      }
      else if (!leftSensor && !rightSensor){
      analogWrite(pinL_PWM, 150);
      analogWrite(pinR_PWM, 150);
      digitalWrite(pinL_DIR, 0);
      digitalWrite(pinR_DIR, 1);
      delay(400); // Adjust for timing of the turn
      currentState = FOL0_PATH;
      bothwhite=3;
      break;
      }
      
      break;

    case TURN_Lefttocruve:
      stop();
      analogWrite(pinL_PWM, 150);
      analogWrite(pinR_PWM, 150);
      digitalWrite(pinL_DIR, 1);
      digitalWrite(pinR_DIR, 0);
      delay(100);
      bothwhite++;
      currentState=FOL0_PATH;

    case TURN_360:
      // 360-degree turn
      analogWrite(pinL_PWM, 150);
      analogWrite(pinR_PWM, 150);
      digitalWrite(pinL_DIR, 0);
      digitalWrite(pinR_DIR, 1);

      delay(1500); // Adjust for full spin
      currentState = FOL0_PATH;
      bothwhite++;
      break;
  /*
    case BACKWARD:
      // Move backward
      analogWrite(pinL_PWM, 150);
      analogWrite(pinR_PWM, 150);
      digitalWrite(pinL_DIR, 0);
      digitalWrite(pinR_DIR, 0);

      delay(1000); // Move back for 1 second
      currentState = STOPPED;
      analogWrite(pinL_PWM, 0);
      analogWrite(pinR_PWM, 0);
      break; */
  }
  }

  
  
}
