#include <Wire.h>
#include <SPI.h>

const byte EEPROM_ID0 = 0x50;
const byte EEPROM_ID1 = 0x51;
const byte EEPROM_ID2 = 0x52;
const byte EEPROM_ID3 = 0x53;

const boolean outA =0;
const boolean outB =1;
const int CS_Pin = 9;

void setup()
{
  Serial.begin(115200);
  analogReference(EXTERNAL);
  Wire.begin();
  SPI.begin();
  pinMode(CS_Pin, OUTPUT);
}

int startTime;
int timeLength;
int buf[128];

void loop()
{
  //Serial.println("Start Read");
  //startTime = millis();
  while(1)
  {
    record128Point();
  //timeLength = millis()-startTime;
  }
  //Serial.println("Done");
  //Serial.print("timeLength :");
  //Serial.println(timeLength);
  
  
  /*for(int i =0;i<256;i++)
  {
    static byte lowData = I2CEEPROM_Read(i*2);
    static byte highData = I2CEEPROM_Read(i*2+1);
    static int res= (int)(highData<<8)|lowData;
    
    Serial.print("Data ");
    Serial.print(i);
    Serial.print(" = ");
    Serial.println(res,HEX);
  }*/

}



void record128Point()
{
  for(int i=0;i<128;i++)
  {
    buf[i]=analogRead(0);
    //I2CEEPROM_Write((i*2), (byte)(buf[i]));
    //I2CEEPROM_Write((i*2)+1, (byte)(buf[i]>>8));
    mcp4822VoltWrite(outA, buf[i]*4);
    //delayUS();
  }
}

void I2CEEPROM_Write(byte addr, byte data)
{
  Wire.beginTransmission(EEPROM_ID0);
  Wire.write(addr);
  Wire.write(data);
  Wire.endTransmission();
  delayUS();
}


byte I2CEEPROM_Read(byte addr)
{
  static byte rData;
  
  Wire.beginTransmission(EEPROM_ID0);
  Wire.write(addr);
  Wire.endTransmission();
  
  Wire.requestFrom(EEPROM_ID0,(byte)1);
  while(Wire.available()==0);
  rData = Wire.read();
  return rData;
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

static void delayUS()
{
  static const int loopTime = 100;
  for(volatile int i=0;i<loopTime;i++)
  {
  }
}
