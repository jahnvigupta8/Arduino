#include <SoftwareSerial.h>


#define RX 10
#define TX 11
String WIFI_SSID = "jahnvi";       // Your WiFi ssid
String PASSWORD = "jahnvigupta";         // Password
String DEVICE_SECRET_KEY  = "v3B85EF6FAF3DD65";


String HOST = "api.pushingbox.com";
String PATH = "/pushingbox";
String PORT = "80";

int countTrueCommand;
int countTimeCommand;
boolean found = false;

SoftwareSerial esp8266(RX, TX);

const int trigPin = 9;
const int echoPin = 8;

void setup() {
  // initialize serial communication:
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop()
{
  // establish variables for duration of the ping, 
  // and the distance result in inches and centimeters:
  long duration, inches, cm;

  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  
  duration = pulseIn(echoPin, HIGH);

  // convert the time into a distance
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
  
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();

  if(inches < 5)
    Serial.print(inches);
  else{
    Serial.print("out of range");
    Notification(inches);
    delay(10000);
  }
  
  delay(25);
}

long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

void Notification(int inches) {
  Serial.begin(115200);
  esp8266.begin(115200);
  esp8266.println("AT");
  Serial.println(esp8266.read());
  sendCommandToESP8266("AT", 5, "OK");
  sendCommandToESP8266("AT+CWMODE=1", 5, "OK");
  sendCommandToESP8266("AT+CWJAP=\"" + WIFI_SSID + "\",\"" + PASSWORD + "\"", 20, "OK");
  link(inches);
}

void link(int inches) {

  String postRequest="GET /pushingbox?devid=v3B85EF6FAF3DD65&distance=" + String(inches) + " HTTP/1.1\r\nHost: api.pushingbox.com\r\n\r\n";

  sendCommandToESP8266("AT+CIPMUX=1", 5, "OK");
  sendCommandToESP8266("AT+CIPSTART=0,\"TCP\",\"" + HOST + "\"," + PORT, 15, "OK");
  String cipSend = "AT+CIPSEND=0," + String(postRequest.length());
  sendCommandToESP8266(cipSend, 4, ">");
  sendData(postRequest);
  sendCommandToESP8266("AT+CIPCLOSE=0", 5, "OK");
}

void sendCommandToESP8266(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while (countTimeCommand < (maxTime * 1))
  {
    esp8266.println(command);
    if (esp8266.find(readReplay))
    {
      found = true;
      break;
    }

    countTimeCommand++;
  }

  if (found == true)
  {
    Serial.println("Success");
    countTrueCommand++;
    countTimeCommand = 0;
  }

  if (found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }

  found = false;
}

void sendData(String postRequest) {
  Serial.println(postRequest);
  esp8266.println(postRequest);
  delay(1500);
  countTrueCommand++;
}
