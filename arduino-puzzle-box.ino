#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 
int pos = 0;    // variable to store the servo position 

const int led1Pin = 2;
int led1State = LOW;

const int led2Pin = 3;
int led2State = LOW;

const int led3Pin = 4;
int led3State = LOW;

const int led4Pin = 5;
int led4State = LOW;

const int activityPin = 6;
int lastActivityState = LOW;
int delayValue = 50;
long lastActivityTime = 0;

// STAGE 1: Hold button for two seconds
const int buttonPin = 7;

// STAGE 2: Shine light for two seconds
const int lightPin = 0;

// STAGE 3: Twist knob to correct position
const int potPin = 1;

// STAGE 4: Tilt box to correct position
const int tiltPin = 2;

void setup()
{
  pinMode(led1Pin, OUTPUT);
  digitalWrite(led1Pin, led1State);
  
  pinMode(led2Pin, OUTPUT);
  digitalWrite(led2Pin, led2State);
  
  pinMode(led3Pin, OUTPUT);
  digitalWrite(led3Pin, led3State);
  
  pinMode(led4Pin, OUTPUT);
  digitalWrite(led4Pin, led4State);
  
  pinMode(activityPin, OUTPUT);
  digitalWrite(activityPin, lastActivityState);
  
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

  myservo.attach(9);  // attaches the servo on pin 9 to the servo object 
  myservo.write(pos);
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

void openBox()
{
  for(pos = 0; pos <= 180; pos += 1) // goes from 0 degrees to 180 degrees 
    {                                  // in steps of 1 degree 
      myservo.write(pos);              // tell servo to go to position in variable 'pos' 
      delay(15);                       // waits 15ms for the servo to reach the position 
    } 
}

void setLEDState()
{
  digitalWrite(led1Pin, led1State);
  digitalWrite(led2Pin, led2State);
  digitalWrite(led3Pin, led3State);
  digitalWrite(led4Pin, led4State);

  if(led1State == HIGH && led2State == HIGH && led3State == HIGH && led4State == HIGH){
    digitalWrite(activityPin, HIGH);
    if(pos == 0)
      openBox();
  }
    
}

void checkActivity()
{ 
  if(lastActivityState == HIGH) {
    digitalWrite(activityPin, HIGH);
    delay(delayValue);
    digitalWrite(activityPin, LOW);
    delay(delayValue);
  }
}

void resetLastActivity()
{
  // reset last activity state and time
  lastActivityState = LOW;
  lastActivityTime = 0;
}

void stageOne()
{  
  checkActivity();
  // read the state of the switch into a local variable:
  int buttonState = digitalRead(buttonPin);
  
  if(lastActivityState != buttonState && buttonState == HIGH)
    lastActivityTime = millis();  
    
  lastActivityState = buttonState;

  // check if the button is held for 2 seconds
  if(millis() - lastActivityTime > 2000 && buttonState == HIGH){
    led1State = HIGH; 
    resetLastActivity();
  }   
}

void stageTwo()
{ 
  checkActivity();
  int lightInput = analogRead(lightPin);
  int lightState = LOW;
  if(lightInput > 700){
    lightState = HIGH;
  }
   
  if(lastActivityState != lightState && lightState == HIGH)
    lastActivityTime = millis();  
    
  lastActivityState = lightState;

  // check if the light is on for 3 seconds
  if(millis() - lastActivityTime > 2000 && lightState == HIGH){
    led2State = HIGH;
    resetLastActivity();
  }  
}

void stageThree()
{ 
  checkActivity();
  int potInput = analogRead(potPin);
  int potState = LOW;
  if(770 < potInput && potInput < 780){
    potState = HIGH;
  }
   
  if(lastActivityState != potState && potState == HIGH)
    lastActivityTime = millis();  
    
  lastActivityState = potState;

  // check if the light is on for 3 seconds
  if(millis() - lastActivityTime > 2000 && potState == HIGH){
    led3State = HIGH;
    resetLastActivity();
  }  
}

void stageFour()
{  
  checkActivity();
  int tiltInput = analogRead(tiltPin);
  int tiltState = LOW;
  if(tiltInput == 1023){
    tiltState = HIGH;
  }
  
  if(lastActivityState != tiltState && tiltState == HIGH)
    lastActivityTime = millis();  
    
  lastActivityState = tiltState;

  // check if the button is held for 2 seconds
  if(millis() - lastActivityTime > 2000 && tiltState == HIGH){
    led4State = HIGH; 
    resetLastActivity();
  }   
}

void loop()
{
  setLEDState();
    
  if(led1State == LOW)
    stageOne();
    
  if(led2State == LOW && led1State == HIGH)
    stageTwo();
    
  if(led3State == LOW && led2State == HIGH && led1State == HIGH)
    stageThree();

  if(led4State == LOW && led3State == HIGH && led2State == HIGH && led1State == HIGH)
    stageFour();
    
}
