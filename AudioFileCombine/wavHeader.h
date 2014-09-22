typedef struct waveHeadTag
{
        char            riff[4];
        unsigned int    riffSize;
        char            wave[4];
        char            fmt[4];
        unsigned int    fmtSize;
} WaveHead;

typedef struct tagWaveFormat
{
        unsigned short  formatTag;
        
        /*== stereo = 2 ==*/
        unsigned short  channels;

        /*==The numbers of voice point in one sec.==*/
        unsigned int    SamplesRate;

        /*==Total Byte in one sec.==*/
        /*== SamplesRate x blockAlign ==*/
        unsigned int    avgBytesPerSec;

        /*==The byte length for one sample unit. ==*/
        /*== (stereo = 2) * (16bits = 2) = (blockAlign = 2)==*/
        unsigned short  blockAlign;

        /*==The bits size for a voice point using==*/
        /*== 16bits = 2 bytes  ==*/
        unsigned short  bitsPerUnit;
} WaveFormat;

typedef struct waveDataStartTag
{
        char            data[4];
        unsigned int    dataSize;
} WaveDataStart;

typedef struct waveTotalInfoTag
{
        WaveHead waveHead;
        WaveFormat waveFormat;
        WaveDataStart waveDataStart;
} WaveTotalInfo;

#define WAVE_HEAD_LENGTH sizeof(WaveTotalInfo)
