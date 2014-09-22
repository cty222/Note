#include <stdio.h>
#include <stdint.h>
#include <alsa/asoundlib.h>
#include "formats.h"
#include "wavHeader.h"

static snd_pcm_t *handle;
static snd_pcm_stream_t stream = SND_PCM_STREAM_PLAYBACK;
static int open_mode = 0;
static u_char *audiobuf = NULL;
static unsigned period_time = 0;
static unsigned buffer_time = 0;
static snd_pcm_uframes_t period_frames = 0;
static snd_pcm_uframes_t buffer_frames = 0;
static snd_pcm_uframes_t chunk_size = 0;

static struct {
        snd_pcm_format_t format;
        unsigned int channels;
        unsigned int rate;
} hwparams;

static snd_pcm_sframes_t (*writei_func)(snd_pcm_t *handle, const void *buffer, snd_pcm_uframes_t size);


static void set_params(void);
static void playback(char *name);


int main(int argc, char *argv[])
{
	writei_func = snd_pcm_writei;

	int err;
	char *pcm_name = "default";

	snd_pcm_info_t *info;
	snd_pcm_info_alloca(&info);

	stream = SND_PCM_STREAM_PLAYBACK;
	err = snd_pcm_open(&handle, pcm_name, stream, open_mode);

	if (err < 0) {
               // error(_("audio open error: %s"), snd_strerror(err));
                return 1;
        }

        if ((err = snd_pcm_info(handle, info)) < 0) {
               // error(_("info error: %s"), snd_strerror(err));
                return 1;
        }
	snd_pcm_nonblock(handle, 0);

        printf("set_params!!\n");
//	set_params();	

	//playbackv(&argv[1], argc0);
	printf("playback!!\n");
	playback(argv[1]);

	return 0;
}

static void set_params(void)
{
	hwparams.format=SND_PCM_FORMAT_S16_LE;
        hwparams.channels=2;
        hwparams.rate=44100;

	snd_pcm_hw_params_t *params;
	snd_pcm_sw_params_t *swparams;

        snd_pcm_hw_params_alloca(&params);
	snd_pcm_sw_params_alloca(&swparams);	

	snd_pcm_hw_params_any(handle, params);
	snd_pcm_hw_params_set_format(handle, params, hwparams.format);
	snd_pcm_hw_params_set_channels(handle, params, hwparams.channels);
	snd_pcm_hw_params_set_rate_near(handle, params, &hwparams.rate, 0);

	buffer_time=0;
	snd_pcm_hw_params_get_buffer_time_max(params,&buffer_time, 0);

	period_time=125000;
	snd_pcm_hw_params_set_period_time_near(handle, params,&period_time, 0);

	buffer_time = 500000;
	snd_pcm_hw_params_set_buffer_time_near(handle, params, &buffer_time, 0);
	
	/*monotonic = */snd_pcm_hw_params_is_monotonic(params);
        /*can_pause = */snd_pcm_hw_params_can_pause(params);

	
	printf("sizeof(params) : %d\n",sizeof(params));

        snd_pcm_hw_params(handle, params);

        snd_pcm_uframes_t buffer_size;
	snd_pcm_hw_params_get_period_size(params, &chunk_size, 0);

	size_t n=chunk_size;

	snd_pcm_sw_params_set_avail_min(handle, swparams, n);


	snd_pcm_uframes_t start_threshold, stop_threshold;	
	start_threshold=22050;
	snd_pcm_sw_params_set_start_threshold(handle, swparams, start_threshold);
	stop_threshold=22050;
	snd_pcm_sw_params_set_stop_threshold(handle, swparams, stop_threshold);
	
	snd_pcm_format_physical_width(hwparams.format);
	
}

static void playback(char *name)
{
	printf("name : %s\n",name);
	int fd;
	fd = open(name, O_RDONLY, 0);

	/* read the file header */
	WaveTotalInfo *waveTotalInfo=(WaveTotalInfo *)malloc(WAVE_HEAD_LENGTH);

	int a;
	a=read(fd, waveTotalInfo, WAVE_HEAD_LENGTH);

        printf("1. a= %d  WAVE_HEAD_LENGTH: %d\n",a,WAVE_HEAD_LENGTH);
	

	/* read data*/
	int rawDataSize=waveTotalInfo->waveDataStart.dataSize;
	uint8_t *rawData=(uint8_t *)malloc(rawDataSize);
	a=read(fd, rawData, rawDataSize);
	
	printf("2. a= %d  rawDataSize: %d\n",a,rawDataSize);
	sleep(1);


	int i;
#if 0
	printf("0000020 : ");
	for(i=0;i<rawDataSize;i++)
	{
		if(rawData[i]==0)
		{
			printf("0x00 ");
		}
		else
		{
			printf("%#02x ",rawData[i]);
		}
		if((i%16)==15)
		{
			printf("\n");
			printf("%06x0 : ",i/16+3);
		}
	}
#endif

#define CUT_PART 10
#define DATA_UNIT_NUMBER (16/8)*2 /*(16bit/8bit) * 2 channel*/
	set_params();
	for(i=0;i<CUT_PART;i++)
	{
                printf("%d\n",(rawDataSize/CUT_PART)*i);
		writei_func(handle, &rawData[(rawDataSize/CUT_PART)*i],rawDataSize/(CUT_PART*DATA_UNIT_NUMBER));
	}
}
