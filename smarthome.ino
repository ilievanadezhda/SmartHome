#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include <dht.h>
#include <Servo.h>

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip (192, 168, 178, 29);

unsigned int localPort = 7000;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
int packetSize;
String dataRequest;
EthernetUDP Udp;

//Ultrasonic sensor
const int echoPin = 2;
const int trigPin = 3;
long duration;
float distance;

//Temperature and humidity sensor
const int dhtPin = 4;
dht d;

//HW-479
const int redPin = 5;
const int bluePin = 6;
const int greenPin = 7;

//Micro Servo
Servo servo;
int servoPin = 8;
int servoAngle = 0;

void setup() {
  Serial.begin(9600);

  //Ethernet and UDP
  Ethernet.begin(mac, ip);
  Udp.begin(localPort);

  //Ultrasonic sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //Servo
  servo.attach(servoPin);

  delay(1500);
}

void loop() {
  packetSize = Udp.parsePacket();
  if (packetSize > 0) {
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    String dataRequest(packetBuffer);
    if (dataRequest == "RED") {
      turnRedLightOn();
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.print("OK");
      Udp.endPacket();
    } else if (dataRequest == "BLUE") {
      turnBlueLightOn();
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.print("OK");
      Udp.endPacket();
    } else if (dataRequest == "GREEN") {
      turnGreenLightOn();
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.print("OK");
      Udp.endPacket();
    } else if (dataRequest == "WHITE") {
      turnWhiteLightOn();
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.print("OK");
      Udp.endPacket();
    } else if (dataRequest == "MAGENTA") {
      turnMagentaLightOn();
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.print("OK");
      Udp.endPacket();
    } else if (dataRequest == "YELLOW") {
      turnYellowLightOn();
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.print("OK");
      Udp.endPacket();
    } else if (dataRequest == "CYAN") {
      turnCyanLightOn();
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.print("OK");
      Udp.endPacket();
    } else if (dataRequest == "OFF") {
      turnLightOff();
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.print("OK");
      Udp.endPacket();
    } else if (dataRequest == "TEMPERATURE") {
      float temperature = measureTemperature();
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.print(temperature);
      Udp.endPacket();
    } else if (dataRequest == "HUMIDITY") {
      float humidity = measureHumidity();
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.print(humidity);
      Udp.endPacket();
    } else if (dataRequest == "EMAIL REPORT") {
      float temperature = measureTemperature();
      float humidity = measureHumidity();
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.print(temperature);
      Udp.print("|");
      Udp.print(humidity);
      Udp.endPacket();
    } else if (dataRequest == "CHECK FOR INTRUDER") {
      bool check = checkForIntruders();
      if (check) {
        Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
        Udp.print("There is something close to the house!");
        Udp.endPacket();
      } else {
        Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
        Udp.print("All good! No one around.");
        Udp.endPacket();
      }
    }
    memset(packetBuffer, 0, UDP_TX_PACKET_MAX_SIZE);
  }
}
bool checkForIntruders() {
  for (servoAngle = 0; servoAngle <= 180; servoAngle += 1) {
    servo.write(servoAngle);
    delay(10);
    float distance = measureDistance();
    if (distance <= 15) {
      servo.write(0);
      servoAngle = 0;
      return true;
    }
    delay(100);
  }
  for (servoAngle = 180; servoAngle >= 0; servoAngle -= 1) {
    servo.write(servoAngle);
    delay(10);
    float distance = measureDistance();
    if (distance <= 15) {
      servo.write(0);
      servoAngle = 0;
      return true;
    }
    delay(100);
  }
  return false;
}
float measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  return distance; //vo cm
}

float measureTemperature() {
  int check = d.read11(dhtPin);
  float temperature = d.temperature;
  return temperature;
}

float measureHumidity() {
  int check = d.read11(dhtPin);
  return d.humidity;
}
void turnRedLightOn() {
  analogWrite(bluePin, 0);
  analogWrite(greenPin, 0);
  analogWrite(redPin, 255);
}
void turnBlueLightOn() {
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 255);
}
void turnGreenLightOn() {
  analogWrite(redPin, 0);
  analogWrite(bluePin, 0);
  analogWrite(greenPin, 255);
}
void turnMagentaLightOn() {
  analogWrite(greenPin, 0);
  analogWrite(redPin, 255);
  analogWrite(bluePin, 255);

}
void turnYellowLightOn() {
  analogWrite(bluePin, 0);
  analogWrite(redPin, 255);
  analogWrite(greenPin, 255);

}
void turnCyanLightOn() {
  analogWrite(redPin, 0);
  analogWrite(greenPin, 255);
  analogWrite(bluePin, 255);
}
void turnWhiteLightOn() {
  analogWrite(redPin, 255);
  analogWrite(bluePin, 255);
  analogWrite(greenPin, 255);
}
void turnLightOff() {
  analogWrite(redPin, 0);
  analogWrite(bluePin, 0);
  analogWrite(greenPin, 0);
}
