
#include <SoftwareSerial.h>
#define trigPin 10
#define echoPin 13
SoftwareSerial BTserial(10, 11); // RX | TX

int sensorPin = A0;

int sensorValue = 0;

float duration,distance;

void setup() {
  // put your setup code here, to run once:
  
BTserial.begin(9600);
Serial.begin (9600);
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);


}

void loop() {
  // put your main code here, to run repeatedly:
 
digitalWrite(trigPin,LOW);
delayMicroseconds(2);
digitalWrite(trigPin,HIGH);
delayMicroseconds(10);
digitalWrite(trigPin,LOW);

duration = pulseIn(echoPin,HIGH);
  Serial.println(duration);

distance = (duration/2) * 0.0343;

sensorValue = analogRead(sensorPin);

Serial.print("Distance = ");
if ( distance <= 40 || distance >=2)
{
  Serial.print(distance);
  Serial.println(" cm " );
    BTserial.print("1");BTserial.print(";");

}
else
{
  Serial.print(distance);
  Serial.println(" Out of Range " );
 // BTserial.print("0");BTserial.print(";");
}
delay(500);
}
