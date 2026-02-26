#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <math.h>

// ===== Wi-Fi 설정 =====
const char *ssid =  "5층";   // WiFi 이름
const char *password =  "48864886"; // WiFi 비밀번호

// 번호에 따른 ID와 토픽
const char *MQTT_ID = "temperature_05";
const char *TOPIC = "device/05/temperature";

IPAddress broker(192,168,0,79); // 라즈베리파이 IP
const int MQTT_PORT = 1883;     // 포트 번호

WiFiClient wclient;
PubSubClient client(wclient);

// ===== 타이밍 설정 =====
unsigned long previousMillis = 0;
const long interval = 1000;   // 2초마다 측정 및 전송

// ===== DHT11 설정 =====
#define DHTPIN 5     // 핀 번호
#define DHTTYPE    DHT11     // DHT 11

DHT dht(DHTPIN, DHTTYPE);

float lastTemp = 0.0;

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

void setup() {
  Serial.begin(115200);
  dht.begin();
  
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
    
    // ===== 온습도 =====
    float newT = dht.readTemperature();
    float newH = dht.readHumidity();

    if (isnan(newT) || isnan(newH) || newH >= 100.0) 
    {
      Serial.println("센서 읽기 실패!");
    }
    else if(newT >= 30.0 || fabs(newT - lastTemp) >= 1.0)
    {
      lastTemp = newT;

      char tempStr[10];
      char humiStr[10];
      dtostrf(newT, 4, 1, tempStr);
      dtostrf(newH, 4, 1, humiStr);

      client.publish(TOPIC, tempStr);
      Serial.println((String)TOPIC + " => " + String(tempStr) + " ℃");

      client.publish(TOPIC, humiStr);
      Serial.println((String)TOPIC + " => " + String(humiStr) + " %");
    }
    else
    {
      Serial.println("큰 변화 없음");
      Serial.print(newT);
      Serial.println(" ℃");
      Serial.print(newH);
      Serial.println(" %");
    }
  }
}
