/*
  SD card read/write
 
 This example shows how to read and write data to and from an SD card file 	
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 9
 
 created   Nov 2010
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 
 This example code is in the public domain.
 	 
 */
 
#include <SD.h>
#include <SPI.h>

File myFile;
const int CS_Pin = 8;
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
   
  if (!SD.begin(9)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
}



void loop()
{
  while(1)
  {
#if 1
  SD.remove("MIC.WAV");
  volatile static unsigned int *wPtr;
  byte wBuf[2];
  wPtr = (unsigned int *)wBuf;
  myFile = SD.open("Mic.wav", FILE_WRITE);
  
  long wTime =millis();
  unsigned int big=0;
  //analogReference(EXTERNAL);
  for(;;)
  {
    digitalWrite(14,0);
    *wPtr=analogRead(0);
    mcp4822VoltWrite(outA, ((*wPtr)<<2));
    /*if(big<(*wPtr))
    {
      big=(*wPtr);
      Serial.println((*wPtr));
    }*/
    //myFile.write(wBuf,2);
    
    delayUs();
  }
  wTime = millis() - wTime;
  myFile.close();
  Serial.print("wTime: ");
  Serial.println(wTime);
  delay(100);
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
  myFile.close();
  Serial.print("rTime: ");
  Serial.println(rTime);
  Serial.println("Done!!!!!!!");
  delay(100);
#endif 
  
  }


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

void play12bitsRawData(unsigned long sNum)
{
  myFile.seek(sNum);
  for(int i =0;i<sectorSize/2;i++)
  {
    while(!myFile.available());
    digitalWrite(9, LOW);
    buf[0]=myFile.read();
    buf[1]=myFile.read();
    digitalWrite(9, HIGH);
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
