#include <alsa/asoundlib.h>
#include <stdint.h>
#include "formats.h"

int set_params_from_wav_header_to_hw(snd_pcm_t *handle);
int play_raw_data(snd_pcm_t *handle, uint8_t *pWavData, int dataSize);

