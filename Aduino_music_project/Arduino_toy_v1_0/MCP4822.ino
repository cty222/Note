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
