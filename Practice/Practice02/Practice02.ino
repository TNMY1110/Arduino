#include <Servo.h>
Servo myservo;
const int ledPin = 13;
const int YledPin = 12;

int sensorPin = A0;   // 가변 저항 연결
int sensorValue = 0;  // 가변 저항값
int servoangle = 0;

void setup() 
{
  myservo.attach(9);
  pinMode(ledPin, OUTPUT);
  pinMode(YledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() 
{
  sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);

  servoangle = map(sensorValue, 0, 1023, 0, 180);
  myservo.write(servoangle);  // 서보 모터 작동
  

  if(servoangle >= 30 && servoangle <= 150)
  {
    digitalWrite(YledPin, HIGH);  // 노랑불 켜기
    digitalWrite(ledPin, LOW);    // 빨간불 끄기
  }
  else
  {
    digitalWrite(ledPin, HIGH);   // 빨간불 켜기
    digitalWrite(YledPin, LOW);   // 노랑불 끄기
  }
}
