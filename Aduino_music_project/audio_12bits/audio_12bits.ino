#include <SPI.h>
#include "audio_data.h"

const int CS_Pin = 9;
const boolean outA =0;
const boolean outB =1;

#define SampleRate22050 24

void setup() {
  Serial.begin(9600);

  SPI.begin();
  pinMode(CS_Pin, OUTPUT);
  
}

void loop() {

  for(int i =0; i< sizeof(data1000Hz)/sizeof(unsigned int); i++)
  {
    mcp4822VoltWrite(outA, data1000Hz[i]);
    delay10Us();
  }

}


/*the volt value range is between 0V and 4V.*/
void mcp4822VoltWrite(boolean outPin, unsigned int DataVolt)
{
  static byte Data1,Data2;
  DataVolt &= 0x1FFF;
  DataVolt |= (outPin<<15)+0x1000;
  
  digitalWrite(CS_Pin, LOW);
  SPI.transfer((byte)(DataVolt>>8));
  SPI.transfer((byte)(DataVolt));
  digitalWrite(CS_Pin, HIGH);

}

static void delay10Us()
{
  static const int loopTime = SampleRate22050;
  for(volatile int i=0;i<loopTime;i++)
  {
  }
}
