const int AA = 6;   // IA
const int AB = 7;   // IB

const int trigPin = 9;  // 트리거
const int echoPin = 10; // 에코

long duration;
int distance;

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

  if(distance <= 5)   // 거리가 5 이하일 때
  {
    digitalWrite(AA, LOW);    // 정지
    digitalWrite(AB, LOW);
  }
  else    // 아니라면
  {
    digitalWrite(AA, HIGH);   // 모터 작동
    digitalWrite(AB, LOW);
  }

  Serial.print("Distance: ");
  Serial.println(distance);
}