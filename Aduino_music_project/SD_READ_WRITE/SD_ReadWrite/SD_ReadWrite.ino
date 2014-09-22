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
#define SampleRate6000 55
#define wSec(x) (x*6000)

void setup()
{
  Serial.begin(9600);
  Serial.print("Initializing SD card...");
  pinMode(10, OUTPUT);
  pinMode(CS_Pin, OUTPUT); 
  digitalWrite(CS_Pin, HIGH);
  analogReference(EXTERNAL);
   
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  SD.remove("MIC.WAV");

#if 1
  myFile = SD.open("Mic.wav", FILE_WRITE);
  if (myFile) {

    byte buf[2];
    unsigned int *wPtr=(unsigned int *)buf;
    long wTime =millis();
    for(int i=0;/*i<wSec(5)*/;i++)
    {
      *wPtr=analogRead(1);
      Serial.println(*wPtr,HEX);
      myFile.write(buf,2);
    }
    wTime = millis() - wTime;
    myFile.close();
    Serial.print("wTime: ");
    Serial.println(wTime);
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
#endif  

#if 1
  Serial.println("Start reading");
  myFile = SD.open("Mic.wav");
  if (myFile) {
#if 0
    for(int i=0;i<44;i++)
    {
      myFile.read();
    }
#endif
    long tTime =millis();
    byte buf[2];
    unsigned int *ptr = (unsigned int *)buf;
    while (myFile.available()) {
        digitalWrite(4, LOW);
        buf[0]=myFile.read();
        buf[1]=myFile.read();
        digitalWrite(4, HIGH);
        mcp4822VoltWrite(outA, ((*ptr)<<2));
        delayUs();
    }
    tTime = millis() - tTime;
    Serial.println(tTime);
    myFile.close();
  } else {
  	// if the file didn't open, print an error:
    Serial.println("error opening test.wav");
  }
#endif
}

void loop()
{
	// nothing happens after setup
}

/*the volt value range is between 0V and 4V.*/
void mcp4822VoltWrite(boolean outPin, unsigned int DataVolt)
{
  DataVolt &= 0x1FFF;
  DataVolt |= (outPin<<15)+0x1000;
  
  digitalWrite(CS_Pin, LOW);
  SPI.transfer((byte)(DataVolt>>8));
  SPI.transfer((byte)(DataVolt));
  digitalWrite(CS_Pin, HIGH);
}

static void delayUs()
{
  static const int loopTime = SampleRate6000;
  for(volatile int i=0;i<loopTime;i++)
  {
  }
}
