void setup()
{
  //for Debug
  Serial.begin(9600);
  
  //Audio setup
  pinMode(SW_AUDIO_OUT, OUTPUT);
  digitalWrite(SW_AUDIO_OUT, LOW);
  
  //Mic_In setup
  pinMode(SW_MIC_IN, OUTPUT);
  digitalWrite(SW_MIC_IN, HIGH);
 
  //SD_Detect
  pinMode(SD_DETECT, INPUT);
  
  //SS PIN =>set Master and slave
  //pinMode(10, OUTPUT);
  
  //?
  pinMode(AnologPin0, OUTPUT);
  
  //set MCP4822 CS to not slect
  pinMode(MCP4822_CS, OUTPUT); 
  digitalWrite(MCP4822_CS, HIGH);
  
  setSampleRateHZUp();
  if (!SD.begin(SDCARD_CS)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
}
