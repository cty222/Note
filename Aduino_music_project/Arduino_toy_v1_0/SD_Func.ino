boolean openFile(File *pFile, FileInfo* wavInfo)
{
  (*pFile) = SD.open((*wavInfo).fileName);
  if ((*pFile))
  {
    Serial.println("file ready.");
    (*wavInfo).fileSize = (*pFile).size();
    return true;
  } 
  else 
  {
    Serial.println("file error");
    (*wavInfo).fileSize = 0;
    return false;
  }
}

void splitSectorReset(unsigned long **ptr, unsigned long fileSize)
{
    free(*ptr);
    (*ptr) = (unsigned long *)malloc(numSector*sizeof(unsigned long));
    sectorSize = ((fileSize-44)/numSector);
    for(int i =0;i<numSector;i++)
    {
      (*ptr)[i]=44+sectorSize*i;
      /*Serial.print("(*ptr)[");
      Serial.print(i);
      Serial.print("]: ");
      Serial.println((*ptr)[i]);*/
    }
}

