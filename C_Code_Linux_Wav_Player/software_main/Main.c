#include <stdio.h>
#include <stdint.h>
#include "wavFunction.h"
#include "playFunction.h"

#define PCM_NAME "default"

#define ERROR_FUNC return -1

int main(int argc, char *argv[])
{
	int res;
	snd_pcm_t *handle;

	/*==Audio in device open.==*/
	res=snd_pcm_open(&handle, PCM_NAME, SND_PCM_STREAM_PLAYBACK, 0);
	if(res<0)
	{
		printf("Device open failed.");
		
		ERROR_FUNC;
	}

	/*==Read data==*/
	uint8_t *pWavData;
	WaveTotalInfo *pWavHeader;
	
	get_pcm_data(argv[1],&pWavData,&pWavHeader);

	/*==Set harware codec control. ==*/
	set_params_from_wav_header_to_hw(handle);	

	/*==Play raw data.==*/
	play_raw_data(handle, pWavData, pWavHeader->waveDataStart.dataSize);	
	
	free(pWavHeader);
	free(pWavData);
	return 0;
}
