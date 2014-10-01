#include <SD.h>
#include <SPI.h>
#include "InfoDefine.h"
#include "WavHeader.h"

boolean currentOutPut = outA;
unsigned long *pSector;
unsigned int numSector=0;
unsigned long sectorSize=0;
byte buf[2]={0,0};
unsigned int *ptr=(unsigned int *) buf;

void loop()
{
  File myFile;
  FileInfo wavFileInfo;
  wavFileInfo.fileName = "test.wav";
  if(!openFile(&myFile,&wavFileInfo))
  {
    while(1)
    {
      delay(10000);
    }
  }

  numSector=50;
  splitSectorReset(&pSector,wavFileInfo.fileSize);
  
  Serial.println("Start Playing");
  while(1)
  {
      playWavFile(&myFile,&wavFileInfo,false);
  }
}







