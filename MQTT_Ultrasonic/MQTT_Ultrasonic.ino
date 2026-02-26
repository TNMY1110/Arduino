#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// ===== Wi-Fi 설정 =====
const char *ssid =  "5층";   // WiFi 이름
const char *password =  "48864886"; // WiFi 비밀번호

// 번호에 따른 ID와 토픽
const char *MQTT_ID = "ultrasonic_05";
const char *TOPIC = "device/05/ultrasonic";

IPAddress broker(192,168,0,79); // 라즈베리파이 IP
const int MQTT_PORT = 1883;

WiFiClient wclient;
PubSubClient client(wclient);

const int trigPin = 14;
const int echoPin = 12;

long duration;
float distance;

unsigned long previousMillis = 0;
const long interval = 2000;   // 2초 (2000ms)

// Connect to WiFi network
void setup_wifi() {
  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password); // Connect to network

  while (WiFi.status() != WL_CONNECTED) { // Wait for connection
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// Reconnect to client
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(MQTT_ID)) {
      Serial.println("connected");

      Serial.print("Subscribe : ");
      Serial.println(TOPIC);
      Serial.println('\n');
      client.subscribe(TOPIC, 1);

      Serial.print("Publishing to: ");
      Serial.println(TOPIC);
      Serial.println('\n');

    } else {
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

float getDistance()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2.0;

  return distance;
}

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(115200);

  delay(100);
  setup_wifi(); // Connect to network

  client.setServer(broker, MQTT_PORT);
}

void loop() {
  if (!client.connected())  // Reconnect if connection is lost
  {
    reconnect();
  }
  client.loop();

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) 
  {
    previousMillis = currentMillis;
    float dist = getDistance();

    if(dist > 0.0)
    {
      char distStr[10];
      dtostrf(distance, 4, 1, distStr);

      client.publish(TOPIC, distStr);
      Serial.println((String)TOPIC + " => " + String(distStr) + " cm");
    }
    else 
    {
      Serial.println("측정 실패 (범위 초과)");
    }
  }
}
