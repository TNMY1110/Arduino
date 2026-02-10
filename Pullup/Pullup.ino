const int ledPin = 13;
const int buttonPin = 2;

int buttonState = 0;

void setup() {
  // pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  buttonState = digitalRead(buttonPin);
  // digitalWrite(LED_BUILTIN, HIGH);
  // digitalWrite(LED_BUILTIN, LOW);
  if(buttonState == HIGH)
  {
    digitalWrite(ledPin, HIGH); // LED 켜기
  }
  else
  {
    digitalWrite(ledPin, LOW); // LED 끄기
  }  
}
