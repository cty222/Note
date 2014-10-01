void play8bitsRawData(unsigned long sNum,File *pFile)
{
 (*pFile).seek(sNum);
 // unsigned long cTime= micros();
  for(int i =0;i<sectorSize;i++)
  {
    while(!(*pFile).available());
    digitalWrite(SDCARD_CS, LOW);
    buf[0]=(*pFile).read();
    digitalWrite(SDCARD_CS, HIGH);
    mcp4822VoltWrite(currentOutPut, ((*ptr)<<4));
  }
 /* cTime=micros()-cTime;
  Serial.println(cTime);*/
}

void play10bitsRawData(unsigned long sNum,File *pFile)
{
  (*pFile).seek(sNum);
  for(int i =0;i<sectorSize/2;i++)
  {
    while(!(*pFile).available());
    digitalWrite(SDCARD_CS, LOW);
    buf[0]=(*pFile).read();
    buf[1]=(*pFile).read();
    digitalWrite(SDCARD_CS, HIGH);
    mcp4822VoltWrite(currentOutPut, ((*ptr)<<2));
  }
}

void recordingWavFile(File *pFile)
{
  SD.remove("MIC.WAV");
  volatile static unsigned int *wPtr;
  byte buf[2];
  wPtr = (unsigned int *)buf;
  static unsigned long i=0;
  (*pFile) = SD.open("Mic.wav", FILE_WRITE);
  
  long wTime =millis();
  for(;i<11025*2;i++)
  {
    *wPtr=analogRead(0);
    (*pFile).write(buf,2);
    digitalWrite(AnologPin0,0);
    delayUs();
  }
  wTime = millis() - wTime;
  (*pFile).close();
  Serial.print("wTime: ");
  Serial.println(wTime);
}

static void (*playRawData)(unsigned long ,File *);
void playWavFile(File* pFile ,FileInfo* wavInfo, boolean randomFlag)
{
   checkedBits(wavInfo);
   long rTime;
   if(!randomFlag)
   {
      rTime =millis();
      for(int i=0;i<numSector;i++)
     {
      playRawData(pSector[i],pFile);
     }
   }
   else
   {
     rTime =millis();
     for(int i=0;i<numSector;i++)
     {
       playRawData(pSector[millis()%numSector],pFile);
     }
   }
    rTime = millis() - rTime;
    Serial.print("rTime: ");
    Serial.println(rTime);
    Serial.println("Done!!!!!!!");
}

void checkedBits(FileInfo* wavInfo)
{
    if(!memcmp((*wavInfo).fileName,"test.wav",sizeof("test.wav")))
    {
     playRawData = play8bitsRawData;
    }
    else if(!memcmp((*wavInfo).fileName,"MIC.WAV",sizeof("MIC.WAV")))
    {
      playRawData = play10bitsRawData;
    }
    else
    {
      playRawData = play8bitsRawData;
    }
}
