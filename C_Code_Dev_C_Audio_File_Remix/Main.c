#include "stdio.h"
#include "stdlib.h" 
#include "WavProtocol.h"

int ReadPCMDataFromWavFile(WAV_FILE_DATA * ThisWavFileData)
{
	if (ThisWavFileData->FileName == NULL)
	{
		return -1;
	}
	
	FILE *wavFile;
	wavFile=fopen(ThisWavFileData->FileName, "rb");
	if(wavFile==0)
    {
                printf("Read wav Error.\n");
                return -1;
    }
    
    ThisWavFileData->WavInfo = (WaveTotalInfo *)malloc(sizeof(WaveTotalInfo));
    fread(ThisWavFileData->WavInfo, sizeof(WaveTotalInfo), 1, wavFile);
    
    int datasize = ThisWavFileData->WavInfo->waveDataStart.dataSize;
    
    ThisWavFileData->PCMData = (uint8_t *)malloc(datasize);
	fread(ThisWavFileData->PCMData, datasize, 1, wavFile);
	
	return 0;
}

int WritePCMDataInWavFile(WAV_FILE_DATA * ThisWavFileData)
{
	FILE *wavFile;
    wavFile=fopen(ThisWavFileData->FileName,"wb");
	
	if(wavFile==0)
    {
                printf("Output wav Error.\n");
                return -1;
    }

	/*==Write wav head info.==*/
	fwrite(ThisWavFileData->WavInfo, sizeof(WaveTotalInfo), 1, wavFile);

	int res;
	int datasize = ThisWavFileData->WavInfo->waveDataStart.dataSize;

	// fwrite return count
	res = fwrite(ThisWavFileData->PCMData, datasize, 1, wavFile);

	fclose(wavFile);
	return 0;
}

int GetPCMDataSize(WAV_FILE_DATA * ThisWavFileData)
{
	return ThisWavFileData->WavInfo->waveDataStart.dataSize;
}

/*void MixPCMData(WAV_FILE_DATA * OutPutFile, WAV_FILE_DATA * InputFile1, WAV_FILE_DATA * InputFile2) 
{
	
}*/

void FreeWavFileProtocol (WAV_FILE_PROTOCOL **ThisProtocol)
{
	if (ThisProtocol != NULL)
	{
		free(*ThisProtocol);
		ThisProtocol = NULL;
	}
}


WAV_FILE_PROTOCOL * CreateWavFileProtocol()
{
	WAV_FILE_PROTOCOL * ThisProtocol = malloc(sizeof(WAV_FILE_PROTOCOL));
	ThisProtocol->OpenWavFile 		= ReadPCMDataFromWavFile;
	ThisProtocol->WriteInWavFile	= WritePCMDataInWavFile;
	ThisProtocol->GetWavPCMDataSize	= GetPCMDataSize;
	ThisProtocol->FreeThisProtocol  = FreeWavFileProtocol;
	return ThisProtocol;
}

int main(int argc, char *argv[])
{
	int res;
	
	WAV_FILE_PROTOCOL *WavFileProtocol = CreateWavFileProtocol();
	
	/*==Read data==*/
	WAV_FILE_DATA WavFile1;
	WAV_FILE_DATA WavFile2;
	WAV_FILE_DATA WavFileOutPut;

	WavFile1.FileName = "01.wav";
	WavFile2.FileName = "02.wav";
	WavFileOutPut.FileName = "Output.wav";
	res = WavFileProtocol->OpenWavFile(&WavFile1);
	if (res != 0)
	{
		printf("File 1 open Error\n");
		return res;
	}

	res = WavFileProtocol->OpenWavFile(&WavFile2);
	if (res != 0)
	{
		printf("File 1 open Error\n");
		return res;
	}
	
	//
	int datasize1 = WavFileProtocol->GetWavPCMDataSize(&WavFile1);
	int datasize2 =  WavFileProtocol->GetWavPCMDataSize(&WavFile2);
	int SmallSize;
	if (datasize1 > datasize2)
	{
		WavFileOutPut.WavInfo = WavFile1.WavInfo;
		WavFileOutPut.PCMData = WavFile1.PCMData;
		
		SmallSize = datasize2;
	}
	else
	{
		WavFileOutPut.WavInfo = WavFile2.WavInfo;
		WavFileOutPut.PCMData = WavFile2.PCMData;
		
		SmallSize = datasize1;
	}
	
	int i;	

	for(i=0; i<SmallSize; i+=2) 
	{
		float a = (*(short *)(&WavFile1.PCMData[i])) / 36768.0f;
		float b = (*(short *)(&WavFile2.PCMData[i])) / 36768.0f;
		float mixed = a + b;
		mixed *= 0.8;
		if (mixed > 1.0f) mixed = 1.0f;
		if (mixed < -1.0f) mixed = -1.0f;

		short out = (short)(mixed * 32768.0f);
		
		WavFileOutPut.PCMData[i] = ((char *)&out)[0];
		WavFileOutPut.PCMData[i+1] = ((char *)&out)[1];
	}

	WavFileProtocol->WriteInWavFile(&WavFileOutPut);
	

	/*
	printf ("fmtSize %d\n", pWavData1->waveHead.fmtSize);
	printf ("riffSize %d\n", pWavData1->waveHead.riffSize);
	printf ("channels %d\n", pWavData1->waveFormat.channels);
	printf ("SamplesRate %d\n", pWavData1->waveFormat.SamplesRate);
	printf ("avgBytesPerSec %d\n", pWavData1->waveFormat.avgBytesPerSec);
	printf ("blockAlign %d\n", pWavData1->waveFormat.blockAlign);
	printf ("bitsPerSample %d\n", pWavData1->waveFormat.bitsPerUnit);
	printf ("dataSize %d\n", pWavData1->waveDataStart.dataSize);
	*/

#if 0	
	if (res == 0)
	{
		free(pWavHeader1);
		free(pWavData1);
		free(pWavHeader2);
		free(pWavData2);
	}
#endif
	
	WavFileProtocol->FreeThisProtocol(&WavFileProtocol);
	return res;
}
