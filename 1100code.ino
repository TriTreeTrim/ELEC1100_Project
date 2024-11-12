
const int pinL_Sensor = A5; // pin A5: left sensor
const int pinB_Sensor = A4; // pin A4: bumper sensor
const int pinR_Sensor = A3; // pin A3: right sensor
const int pinM_Sensor = A2; // pin A2: middle sensor

const int pinL_PWM = 9;  // pin D9: left motor speed
const int pinL_DIR = 10; // pin D10: left motor direction

const int pinR_PWM = 11; // pin D11: right motor speed
const int pinR_DIR = 12; // pin D12: right motor direction

// define variables to be used in script

int bumperSensor = 1; // not sensing white
int leftSensor = 1;   // not sensing white
int rightSensor = 1;  // not sensing white
int middleSensor = 0; // sensing white

int countBumper = 0; // bumper sensor not triggered yet
int countTJunc = 0;  // count no. of T-junctions encountered

int startTime = 300;    // Time for car to move a bit after start line
int turn360Time = 1500; // Time for car to turn 360 degrees
                        // value TO BE TESTED
int go30cmTime = 300;   // TASK15: Time for car to go backwards >= 30cm
                        // value TO BE TESTED

int inLoop = 0;   // if enter loop, change pwn values
int turn360 = 0;  // faster pwn when 360 turn
int afterCuv = 0; // change turn pwn values
int after360 = 0; // stop before turning
int backwards = 0;
enum State
{
    TURN_LEFT1,
    TO_LOOP,
    IN_LOOP_1,
    IN_LOOP,
    EXIT_LOOP,
    FOLLOWPATH_turnleft,
    FOLLOWPATH,
    TURN_360,
    GOTurn,
    BACKWARD
};
State currentstate;

