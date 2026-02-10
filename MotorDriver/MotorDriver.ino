int AA = 6;
int AB = 7;

void setup() 
{
  pinMode(AA, OUTPUT);
  pinMode(AB, OUTPUT);
}

void loop() 
{
  digitalWrite(AA, HIGH);
  digitalWrite(AB, LOW);
  delay(1000);

  digitalWrite(AA, LOW);
  digitalWrite(AB, LOW);
  delay(1000);
}
