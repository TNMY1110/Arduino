/*
  Fade

  This example shows how to fade an LED on pin 9 using the analogWrite()
  function.

  The analogWrite() function uses PWM, so if you want to change the pin you're
  using, be sure to use another PWM capable pin. On most Arduino, the PWM pins
  are identified with a "~" sign, like ~3, ~5, ~6, ~9, ~10 and ~11.

  This example code is in the public domain.

  https://docs.arduino.cc/built-in-examples/basics/Fade/
*/

int led = 9;         // the PWM pin the LED is attached to
int brightness = 0;  // how bright the LED is
int fadeAmount = 5;  // how many points to fade the LED by
int sensorPin = A3;   // 가변 저항
int sensorValue = 0;  // 가변 저항값

// the setup routine runs once when you press reset:
void setup() {
  // declare pin 9 to be an output:
  pinMode(led, OUTPUT);
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(sensorPin);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue * (5.0 / 1023.0);
  // print out the value you read:
  
  // set the brightness of pin 9:
  // analogWrite(led, brightness);
  sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);
  Serial.println(voltage);

  brightness = sensorValue / 4;
  analogWrite(led, brightness);

  // change the brightness for next time through the loop:
  // brightness = brightness + fadeAmount;

  // // reverse the direction of the fading at the ends of the fade:
  // if (brightness <= 0 || brightness >= 255) {
  //   fadeAmount = -fadeAmount;
  // }
  // // wait for 30 milliseconds to see the dimming effect
  // delay(30);
}
