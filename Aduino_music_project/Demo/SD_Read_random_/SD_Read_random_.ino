/*
  SD card read/write
 
 This example shows how to read and write data to and from an SD card file 	
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** SDCARD_CS - pin 8
*/ 

#include <SD.h>
#include <SPI.h>

File myFile;
const int MCP4822_CS = 9;
const int SDCARD_CS = 8;
const boolean outA =0;
const boolean outB =1;
boolean currentOutPut = outA;

const int SD_DETECT =4;
const int SW_MIC_IN =3;
const int SW_AUDIO_OUT =2;

unsigned long fileSize=0;
unsigned long *pSector;
unsigned int numSector=0;
unsigned long sectorSize=0;
byte buf[2]={0,0};
unsigned int *ptr=(unsigned int *) buf;


void setup()
{
  pinMode(SW_AUDIO_OUT, OUTPUT);
  digitalWrite(SW_AUDIO_OUT, LOW);
  
  pinMode(SW_MIC_IN, OUTPUT);
  digitalWrite(SW_MIC_IN, HIGH);
  
  pinMode(SD_DETECT, OUTPUT);
  digitalWrite(SD_DETECT, LOW);
  
  Serial.begin(9600);
  pinMode(10, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(MCP4822_CS, OUTPUT); 
  digitalWrite(MCP4822_CS, HIGH);
  setSampleRateHZUp();
   
  if (!SD.begin(SDCARD_CS)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
}



void loop()
{
  #if 0
  SD.remove("MIC.WAV");
  volatile static unsigned int *wPtr;
  byte buf[2];
  wPtr = (unsigned int *)buf;
  static unsigned long i=0;
  myFile = SD.open("Mic.wav", FILE_WRITE);
  
  long wTime =millis();
  for(;i<11025*2;i++)
  {
    *wPtr=analogRead(0);
    myFile.write(buf,2);
    digitalWrite(14,0);
    delayUs();
  }
  wTime = millis() - wTime;
  myFile.close();
  Serial.print("wTime: ");
  Serial.println(wTime);
#endif
  
#if 1
  myFile = SD.open("test.wav");
  if (myFile)
  {
    fileSize = myFile.size();
    Serial.println("file ready.");
  } 
  else 
  {
    Serial.println("file error");
  }
  numSector=50;
  splitSectorReset();
  while(1)
  {
    int a;
    int tes[numSector];
    memset(tes ,'\0',numSector*sizeof(int));
    
    long rTime =millis();

//#define RANDOM    
#ifndef RANDOM
    for(int i=0;i<numSector;i++)
    {
      play8bitsRawData(pSector[i]);
    }
    rTime = millis() - rTime;
    Serial.print("rTime: ");
    Serial.println(rTime);
    Serial.println("Done!!!!!!!");
#else
    for(int i=0;i<numSector;i++)
    {
      play8bitsRawData(pSector[millis()%numSector]);
    }
    rTime = millis() - rTime;
    Serial.print("rTime: ");
    Serial.println(rTime);
    Serial.println("Done!!!!!!!");
#endif
  }
#endif 

}



void splitSectorReset()
{
    free(pSector);
    pSector = (unsigned long *)malloc(numSector*sizeof(unsigned long));
    sectorSize = ((fileSize-44)/numSector);
    for(int i =0;i<numSector;i++)
    {
      pSector[i]=44+sectorSize*i;
      /*Serial.print("pSector[");
      Serial.print(i);
      Serial.print("]: ");
      Serial.println(pSector[i]);*/
    }
}

void play8bitsRawData(unsigned long sNum)
{
  myFile.seek(sNum);
 // unsigned long cTime= micros();
  for(int i =0;i<sectorSize;i++)
  {
    while(!myFile.available());
    digitalWrite(SDCARD_CS, LOW);
    buf[0]=myFile.read();
    digitalWrite(SDCARD_CS, HIGH);
    mcp4822VoltWrite(currentOutPut, ((*ptr)<<4));
  }
 /* cTime=micros()-cTime;
  Serial.println(cTime);*/
}

void play12bitsRawData(unsigned long sNum)
{
  myFile.seek(sNum);
  for(int i =0;i<sectorSize/2;i++)
  {
    while(!myFile.available());
    digitalWrite(SDCARD_CS, LOW);
    buf[0]=myFile.read();
    buf[1]=myFile.read();
    digitalWrite(SDCARD_CS, HIGH);
    mcp4822VoltWrite(currentOutPut, ((*ptr)<<2));
  }
}

/*the volt value range is between 0V and 4V.*/
void mcp4822VoltWrite(boolean outPin, unsigned int DataVolt)
{
  DataVolt &= 0x1FFF;
  DataVolt |= (outPin<<15)+0x1000;
  
  digitalWrite(MCP4822_CS, LOW);
  SPItransfer((byte)(DataVolt>>8));
  SPItransfer((byte)(DataVolt));
  while(!(SPSR & (1<<SPIF)));
  
  digitalWrite(MCP4822_CS, HIGH);
}

void setSampleRateHZUp()
{
  bitClear(ADCSRA,ADPS0);
  bitClear(ADCSRA,ADPS1);
  bitSet(ADCSRA,ADPS2);
  bitClear(SPCR,SPR0);
  bitClear(SPCR,SPR1);
  bitSet(SPSR,SPI2X);
}

void SPItransfer(byte data)
{
  SPDR = data;
  while(!(SPSR & (1<<SPIF)));
}

#define SampleRate6000 23
static void delayUs()
{
  static const int loopTime = SampleRate6000;
  for(volatile int i=0;i<loopTime;i++)
  {
  }
}
