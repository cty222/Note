/*
  SD card read/write
 
 This example shows how to read and write data to and from an SD card file 	
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4
 
 created   Nov 2010
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 
 This example code is in the public domain.
 	 
 */
 
#include <SD.h>
#include <SPI.h>

File myFile;
const int CS_Pin = 3;
const boolean outA =0;
const boolean outB =1;

unsigned long fileSize=0;
unsigned long *pSector;
unsigned int numSector=0;
unsigned long sectorSize=0;
byte buf[2]={0,0};
unsigned int *ptr=(unsigned int *) buf;


void setup()
{
  Serial.begin(9600);
  pinMode(10, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(CS_Pin, OUTPUT); 
  digitalWrite(CS_Pin, HIGH);
  setSampleRateHZUp();
   
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  

  

}



void loop()
{
  #if 1
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
  myFile = SD.open("Mic.wav");
  if (myFile)
  {
    fileSize = myFile.size();
    Serial.println("file ready.");
  } 
  else 
  {
    Serial.println("file error");
  }
  numSector=100;
  splitSectorReset();
  while(1)
  {
    int a;
    int tes[numSector];
    memset(tes ,'\0',numSector*sizeof(int));
    
    long rTime =millis();
    for(int i=0;i<numSector;i++)
    {
//      play8bitsRawData(pSector[i]);
      play12bitsRawData(pSector[i]);
    }
    rTime = millis() - rTime;
    Serial.print("rTime: ");
    Serial.println(rTime);
    Serial.println("Done!!!!!!!");
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
  unsigned long cTime= micros();
  for(int i =0;i<sectorSize;i++)
  {
    while(!myFile.available());
    digitalWrite(4, LOW);
    buf[0]=myFile.read();
    digitalWrite(4, HIGH);
    mcp4822VoltWrite(outA, ((*ptr)<<4));
  }
  cTime=micros()-cTime;
  Serial.println(cTime);
}

void play12bitsRawData(unsigned long sNum)
{
  myFile.seek(sNum);
  for(int i =0;i<sectorSize/2;i++)
  {
    while(!myFile.available());
    digitalWrite(4, LOW);
    buf[0]=myFile.read();
    buf[1]=myFile.read();
    digitalWrite(4, HIGH);
    mcp4822VoltWrite(outA, ((*ptr)<<2));
  }
}

/*the volt value range is between 0V and 4V.*/
void mcp4822VoltWrite(boolean outPin, unsigned int DataVolt)
{
  DataVolt &= 0x1FFF;
  DataVolt |= (outPin<<15)+0x1000;
  
  digitalWrite(CS_Pin, LOW);
  SPItransfer((byte)(DataVolt>>8));
  SPItransfer((byte)(DataVolt));
  while(!(SPSR & (1<<SPIF)));
  
  digitalWrite(CS_Pin, HIGH);
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
