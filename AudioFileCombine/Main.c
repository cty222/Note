#include "stdio.h"
#include "stdlib.h" 
#include "wavFunction.h"

int get_pcm_data(char *fileName, uint8_t **pWavData,WaveTotalInfo **pWavHeader)
{
	FILE *wavFile;
    wavFile=fopen(fileName,"rb");
	
	if(wavFile==0)
    {
                printf("Read wav Error.\n");
                return -1;
    }

	/*==Read wav head info.==*/
	*pWavHeader = (WaveTotalInfo *)malloc(sizeof(WaveTotalInfo));
	fread(*pWavHeader,sizeof(WaveTotalInfo),1,wavFile);

	*pWavData = (uint8_t *)malloc((*pWavHeader)->waveDataStart.dataSize);
	fread(*pWavData,(*pWavHeader)->waveDataStart.dataSize,1,wavFile);

	return 0;
}

int Out_pcm_data(char *fileName, uint8_t *pWavData, WaveTotalInfo *pWavHeader)
{
	FILE *wavFile;
    wavFile=fopen(fileName,"wb");
	
	if(wavFile==0)
    {
                printf("Output wav Error.\n");
                return -1;
    }

	/*==Write wav head info.==*/
	fwrite(pWavHeader, sizeof(WaveTotalInfo), 1, wavFile);

	int res;
	int datasize = pWavHeader->waveDataStart.dataSize;

	// fwrite return count
	res = fwrite(pWavData, datasize, 1, wavFile);

	fclose(wavFile);
	return 0;
}


static char* First1 = "01.wav";
static char* Second2 = "02.wav";
static char* Output = "Output.wav";

int main(int argc, char *argv[])
{
	int res;
	
	/*==Read data==*/
	uint8_t *pWavData1;
	WaveTotalInfo *pWavHeader1;
	
	uint8_t *pWavData2;
	WaveTotalInfo *pWavHeader2;
	
	uint8_t *pWavDataOut;
	WaveTotalInfo *pWavHeaderOut;
	
	res = get_pcm_data(First1,&pWavData1,&pWavHeader1);
	if (res != 0)
	{
		printf("File 1 open Error\n");
		return res;
	}
	
	res = get_pcm_data(Second2,&pWavData2,&pWavHeader2);
	if (res != 0)
	{
		printf("File 2 open Error\n");
		return res;
	}
	
	
	//
	int datasize1 = pWavHeader1->waveDataStart.dataSize;
	int datasize2 = pWavHeader2->waveDataStart.dataSize;
	int SmallSize;
	if (datasize1 > datasize2)
	{
		pWavHeaderOut = pWavHeader1;
		pWavDataOut = pWavData1;
		
		SmallSize = datasize2;
	}
	else
	{
		pWavHeaderOut = pWavHeader2;
		pWavDataOut = pWavData2;
		
		SmallSize = datasize1;
	}
	
	int i;
#if 1
	for(i=0; i<SmallSize; i+=2) 
	{
		float a = (*(short *)(&pWavData1[i])) / 36768.0f;
		float b = (*(short *)(&pWavData2[i])) / 36768.0f;
		float mixed = a + b;
		mixed *= 0.8;
		if (mixed > 1.0f) mixed = 1.0f;
		if (mixed < -1.0f) mixed = -1.0f;

		short out = (short)(mixed * 32768.0f);
		
		pWavDataOut[i] = ((char *)&out)[0];
		pWavDataOut[i+1] = ((char *)&out)[1];
	}
#endif
	Out_pcm_data(Output, pWavDataOut, pWavHeaderOut);
	
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
	
	if (res == 0)
	{
		free(pWavHeader1);
		free(pWavData1);
		free(pWavHeader2);
		free(pWavData2);
	}
	return res;
}
