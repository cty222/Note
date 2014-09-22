#include "playFunction.h"

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

int set_params_from_wav_header_to_hw(snd_pcm_t *handle)
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
	
	return 0;
}

static snd_pcm_sframes_t (*writei_func)(snd_pcm_t *handle, const void *buffer, snd_pcm_uframes_t size);

int play_raw_data(snd_pcm_t *handle, uint8_t *pWavData, int dataSize)
{
        writei_func = snd_pcm_writei;

#define CUT_PART 10
#define DATA_UNIT_NUMBER (16/8)*2 /*(16bit/8bit) * 2 channel*/

	int i;
	for(i=0;i<CUT_PART;i++)
	{
		writei_func(handle, &pWavData[(dataSize/CUT_PART)*i], dataSize/(CUT_PART*DATA_UNIT_NUMBER));
	}
	return 0;
}