void stop()
{
      analogWrite(pinL_PWM, 0);     // stop at the start position
    analogWrite(pinR_PWM, 0);  
}
// the setup function runs once when you press reset or power the board
void setup()
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
    digitalWrite(pinL_DIR, HIGH); // forward direction
    digitalWrite(pinR_DIR, HIGH); // forward direction
    analogWrite(pinL_PWM, 0);     // stop at the start position
    analogWrite(pinR_PWM, 0);     // stop at the start position
}
void loop()
{

    // Arduino is reading the sensor measurements
    bumperSensor = digitalRead(pinB_Sensor);
    leftSensor = digitalRead(pinL_Sensor);
    rightSensor = digitalRead(pinR_Sensor);
    middleSensor = digitalRead(pinM_Sensor);

    // TASK1-2: car stops at the start position when bumper sensor no trigger
    if (bumperSensor && countBumper == 0)
    {
        stop();
    }

    // TASK3: bumper sensor is triggered at the start position for the 1st time
    else if (!bumperSensor && countBumper == 0)
    {
        analogWrite(pinL_PWM, 150);
        analogWrite(pinR_PWM, 150);
        digitalWrite(pinL_DIR, 1);
        digitalWrite(pinR_DIR, 1);
        countBumper++;
        delay(startTime); // to let the car leave the start position with no miscount
        currentstate = TURN_LEFT1;
    }

    // TASK4-13: car is tracking on the white line
    else if (bumperSensor && countBumper == 1)
    {
        switch (currentstate)
        {
        case TURN_LEFT1:
            if (leftSensor && rightSensor)
            {
                // Continue forward
                analogWrite(pinL_PWM, 150);
                analogWrite(pinR_PWM, 150);
                digitalWrite(pinL_DIR, 1);
                digitalWrite(pinR_DIR, 1);
            }
         
            
            else if (!leftSensor && !rightSensor)
            {   stop();
                delay(50);
                analogWrite(pinL_PWM, 250);
                analogWrite(pinR_PWM, 250);
                digitalWrite(pinL_DIR, 0);
                digitalWrite(pinR_DIR, 1);
                delay(160);
                analogWrite(pinL_PWM, 150);
                analogWrite(pinR_PWM, 150);
                digitalWrite(pinL_DIR, 1);
                digitalWrite(pinR_DIR, 1); 
                delay(50);
                currentstate = TO_LOOP;
            }
            break;
        case TO_LOOP:
            // go to loop
             
            if (leftSensor && rightSensor)
            {
                // Continue forward
                analogWrite(pinL_PWM, 120);
                analogWrite(pinR_PWM, 120);
                digitalWrite(pinL_DIR, 1);
                digitalWrite(pinR_DIR, 1);
            }
            else{
                
                analogWrite(pinL_PWM, 250);
                analogWrite(pinR_PWM, 250);
                digitalWrite(pinL_DIR, 0);
                digitalWrite(pinR_DIR, 1);
                delay(160);
                currentstate = IN_LOOP_1;
            }
            break;
        case IN_LOOP_1:
            if (leftSensor && rightSensor)
            {
                // Continue forward
                analogWrite(pinL_PWM, 120);
                analogWrite(pinR_PWM, 120);
                digitalWrite(pinL_DIR, 1);
                digitalWrite(pinR_DIR, 1);
            }
            else if (leftSensor && !rightSensor)
            {
                // Turn right
                analogWrite(pinL_PWM, 100);
                analogWrite(pinR_PWM, 100);
                digitalWrite(pinL_DIR, 1);
                digitalWrite(pinR_DIR, 0);
            }
            else if (!leftSensor && rightSensor)
            {
                // Turn left
                analogWrite(pinL_PWM, 100);
                analogWrite(pinR_PWM, 100);
                digitalWrite(pinL_DIR, 0);
                digitalWrite(pinR_DIR, 1);
            }
            else if (!leftSensor && !rightSensor)
            {
                // face entering of square
                analogWrite(pinL_PWM, 250);
                analogWrite(pinR_PWM, 250);
                digitalWrite(pinL_DIR, 0);
                digitalWrite(pinR_DIR, 1);
                delay(160);
                currentstate = IN_LOOP;
            }
            break;
        case IN_LOOP:
            if (leftSensor && rightSensor)
            {
                // Continue forward
                analogWrite(pinL_PWM, 120);
                analogWrite(pinR_PWM, 120);
                digitalWrite(pinL_DIR, 1);
                digitalWrite(pinR_DIR, 1);
            }
            else if (leftSensor && !rightSensor)
            {
                // Turn right
                analogWrite(pinL_PWM, 100);
                analogWrite(pinR_PWM, 100);
                digitalWrite(pinL_DIR, 1);
                digitalWrite(pinR_DIR, 0);
            }
            else if (!leftSensor && rightSensor)
            {
                // Turn left
                analogWrite(pinL_PWM, 100);
                analogWrite(pinR_PWM, 100);
                digitalWrite(pinL_DIR, 0);
                digitalWrite(pinR_DIR, 1);
            }
            else if (!leftSensor && !rightSensor)
            {
                // out of loop
                analogWrite(pinL_PWM, 150);
                analogWrite(pinR_PWM, 150);
                digitalWrite(pinL_DIR, 0);
                digitalWrite(pinR_DIR, 1);
                delay(200);
                currentstate = EXIT_LOOP;
            }
            break;
        case EXIT_LOOP:
            if (leftSensor && rightSensor)
            {
                // Continue forward
                analogWrite(pinL_PWM, 120);
                analogWrite(pinR_PWM, 120);
                digitalWrite(pinL_DIR, 1);
                digitalWrite(pinR_DIR, 1);
            }
            else if (leftSensor && !rightSensor)
            {
                // Turn right
                analogWrite(pinL_PWM, 100);
                analogWrite(pinR_PWM, 100);
                digitalWrite(pinL_DIR, 1);
                digitalWrite(pinR_DIR, 0);
            }
            else if (!leftSensor && rightSensor)
            {
                // Turn left
                analogWrite(pinL_PWM, 100);
                analogWrite(pinR_PWM, 100);
                digitalWrite(pinL_DIR, 0);
                digitalWrite(pinR_DIR, 1);
            }
            else if (!leftSensor && !rightSensor)
            {
                // out of loop
                analogWrite(pinL_PWM, 250);
                analogWrite(pinR_PWM, 250);
                digitalWrite(pinL_DIR, 0);
                digitalWrite(pinR_DIR, 1);
                delay(150);
                currentstate = FOLLOWPATH_turnleft;
            }
            break;
            case FOLLOWPATH_turnleft:
            if (leftSensor && rightSensor)
            {
                // Continue forward
                analogWrite(pinL_PWM, 150);
                analogWrite(pinR_PWM, 150);
                digitalWrite(pinL_DIR, 1);
                digitalWrite(pinR_DIR, 1);
            }
            else if (leftSensor && !rightSensor)
            {
                // Turn right
                analogWrite(pinL_PWM, 150);
                analogWrite(pinR_PWM, 150);
                digitalWrite(pinL_DIR, 1);
                digitalWrite(pinR_DIR, 0);
            }
            else if (!leftSensor && rightSensor)
            {
                // Turn left
                analogWrite(pinL_PWM, 150);
                analogWrite(pinR_PWM, 150);
                digitalWrite(pinL_DIR, 0);
                digitalWrite(pinR_DIR, 1);
            }
            else if (!leftSensor && !rightSensor)
            {
                // enter curve
                analogWrite(pinL_PWM, 150);
                analogWrite(pinR_PWM, 150);
                digitalWrite(pinL_DIR, 0);
                digitalWrite(pinR_DIR, 1);
                delay(150);
                currentstate = FOLLOWPATH;
            }
            break;
            case FOLLOWPATH:
            if (leftSensor && rightSensor)
            {
                // Continue forward
                analogWrite(pinL_PWM, 150);
                analogWrite(pinR_PWM, 150);
                digitalWrite(pinL_DIR, 1);
                digitalWrite(pinR_DIR, 1);
            }
            else if (leftSensor && !rightSensor)
            {
                // Turn right
                analogWrite(pinL_PWM, 150);
                analogWrite(pinR_PWM, 150);
                digitalWrite(pinL_DIR, 1);
                digitalWrite(pinR_DIR, 0);
            }
            else if (!leftSensor && rightSensor)
            {
                // Turn left
                analogWrite(pinL_PWM, 150);
                analogWrite(pinR_PWM, 150);
                digitalWrite(pinL_DIR, 0);
                digitalWrite(pinR_DIR, 1);
            }
            else if (!leftSensor && !rightSensor)
            {
                // enter curve
                analogWrite(pinL_PWM, 250);
                analogWrite(pinR_PWM, 250);
                digitalWrite(pinL_DIR, 0);
                digitalWrite(pinR_DIR, 1);
                delay(150);
                currentstate = TURN_360;
            }
            break;
            case TURN_360:
            if (leftSensor && rightSensor)
            {
                // Continue forward
                analogWrite(pinL_PWM, 150);
                analogWrite(pinR_PWM, 150);
                digitalWrite(pinL_DIR, 1);
                digitalWrite(pinR_DIR, 1);
            }
            else if (leftSensor && !rightSensor)
            {
                // Turn right
                analogWrite(pinL_PWM, 150);
                analogWrite(pinR_PWM, 150);
                digitalWrite(pinL_DIR, 1);
                digitalWrite(pinR_DIR, 0);
            }
            else if (!leftSensor && rightSensor)
            {
                // Turn left
                analogWrite(pinL_PWM, 150);
                analogWrite(pinR_PWM, 150);
                digitalWrite(pinL_DIR, 0);
                digitalWrite(pinR_DIR, 1);
            }
            else if (!leftSensor && !rightSensor)
            {
                // enter curve
                analogWrite(pinL_PWM, 250);
                analogWrite(pinR_PWM, 250);
                digitalWrite(pinL_DIR, 0);
                digitalWrite(pinR_DIR, 1);
                delay(875);
                currentstate = GOTurn;
            }
            case GOTurn:
             if (leftSensor && rightSensor)
            {
                // Continue forward
                analogWrite(pinL_PWM, 150);
                analogWrite(pinR_PWM, 150);
                digitalWrite(pinL_DIR, 1);
                digitalWrite(pinR_DIR, 1);
            }
            else if (leftSensor && !rightSensor)
            {
                // Turn right
                analogWrite(pinL_PWM, 250);
                analogWrite(pinR_PWM, 250);
                digitalWrite(pinL_DIR, 1);
                digitalWrite(pinR_DIR, 0);
                delay(150);
            }
            else if (!leftSensor && rightSensor)
            {
                // Turn left
                analogWrite(pinL_PWM, 150);
                analogWrite(pinR_PWM, 150);
                digitalWrite(pinL_DIR, 0);
                digitalWrite(pinR_DIR, 1);
                delay(150);
            }
            else if (!bumperSensor){
                currentstate=BACKWARD;
            }
            break;
            case BACKWARD:
            analogWrite(pinL_PWM, 150);
            analogWrite(pinR_PWM, 150);
            digitalWrite(pinL_DIR, 0);
            digitalWrite(pinR_DIR, 0);
            delay(150);
            break;
        }
    }
}
