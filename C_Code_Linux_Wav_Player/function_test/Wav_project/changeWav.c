#include <stdio.h>
#include <stdlib.h>
#include "wavHeader.h"

int countFileLength(FILE *pFile)
{
	 int length;
	 length=ftell(pFile);
	 fseek(pFile,0,SEEK_END);
	 length=ftell(pFile)-length;
	 fseek(pFile,0,SEEK_SET);
	 return length;
}

FILE *wavFile, *outputFile;
#define FILENAME   "10sec.wav"
#define FILEOUTPUT "test1.wav"

static int wavHeadSize=sizeof(WaveTotalInfo);
WaveTotalInfo* waveTotalInfo;

#define STARTSEC 0
#define TOTALSEC 10

void splitToTenPart(unsigned char* pWav, unsigned char** ptr)
{
	int i;
	for(i=STARTSEC;i<TOTALSEC;i++)
		*(ptr+i)=pWav+wavHeadSize+i*waveTotalInfo->waveFormat.avgBytesPerSec;
}

int main(void)
{
	wavFile=fopen(FILENAME,"rb");
	if(wavFile==0)
        {
                printf("Read wav Error.\n");
		return -1;
        }

	/*==read wav file.==*/
	int totalLength=countFileLength(wavFile);
	unsigned char *pWav=(unsigned char*)malloc(totalLength);
	fread(pWav,totalLength,1,wavFile);


#if 0
	int j;
	for(j=0;j<0x2C;j++)
		printf("%#x\n",*(pWav+j));
#endif

	/*==get wav info head.==*/
	waveTotalInfo=(WaveTotalInfo *)pWav;


#if 0
	printf("avgBytesPerSec: %#x\n",waveTotalInfo->waveFormat.avgBytesPerSec);	
	printf("count %d\n",waveTotalInfo->waveFormat.avgBytesPerSec*10+wavHeadSize);
#endif

	
	/*==set 10 part pointer==*/
	char* ptr[10];
	splitToTenPart(pWav,&ptr[0]);


#if 0
	printf("pWav: %#x\n",pWav);
	printf("totalLength: %#\n",totalLength);

	int i;
	for(i=0;i<10;i++)
		printf("%#x\n",ptr[i]);
#endif

//===============================================================

	outputFile=fopen(FILEOUTPUT,"wb");
	if(outputFile==0)
	{
		printf("Create wav Error.\n");
		return 0;
	}

	/*==Set new datalength in wav info temp==*/
	waveTotalInfo->waveDataStart.dataSize=waveTotalInfo->waveFormat.avgBytesPerSec*10+wavHeadSize;
	waveTotalInfo->waveHead.riffSize=waveTotalInfo->waveDataStart.dataSize+0x24;
	/*==Write wav info in new wav.==*/
	fwrite(waveTotalInfo,sizeof(WaveTotalInfo),1,outputFile);	

	/*==set data turn.==*/
	char* outPtr[10];
	outPtr[0]=ptr[3];
	outPtr[1]=ptr[1];
	outPtr[2]=ptr[6];
	outPtr[3]=ptr[2];
	outPtr[4]=ptr[0];
	outPtr[5]=ptr[7];
	outPtr[6]=ptr[9];
	outPtr[7]=ptr[4];
	outPtr[8]=ptr[8];
	outPtr[9]=ptr[5];
	
	int k;
	for(k=0;k<10;k++)
		fwrite(outPtr[k],waveTotalInfo->waveFormat.avgBytesPerSec,1,outputFile);

//===============================================================
	free(pWav);
	fclose(wavFile);
	fclose(outputFile);
	return 0;
}
