const int ledPin = 13;
const int YledPin = 9;
const int buttonPin = 2;
int sensorPin = A0;   // 가변 저항
int sensorValue = 0;  // 가변 저항값
int buttonState = 0;
int lastbuttonState = 0;
int brightness = 0;
bool isOn = false;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(YledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  buttonState = digitalRead(buttonPin);
  sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);
  // brightness = map(sensorValue, 0, 1023, 0, 255); // 값을 변환
  // brightness = (255L * sensorValue) / 1023;   // 맵 함수 대신 비례식 계산,  아두이노는 8bit이기 때문에 int값이 -32,768 ~ 32,767이라고 한다. long타입 명시를 사용해야 계산시 int값의 오버플로우가 일어나지 않음
  brightness = sensorValue / 4;  // 제미나이의 추천. 255와 1023은 거의 4배 차이이기 때문에(255*4 = 1020 ≈ 1023) 식을 간결하게 만든다면 이게 낫다고 한다...생각해보니 그러네

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
    // delay(sensorValue);

    // digitalWrite(ledPin, LOW); // LED 끄기
    // delay(sensorValue);
  }

  else
  {
    digitalWrite(ledPin, LOW); // LED 끄기
  }  
}
