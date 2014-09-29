#include "wavHeader.h"

typedef unsigned char uint8_t;
#pragma pack(1)
typedef struct
{
	const char* 	FileName;
	WaveTotalInfo* 	WavInfo;
	uint8_t * 		PCMData;
} WAV_FILE_DATA;

typedef struct _WAV_FILE_PROTOCOL WAV_FILE_PROTOCOL;


typedef int (*OPEN_WAV_FILE) (
	WAV_FILE_DATA * ThisWavFileData
 	);

typedef int (*WRITE_IN_WAV_FILE) (
	WAV_FILE_DATA * ThisWavFileData
	);
	
typedef int (*GET_WAV_PCM_DATA_SIZE) (
	WAV_FILE_DATA * ThisWavFileData
	);

typedef void (*FREE_THIS_PROTOCOL) (
	WAV_FILE_PROTOCOL **ThisProtocol
	);

typedef struct _WAV_FILE_PROTOCOL
{
 	OPEN_WAV_FILE 			OpenWavFile;
 	WRITE_IN_WAV_FILE		WriteInWavFile;
 	GET_WAV_PCM_DATA_SIZE	GetWavPCMDataSize;
 	FREE_THIS_PROTOCOL		FreeThisProtocol;
} WAV_FILE_PROTOCOL;

#pragma pack()

