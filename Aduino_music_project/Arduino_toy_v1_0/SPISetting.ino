
//set Spi faster by datasheet
void setSampleRateHZUp()
{
  bitClear(ADCSRA,ADPS0);
  bitClear(ADCSRA,ADPS1);
  bitSet(ADCSRA,ADPS2);
  bitClear(SPCR,SPR0);
  bitClear(SPCR,SPR1);
  bitSet(SPSR,SPI2X);
}

//SpitransData function by datasheet
void SPItransfer(byte data)
{
  SPDR = data;
  while(!(SPSR & (1<<SPIF)));
}

//For turning times
#define SampleRate6000 23
static void delayUs()
{
  static const int loopTime = SampleRate6000;
  for(volatile int i=0;i<loopTime;i++)
  {
  }
}
