struct fileInfoMation
{
  char* fileName;
  unsigned long fileSize;
};
#define FileInfo struct fileInfoMation

struct waveHeadTag
{
        char            riff[4];
        unsigned long    riffSize;
        char            wave[4];
        char            fmt[4];
        unsigned long    fmtSize;
};
#define WaveHead struct waveHeadTag

struct tagWaveFormat
{
        unsigned short  formatTag;
        unsigned short  channels;
        /*==The numbers of voice point in one sec.==*/
        unsigned long    samplesPerSec;
        /*==Total Byte in one sec.==*/
        unsigned long    avgBytesPerSec;
        /*==The byte length for one sample unit. ==*/
        unsigned short  blockAlign;
        /*==The bits size for a voice point using==*/
        unsigned short  bitsPerSample;
};
#define WaveFormat struct tagWaveFormat

struct waveDataStartTag
{
        char           data[4];
        unsigned long  dataSize;
} ;
#define WaveDataStart struct waveDataStartTag

struct waveTotalInfoTag
{
        WaveHead waveHead;
        WaveFormat waveFormat;
        WaveDataStart waveDataStart;
};
#define WaveTotalInfo struct waveTotalInfoTag

