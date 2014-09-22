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
