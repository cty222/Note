#include <stdio.h>
#include <stdlib.h>

#define ENABLE  1
#define DISABLE 0
#define PRINT_DEBUG ENABLE

typedef  unsigned char UINT8;
typedef  unsigned int UINT32;

typedef struct
{
	UINT8 Offset;
	UINT8 Length;
	UINT8 NextWord;
	void *Forward;
	void *Backward;
} LZ77_NODE;

//UINT8 FileContent[] = "a5df8alroifdksoaksdlgkerofxksa87543sdasdassdaodrflasdasddas4d8a152asdas876wedsjfuiodsfu5j4mn5gheriogmrsdkfsdsda";
UINT8 FileContent[] = "cabracadabrarrarrad";
const UINT8 SearchBufferSize = 7;
const UINT8 LookAheadBufferSize = 6;

UINT8 *SearchBufferPointer;
UINT8 *LookAheadBufferPointer;
UINT8 *OffsetPointer;
UINT8 OffsetCounter = 0;
LZ77_NODE *FirstNode = NULL; 
LZ77_NODE *CurrentNode = NULL;

UINT8 *EndPoint =  FileContent + sizeof(FileContent) -1;

UINT32 strncmpReturnSameCharCount(UINT8* a, UINT8* b, UINT32 Length)
{
	UINT32 Index;
	for (Index = 0; Index < Length; Index++)
	{
		if (a[Index] != b[Index])
		{
			break;
		}
		else if (a[Index] == '\0')
		{
	    	return 0;
	    }
	}
	return Index;
}

void PrintNodeInfo (LZ77_NODE * Node)
{
	printf ("Node->Length		: %d\n", Node->Length);
	printf ("Node->Offset		: %d\n", Node->Offset);
	printf ("Node->NextWord 	: %c\n", Node->NextWord);	
}

void CleanAllNode()
{
	if (FirstNode != NULL)
	{
		CurrentNode = FirstNode;
		while (CurrentNode->Forward != NULL)
		{
#if PRINT_DEBUG			
			printf("Remove node: ");
			PrintNodeInfo (FirstNode);
#endif
			CurrentNode = (LZ77_NODE *)CurrentNode->Forward;
			CurrentNode->Backward = NULL;

			free(FirstNode);
			FirstNode = CurrentNode;
		}
#if PRINT_DEBUG			
		printf("Remove node: ");
		PrintNodeInfo (CurrentNode);
#endif
		free(CurrentNode);
		CurrentNode = NULL;
		FirstNode = NULL;
	}
}

void EncodeLZ77()
{
	SearchBufferPointer = FileContent;
	LookAheadBufferPointer = SearchBufferPointer + SearchBufferSize;
	OffsetCounter = 0;
	
	while ((LookAheadBufferPointer + LookAheadBufferSize) <= EndPoint)
	{
		OffsetPointer = LookAheadBufferPointer - 1;
		LZ77_NODE * Tmp = (void *)malloc (sizeof(LZ77_NODE));
		Tmp->Offset = 0;
		Tmp->Length = 0;
		Tmp->NextWord = LookAheadBufferPointer[0];
		Tmp->Forward = NULL;
		Tmp->Backward = NULL;
		
		if (FirstNode == NULL)
		{
			FirstNode = Tmp;
			CurrentNode = FirstNode;	
		}
		else
		{
			CurrentNode->Forward = (LZ77_NODE *)Tmp;
			Tmp->Backward = (void *)CurrentNode;
			CurrentNode = Tmp;
		}
		
		int OffsetCounter = 0;
		for (OffsetCounter = 0; OffsetCounter < SearchBufferSize; OffsetCounter++)
		{
#if PRINT_DEBUG
			printf ("%2d. %c ?= %c \n",OffsetCounter, LookAheadBufferPointer[0], *OffsetPointer);
#endif
			if (LookAheadBufferPointer[0] == *OffsetPointer)
			{
				UINT8 Length = strncmpReturnSameCharCount(LookAheadBufferPointer, OffsetPointer, LookAheadBufferSize + OffsetCounter);
				if (Tmp->Length < Length)
				{
					Tmp->Length = Length;
					Tmp->Offset = OffsetCounter + 1;
					if((LookAheadBufferPointer + Length) != EndPoint) 
					{
						Tmp->NextWord = LookAheadBufferPointer[ Length + 1];
					}
					else
					{
						Tmp->NextWord = 0;
					}

				}
			}
			OffsetPointer--;
		}
		
#if PRINT_DEBUG
		PrintNodeInfo (Tmp);
#endif
		
		// =====================================
		printf(" ============  Buffer Move ===============\n");
		SearchBufferPointer += Tmp->Length + 1 ;
		LookAheadBufferPointer = SearchBufferPointer + SearchBufferSize;
		OffsetCounter = 0;
	}
}

int main ()
{


	EncodeLZ77();

	// Clean AllNode
	CleanAllNode();
	
	return 0;
}
