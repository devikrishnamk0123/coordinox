#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define triggerPinX D5 
#define echoPinX D6      

#define triggerPinY D1   
#define echoPinY D2   

#define calibrationPinX D7 
#define calibrationPinY D8    

#define activationPin D3

#define acousticVelocity 0.0343

int distanceX, distanceY;
String dataBuffer;
char payload[32];

const char* ssid="TUF A15";
const char* password = "qwertyui";

WiFiUDP Udp;
unsigned int localUdpPort = 4210;
char incomingPacket[256];
char replyPacket[] = "Connection Granted!";

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

// void printProjectionCoordinates(int x, int y) {
//     Serial.println("----------  Projection Coordinates  ----------");
//     Serial.print("X: ");
//     Serial.print(x);
//     Serial.print(" cm\t"); 
//     Serial.print("Y: ");
//     Serial.print(y);
//     Serial.println(" cm");
// }

void sendData(String opcode) {
  distanceX = getXCoordinate();
  distanceY = getYCoordinates(); 

  dataBuffer = opcode + ": " + String(distanceX) + ", " + String(distanceY);
  dataBuffer.toCharArray(payload, 32);
   
  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
  Udp.write(payload);
  Udp.endPacket();
}

void setup() {
  Serial.begin(19200);

  pinMode(triggerPinX, OUTPUT);
  pinMode(echoPinX, INPUT);

  pinMode(triggerPinY, OUTPUT);
  pinMode(echoPinY, INPUT);

  pinMode(calibrationPinX, INPUT);
  pinMode(calibrationPinY, INPUT);
  pinMode(activationPin, INPUT);

  pinMode(LED_BUILTIN, OUTPUT);

  Serial.println();
  Serial.print("Wifi connecting to ");
  Serial.println( ssid );

  WiFi.begin(ssid,password);

  Serial.println();
  Serial.print("Connecting");

  while( WiFi.status() != WL_CONNECTED ){
      delay(500);
      Serial.print(".");        
  }

  digitalWrite(LED_BUILTIN , HIGH);
  Serial.println();

  Serial.println("Wifi Connected Success!");
  Serial.print("NodeMCU IP Address : ");
  Serial.println(WiFi.localIP());

  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
}

void loop() {
  // put your main code here, to run repeatedly:

  int packetSize = Udp.parsePacket();

  if (packetSize)
  {
    // receive incoming UDP packets
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    Serial.printf("[Client]: %s\n", incomingPacket);

    // send back a reply, to the IP address and port we got the packet from
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(replyPacket);
    Udp.endPacket();
  }

  digitalWrite(activationPin, LOW);

  if(digitalRead(calibrationPinX) == HIGH)
  {
    sendData("cx");
    Serial.println("X coordinate calibrated");
  }

  if(digitalRead(calibrationPinY) == HIGH)
  {
    sendData("cy");
    Serial.println("Y coordinate calibrated");
  }

  if(digitalRead(activationPin) == LOW) 
  {
    Serial.println("Click activated");
    sendData("click");            
  }

  sendData("move");
  delay(100);
}
