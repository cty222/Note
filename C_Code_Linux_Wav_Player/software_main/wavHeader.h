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
        unsigned short  channels;

        /*==The numbers of voice point in one sec.==*/
        unsigned int    samplesPerSec;

        /*==Total Byte in one sec.==*/
        unsigned int    avgBytesPerSec;

        /*==The byte length for one sample unit. ==*/
        unsigned short  blockAlign;

        /*==The bits size for a voice point using==*/
        unsigned short  bitsPerSample;
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
