#include <SoftwareSerial.h>
SoftwareSerial mySerial(4, 2); // RX, TX
int inRightA = 5;
int inRightB = 3;
int inLeftA = 6;
int inLeftB = 9;
int state = -1;
String st = "";
char ch = 'a';
int wasBLC = 0;
int turnTime = 2000;
int waitTime = 50;
double stoppingDistance = 10.0; //minimum distance for detecting obstacles.

int triggerPin = 8;
int echoPin = 7;
#define rightIR A5
#define leftIR A4

//ir value that will be considered a gray/black point
int minWhiteValue = 600;
int maxBlackValue = 300;

//speed control
int HIGh = 120;
int MID = 50;
int LO = 0;

//position
int currX = 0;//current y value
int currY = 0;//current x value
int dir = 0; //current direcion

int comparisonTime = 100; //time between two test, of seeing if there is a gray dot
int TurnTime = 2000;//time for a 90 degree turn

void motorF(int msec)
{
   analogWrite(inRightA, MID);
   analogWrite(inRightB, LO);
   
   analogWrite(inLeftA, MID);
   analogWrite(inLeftB, LO);
   delay(msec);
}

void motorS()
{
   analogWrite(inRightA, LO);
   analogWrite(inRightB, LO);
   
   analogWrite(inLeftA, LO);
   analogWrite(inLeftB, LO);
}

void motorR(int msec)
{
   analogWrite(inRightA, LO);
   analogWrite(inRightB, LO);
   
   analogWrite(inLeftA, HIGh);
   analogWrite(inLeftB, LO);
   delay(msec);
}

void motorL(int msec)
{
   analogWrite(inRightA, HIGh);
   analogWrite(inRightB, LO);
   
   analogWrite(inLeftA, LO);
   analogWrite(inLeftB, LO);
   delay(msec);
}

void motorB(int msec)
{
   analogWrite(inRightA, LO);
   analogWrite(inRightB, MID);
   
   analogWrite(inLeftA, LO);
   analogWrite(inLeftB, MID);
   
   delay(msec);
   motorS();
}

double distance ()
{
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  double distance1 = (pulseIn(echoPin, HIGH)/58.0);
  Serial.println(distance1);
  return distance1;
}

void blackLineFollow()
{
   if (analogRead(rightIR) <= minWhiteValue && analogRead(leftIR) >= maxBlackValue) // Black line in the right.
   {
     // Clockwise.
     motorL(waitTime);
   }
   else if (analogRead(rightIR) >= maxBlackValue && analogRead(leftIR) <= minWhiteValue) // Black line in the left.
   {
     // Counterclockwise.
     motorR(waitTime);
   }
   else
   {
    motorF(1);
   }
}
void autoDrive()
{
  double Distance = distance();
  if (Distance >= stoppingDistance)
  {
    blackLineFollow();
  }
   else
   {
      // Object in front of the robot.
      motorS();
   }
}
void setup () {
   pinMode(inRightA, OUTPUT);
   pinMode(inRightB, OUTPUT);
   pinMode(inLeftA, OUTPUT);
   pinMode(inLeftB, OUTPUT);
   pinMode(triggerPin, OUTPUT);
   pinMode(echoPin, INPUT);
   analogWrite(inRightA, LO);
   analogWrite(inRightB, LO);
   analogWrite(inLeftA, LO);
   analogWrite(inLeftB, LO);
   Serial.begin(9600);
   mySerial.begin(9600);
   pinMode(13, OUTPUT);
   Serial.println("C");
}

void loop() {

  if (mySerial.available())
  {
    ch = mySerial.read();
  }
 
  if (ch != '1' && wasBLC)
  {
    motorS();
  }

  if (ch == '1')
  {
    autoDrive();
    wasBLC = 1;
  }
  else if (ch == '2')
  {
    motorR(50);
  }
  else if (ch == '3')
  {
    motorL(50);
  }
  else if (ch == '4')
  {
    motorF(50);
  }
  else if (ch == '5')
  {
    motorB(50);
  }
  else if (ch == '6')
  {
    motorS();
  }
}
