/*********
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete project details at https://RandomNerdTutorials.com/esp32-esp8266-web-server-physical-button/
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*********/

// Import required libraries  // 라이브러리 선언
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Replace with your network credentials
const char* ssid = "5층";     // 와이파이 이름
const char* password = "48864886";    // 비밀번호

const char* PARAM_INPUT_1 = "state";

const int output = 2;     // 핀 번호
const int buttonPin = 4;  // 핀 번호

// Variables will change:
int ledState = LOW;          // 출력 핀의 현재 상태
int buttonState;             // 입력 핀에서 나오는 전류 읽기
int lastButtonState = LOW;   // 입력 핀의 이전 측정값

// the following variables are unsigned longs because the time, measured in
// 밀리초는 int에 저장할 수 있는 값보다 크게 변하므로 long타입  // 노이즈를 무시하기 위한 타이머 역할
unsigned long lastDebounceTime = 0;  // 출력 핀이 마지막으로 토글되는 순간
unsigned long debounceDelay = 50;    // 디바운스 시간, 출력이 깜빡이면 증가합니다.

// 포트 80에 AsyncWebServer 객체를 생성하기
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style> <!--폰트 스타일 지정--> 
    html {font-family: Arial; display: inline-block; text-align: center;} 
    h2 {font-size: 3.0rem;}
    p {font-size: 3.0rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 34px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 68px}
    input:checked+.slider {background-color: #2196F3}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
  </style>
</head>
<body>
  <h2>ESP Web Server</h2>
  %BUTTONPLACEHOLDER% <!--나중에 processor 함수가 이 글자를 찾아 HTML 버튼 코드로 갈아 끼워줍니다.-->
<script>function toggleCheckbox(element) {    <!--사용자가 웹 화면의 스위치를 클릭하면 즉시 /update?state=1 같은 주소로 신호를 보냅니다.-->
  var xhr = new XMLHttpRequest();
  if(element.checked){ xhr.open("GET", "/update?state=1", true); }
  else { xhr.open("GET", "/update?state=0", true); }
  xhr.send();
}

setInterval(function ( ) {      <!--1초마다 /state 주소에 접속해서 LED 상태를 물어봅니다. 덕분에 물리 버튼으로 LED를 켜도 웹 화면의 스위치가 자동으로 따라 움직입니다.-->
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var inputChecked;
      var outputStateM;
      if( this.responseText == 1){ 
        inputChecked = true;
        outputStateM = "On";
      }
      else { 
        inputChecked = false;
        outputStateM = "Off";
      }
      document.getElementById("output").checked = inputChecked;
      document.getElementById("outputState").innerHTML = outputStateM;
    }
  };
  xhttp.open("GET", "/state", true);
  xhttp.send();
}, 1000 ) ;
</script>
</body>
</html>
)rawliteral";

// 웹페이지에서 BUTTONPLACEHOLDER를 대체함
String processor(const String& var){    // BUTTONPLACEHOLDER를 찾아서 HTML 버튼 코드로 갈아 끼워줌
  //Serial.println(var);
  if(var == "BUTTONPLACEHOLDER"){
    String buttons ="";
    String outputStateValue = outputState();    // 버튼 상태를 표시할 HTML 텍스트
    buttons+= "<h4>Output - GPIO 2 - State <span id=\"outputState\"></span></h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"output\" " + outputStateValue + "><span class=\"slider\"></span></label>";
    return buttons;
  }
  return String();
}

String outputState(){   // 현재 출력을 반환하는 함수
  if(digitalRead(output)){
    return "checked";
  }
  else {
    return "";
  }
  return "";
}

void setup(){
  // 디버깅을 위한 직렬 포트
  Serial.begin(115200);

  pinMode(output, OUTPUT);
  digitalWrite(output, LOW);
  pinMode(buttonPin, INPUT);
  
  // 와이파이 연결
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // ESP Local IP 주소 출력
  Serial.println(WiFi.localIP());

  // 웹페이지 경로
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){  // processor 함수를 거쳐 완성된 HTML을 사용자에게 보냄
    request->send_P(200, "text/html", index_html, processor);
  });

  // GET request를 ESP_IP>/update?state=<inputMessage>로 보냄
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {    // 주소 뒤에 붙은 state 값을 읽어서 digitalWrite(output, 값)으로 실제 LED를 켜거나 끔
    String inputMessage;
    String inputParam;
    // GET input1 값은 <ESP_IP>/update?state=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      digitalWrite(output, inputMessage.toInt());
      ledState = !ledState;
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });

  // GET request를 <ESP_IP>/state로 보냄
  server.on("/state", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send(200, "text/plain", String(digitalRead(output)).c_str());
  });
  // 서버 시작
  server.begin();
}
  
void loop() {
  // 스위치의 상태를 로컬 변수로 읽어냄
  int reading = digitalRead(buttonPin);

  // 버튼을 누른건지 확인
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // 소음이나 누르는 행위로 인해 스위치가 변경된 경우
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {    // 버튼의 상태 변화가 debounceDelay(50ms)보다 오래 유지될 때만 "아, 진짜로 사람이 눌렀구나!"라고 판단하여 ledState를 반전
    // reading이 어떤 상태든 debounce보다 오래 존재했을 경우
    // 지연 상태를 실제 현재 상태로 받아들임

    // 버튼 상태가 변경된 경우
    if (reading != buttonState) {
      buttonState = reading;

      // 새 버튼 상태가 HIGH일 때만 LED를 토글
      if (buttonState == HIGH) {
        ledState = !ledState;
      }
    }
  }

  // LED 설정
  digitalWrite(output, ledState);   // digitalWrite(output, ledState)를 통해 웹에서 명령을 내렸든, 손가락으로 버튼을 눌렀든 최종 상태를 LED에 반영

  // 다음 loop에서 사용할 lastButtonState에 reading를 저장
  lastButtonState = reading;
}