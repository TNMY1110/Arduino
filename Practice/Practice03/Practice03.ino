const int AA = 6;   // IA
const int AB = 7;   // IB

const int trigPin = 9;  // 트리거
const int echoPin = 10; // 에코

long duration;
int distance;

int motorspeed = 255;
int disspeed = 30;

void setup() 
{
  pinMode(AA, OUTPUT);
  pinMode(AB, OUTPUT);
  pinMode(trigPin, OUTPUT); // 트리거 출력
  pinMode(echoPin, INPUT); // 에코 인풋
  Serial.begin(9600); // 시리얼
}

void loop() 
{
  digitalWrite(trigPin, LOW); // 트리거 핀 초기화?
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);  // 트리거핀 HIGH로 세팅
  delayMicroseconds(10);        // 10마이크로초간
  digitalWrite(trigPin, LOW);   // LOW로 세팅
  duration = pulseIn(echoPin, HIGH);  // 돌아온 시간
  distance = duration * 0.034 / 2;  // 거리 계산

  if(distance >= 25)    // 거리가 25 이상이면
  {
    motorspeed = 200;
  }
  else if(distance >= 15)   // 거리가 25 미만 15 이상일 때
  {
    motorspeed = map(distance, 15, 25, 100, 200); // 맵 함수를 사용해 속도 조절
  }
  else   // 거리가 15 미만일 때
  {
    motorspeed = 0;
  }
  analogWrite(AA, motorspeed);    // 정지
  digitalWrite(AB, LOW);

  Serial.print("Distance: ");
  Serial.println(distance);

  delay(50);
}