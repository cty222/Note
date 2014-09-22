void setup()
{
  Serial.begin(9600);
  pinMode(14,OUTPUT);
}

void loop()
{
  while(1)
  {
    digitalWrite(14,LOW);
    unsigned int i = analogRead(0);
    Serial.println(i,HEX);
    delay(100);
    
  }
}

