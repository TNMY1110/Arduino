const int ledPin = 13;
const int YledPin = 9;
const int buttonPin = 2;
int sensorPin = A0;   // select the input pin for the potentiometer
int sensorValue = 0;  // variable to store the value coming from the sensor
int buttonState = 0;
int lastbuttonState = 0;
int brightness = 0;
bool isOn = false;

void setup() {
  // pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(YledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  buttonState = digitalRead(buttonPin);
  sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);
  brightness = map(sensorValue, 0, 1023, 0, 255); // 값을 변환
  analogWrite(YledPin, brightness);

  if(buttonState == HIGH && lastbuttonState == LOW)
  {
    isOn = !isOn;
    delay(50);
  }

  lastbuttonState = buttonState;

  if(isOn)
  {
    digitalWrite(ledPin, HIGH); // LED 켜기
    // digitalWrite(YledPin, HIGH); // LED 켜기
  }

  else
  {
    digitalWrite(ledPin, LOW); // LED 끄기
    // digitalWrite(YledPin, LOW); // LED 켜기
  }  
}
