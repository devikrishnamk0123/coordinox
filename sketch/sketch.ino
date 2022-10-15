// #include <SoftwareSerial.h>

#define triggerPinX D5 
#define echoPinX D6      

#define triggerPinY D1   
#define echoPinY D2   

#define calibrationPinX D7 
#define calibrationPinY D8    

#define activationPin D3

// The unit is cm/microseconds
#define acousticVelocity 0.0343

int distanceX, distanceY;

// SoftwareSerial btSerial(RX, TX);
// String ledb = "";

int getXCoordinate() {
  long duration;
  float distance;

  digitalWrite(triggerPinX, LOW);
  delayMicroseconds(2);

  digitalWrite(triggerPinX, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPinX, LOW);

  duration = pulseIn(echoPinX, HIGH);
  distance = (duration * acousticVelocity) / 2;

  return (int) distance;
}

int getYCoordinates() {
  long duration;
  float distance;

  digitalWrite(triggerPinY, LOW);
  delayMicroseconds(2);

  digitalWrite(triggerPinY, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPinY, LOW);

  duration = pulseIn(echoPinY, HIGH);
  distance = (duration * acousticVelocity) / 2;

  return (int) distance;
}

void setup() {
  // put your setup code here, to run once:

  Serial.begin(74880);
  // Serial.begin(115200);
  //Serial.begin(921600);
  pinMode(triggerPinX, OUTPUT);
  pinMode(echoPinX, INPUT);

  pinMode(triggerPinY, OUTPUT);
  pinMode(echoPinY, INPUT);

  pinMode(calibrationPinX, INPUT);
  pinMode(calibrationPinY, INPUT);
  pinMode(activationPin, INPUT);

  pinMode(LED_BUILTIN, OUTPUT);
  
}

void printProjectionCoordinates(int x, int y) {
    Serial.println("----------  Projection Coordinates  ----------");
    Serial.print("X: ");
    Serial.print(x);
    Serial.print(" cm\t"); 
    Serial.print("Y: ");
    Serial.print(y);
    Serial.println(" cm");
}

void loop() {

  digitalWrite(activationPin, LOW);

  if(digitalRead(calibrationPinX) == HIGH)
    Serial.println("X coordinate calibrated");

  if(digitalRead(calibrationPinY) == HIGH)
    Serial.println("Y coordinate calibrated");

  if(digitalRead(activationPin) == LOW) 
  {
    Serial.println("Device activated");
    digitalWrite(LED_BUILTIN, HIGH);

    distanceX = getXCoordinate();
    distanceY = getYCoordinates(); 

    printProjectionCoordinates(distanceX, distanceY);
  }
}
