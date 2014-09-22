#include <Wire.h>
#include <SPI.h>
#include "sec16bit.h"

const unsigned int bufNum =3;
unsigned short *ptrData = raw16bitData0;

#if 1
const unsigned int DataLength = sizeof(raw16bitData0);
#else
const unsigned int DataLength = sizeof(raw16bitData22);
#endif
const byte EEPROM_ID_page0 = 0x50;


/*==DAC==*/
const int CS_Pin = 9;
const boolean outA =0;
const boolean outB =1;
#define SampleRate22050 280
/*===============*/

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  unsigned int address = bufNum*512;

/*==DAC==*/
  SPI.begin();
  pinMode(CS_Pin, OUTPUT);
/*======*/  

  
#if 0
  Serial.println("Writing....");

  for(unsigned int i=0;i<(DataLength/sizeof(unsigned int)) ;i++,address++)
  {
    I2CEEROM_Write(address*2, (byte)(*(ptrData+i)));
    delay(2);
    I2CEEROM_Write(address*2+1, (byte)((*(ptrData+i)>>8)));
    delay(2);
  }
  Serial.println("Write End!!");

  Serial.println("Read start!!");
  byte buf[2];
  unsigned int *ptr = (unsigned int *)buf;
  address = bufNum*512;
  for(unsigned int i=0;i<(DataLength/sizeof(unsigned int));i++,address++)
  {
    buf[0]=I2CEEROM_Read(address*2);
    buf[1]=I2CEEROM_Read(address*2+1);
    Serial.print("*ptr :");
    Serial.println(*ptr,HEX);
    delay(1);
  }
  Serial.println("Read End!!");
#endif
}

byte buf[2];
unsigned int *ptr = (unsigned int *)buf;
static unsigned int i;
void loop()
{
  long tTime =millis();
  for(i=0;i<(22050/sizeof(unsigned int));i=i+1*16)
  {
    buf[0]=I2CEEROM_Read(i*2);
    buf[1]=I2CEEROM_Read(i*2+1);
#if 1
    mcp4822VoltWrite(outA, (*ptr)*16);
    delay10Us();
#else
    /*Serial.print("*ptr :");
    Serial.println(*ptr,HEX);*/
#endif
  }
  tTime = millis() - tTime;
  Serial.println(tTime);
  delay(1000000);
 
}

void I2CEEROM_Write(unsigned int address, byte data)
{
  Wire.beginTransmission(EEPROM_ID_page0);
  Wire.write((byte)(address>>8));
  Wire.write((byte)address);
  Wire.write(data);
  Wire.endTransmission();
}


byte I2CEEROM_Read(unsigned int address)
{
  Wire.beginTransmission(EEPROM_ID_page0);
  Wire.write((byte)(address>>8));
  Wire.write((byte)address);
  Wire.endTransmission();

  Wire.requestFrom(EEPROM_ID_page0,(byte)1);
  while(Wire.available())
  {
     return Wire.read();
  }
  
}

void I2CEEPROM_Clear(unsigned int size)
{
  Serial.println("Start clear eeprom");
  for(unsigned int i=0;i<size;i++)
  {
    I2CEEROM_Write(i, 0x00);
    delay(2);
  }
  Serial.println("Clear complete");
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
