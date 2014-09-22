#include "stdio.h"
#include "stdlib.h"
#include "GlobalDefine.h"

STATUS 
openHtml (
	IN char *FileName,
	OUT void **dpPageData,
	OUT UINT32 *pDataLength 
)
{
	STATUS Status;
	Status = SUCCESS;

	FILE *TargetPage;

	TargetPage = fopen (FileName,"r");

	fseek(TargetPage, 0, SEEK_END);

	*pDataLength = ftell(TargetPage);

	fseek(TargetPage, 0, SEEK_SET);

	*dpPageData = malloc (*pDataLength);

	if (fread (*dpPageData, sizeof(CHAR8), *pDataLength, TargetPage) == 0){
		Status = ERR_READ_FAILDED;
	}

	return Status;
}
