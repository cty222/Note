void setup()
{
  Serial.begin(115200);
  analogReference(EXTERNAL);
}

int startTime;
int timeLength;
int buf[689];
void loop()
{
  Serial.println("Start Read");
  startTime = millis();
  
  for(int i=0;i<689;i++)
  {
    buf[i]=analogRead(0);
  }
  
  timeLength = millis()-startTime;
  Serial.println("Done");
  Serial.print("timeLength :");
  Serial.println(timeLength);
  delay(10000000);
  
}
