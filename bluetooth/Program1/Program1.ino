
#define trigPin 9
#define echoPin 10

float duration,distance;

void setup() {
  // put your setup code here, to run once:
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

Serial.print("Distance = ");
if ( distance >= 40 || distance <=2)
{
  Serial.println(distance);
  Serial.println("Out of Range");
}
else
{
  Serial.print(distance);
  Serial.println(" cm " );
  delay(500);
}
}

