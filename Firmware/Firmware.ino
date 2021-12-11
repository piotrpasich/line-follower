
// Include Libraries
#include "Arduino.h"
#include "DCMDriverL298.h"
#include "Button.h"


// Pin Definitions
#define DCMOTORDRIVERL298_PIN_INT1  4
#define DCMOTORDRIVERL298_PIN_ENB 3
#define DCMOTORDRIVERL298_PIN_INT2  5
#define DCMOTORDRIVERL298_PIN_ENA 2
#define DCMOTORDRIVERL298_PIN_INT3  6
#define DCMOTORDRIVERL298_PIN_INT4  7
#define IRLINEFOLLOW_1_PIN_OUT  8
#define IRLINEFOLLOW_2_PIN_OUT  9
#define IRLINEFOLLOW_3_PIN_OUT  10 // srodkowy

#define FORWARD 0
#define BACKWARD 1
#define MAX_SPEED 250
double multiplier = 0.4;
double GO_BACK_MULTIPLIER = 0.7;


// Global variables and defines

// object initialization
DCMDriverL298 dcMotorDriverL298(DCMOTORDRIVERL298_PIN_ENA,DCMOTORDRIVERL298_PIN_INT1,DCMOTORDRIVERL298_PIN_INT2,DCMOTORDRIVERL298_PIN_ENB,DCMOTORDRIVERL298_PIN_INT3,DCMOTORDRIVERL298_PIN_INT4);



// define vars for testing menu
const int timeout = 10000;       //define timeout of 10 sec
char menuOption = 0;
long time0;

// Setup the essentials for your circuit to work. It runs first every time your circuit is powered with electricity.
void setup() 
{
    // Setup Serial which is useful for debugging
    // Use the Serial Monitor to view printed messages
    Serial.begin(9600);
    while (!Serial) ; // wait for serial port to connect. Needed for native USB
    Serial.println("start");
    
    pinMode(IRLINEFOLLOW_1_PIN_OUT, INPUT);
    pinMode(IRLINEFOLLOW_2_PIN_OUT, INPUT);
    pinMode(IRLINEFOLLOW_3_PIN_OUT, INPUT);
}

bool leftLineCrossed = false;
bool rightLineCrossed = false; 
void loop() 
{
    bool leftSensor = digitalRead(IRLINEFOLLOW_2_PIN_OUT);
    bool rightSensor = digitalRead(IRLINEFOLLOW_1_PIN_OUT);
    bool centerSensor = digitalRead(IRLINEFOLLOW_3_PIN_OUT);

    double MULTIPLIER = multiplier;

    int leftMotorSpeed = MAX_SPEED * (leftSensor && !centerSensor ? MULTIPLIER : 1);
    int rightMotorSpeed = MAX_SPEED * (rightSensor && !centerSensor ? MULTIPLIER : 1);

    if (leftSensor) {
      leftLineCrossed = true;
    }
    if (rightSensor) {
      rightLineCrossed = true;
    }
    if (centerSensor && (rightLineCrossed || leftLineCrossed)) {
      if (leftLineCrossed) {
        rightMotorSpeed = MAX_SPEED * GO_BACK_MULTIPLIER;
        leftMotorSpeed = MAX_SPEED;
      }
      if (rightLineCrossed) {
        leftMotorSpeed = MAX_SPEED * (GO_BACK_MULTIPLIER) ;
        rightMotorSpeed = MAX_SPEED;
      }
      dcMotorDriverL298.setMotorA(rightMotorSpeed, FORWARD);
      dcMotorDriverL298.setMotorB(leftMotorSpeed, FORWARD);
      Serial.println("BACK");
      delay(200);
      leftLineCrossed = false;
      rightLineCrossed = false;
    }
    
    
    if (!centerSensor && !leftSensor && !rightSensor) {
      if (leftLineCrossed) {
        leftMotorSpeed = MAX_SPEED * GO_BACK_MULTIPLIER;
        rightMotorSpeed = MAX_SPEED;
      } 
      if (rightLineCrossed) {
        rightMotorSpeed = MAX_SPEED * GO_BACK_MULTIPLIER;
        leftMotorSpeed = MAX_SPEED;
      } 
    } 

    
    Serial.println("|:");
    Serial.print(leftSensor);
    Serial.print("|");
    Serial.print(leftMotorSpeed);
    Serial.print("|c:");
    Serial.print(centerSensor );
    Serial.print("|");
    Serial.print(rightSensor);
    Serial.print("|");
    Serial.print(rightMotorSpeed);
    Serial.print("|");
    Serial.print(MULTIPLIER);
    Serial.print("|");
    
    dcMotorDriverL298.setMotorA(rightMotorSpeed, FORWARD);
    dcMotorDriverL298.setMotorB(leftMotorSpeed, FORWARD);
    delay(5);
    //Stop both motors
    //dcMotorDriverL298.stopMotors();
    //delay(200);    
}
