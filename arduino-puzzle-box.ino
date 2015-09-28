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
int lastButtonState = LOW;

// STAGE 2: Shine light for two seconds
const int lightPin = 0;
int lastLightState = LOW;

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

  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

void setLEDState()
{
  digitalWrite(led1Pin, led1State);
  digitalWrite(led2Pin, led2State);
  digitalWrite(led3Pin, led3State);
  digitalWrite(led4Pin, led4State);

  if(led1State == HIGH && led2State == HIGH){
    digitalWrite(activityPin, HIGH);
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
  
  Serial.println(lightInput); //Write the value of the photoresistor to the serial monitor.
   
  if(lastActivityState != lightState && lightState == HIGH)
    lastActivityTime = millis();  
    
  lastActivityState = lightState;

  // check if the light is on for 3 seconds
  if(millis() - lastActivityTime > 2000 && lightState == HIGH){
    led2State = HIGH;
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
    
}
