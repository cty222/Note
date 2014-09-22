#include <stdio.h>
#include <stdlib.h>

// link list node
typedef struct _IntNode
{
	int value;
	void *next;
} IntNode;

// 大整數 
typedef struct _largeIntNum
{
	int *largeInt;
	int length;
} largeIntNum;

int prod (largeIntNum*, largeIntNum*, largeIntNum *);

// 建立大整數 用的link list node 
void createNode (IntNode **dPtr)
{
	(*dPtr) = (IntNode *) malloc (sizeof(IntNode));
	(*dPtr)->value = -1;
	(*dPtr)->next = NULL;
}

// 將數字轉成大整數 
void convertLargeInt(unsigned int input, largeIntNum* output)
{
	IntNode *head, *ptr, *NewNode;
	int count;
	
	output->length = 0;
	
	// create first node
	createNode (&ptr);
	head = ptr;
	
	// create number list
	while ( input >= 0 )
	{
		ptr->value = (input % 10);
		createNode (&NewNode);
		ptr->next = NewNode;
		ptr = ptr->next;
		output->length++;
		input /= 10;
		if (input == 0)
		{
			break;
		} 
	}
	
	output->largeInt = malloc (sizeof(int) * output->length);
	count = 0;
	// Move data to output and free link list 
	ptr = head;
	while (ptr->next!=NULL)
	{
		if ((count < output->length))
		{
			output->largeInt[count] = ptr->value;
			count++;
		}
		else
		{
			printf ("Error\n");
		}
		head = ptr->next;
		free (ptr);
		ptr = head;
	}
	free (ptr);
}

// 列印大整數 
void printlnLargeInt(largeIntNum* output)
{
	int i=0;
	for(i=output->length -1 ; i>=0; i--)
	{
		printf("%d",output->largeInt[i]);
	}
	printf("\n");
}

// 初始化大整數 
void initLargeIntNum(largeIntNum *newLargeIntNum) 
{
	newLargeIntNum->largeInt = NULL;
	newLargeIntNum->length = 0;
}


// 大整數乘法 
int prod (largeIntNum* num1, largeIntNum* num2, largeIntNum *output)
{
	int length, divNum;
	largeIntNum *largerNum, *smallerNum;
	int i = 0;
	
    if (num1->length ==0 || num2->length ==0)
	{
		printf ("No Numbers\n");
		return -1;
	}
	
	if (num1->length >= num2->length)
	{
		largerNum = num1;
		smallerNum = num2;
	}
	else
	{
		largerNum = num2;
		smallerNum = num1;
	}
	length = smallerNum->length;
	
	divNum = length/2;
	
	// 如果有一項已經被切到剩個位數, 就要直接相乘, 無法再切 
	if (divNum == 0)
	{
		int outNum=0, tmpNum=0;
		
		output->length = largerNum->length+1;
		output->largeInt = malloc (sizeof(int) * (output->length));
		
		for(i = 0; i<largerNum->length; i++)
		{
			outNum = largerNum->largeInt[i] * smallerNum->largeInt[0] + tmpNum;
			tmpNum = 0; 
			if (outNum >= 10)
			{
				tmpNum = outNum/10;
			} 
			output->largeInt[i] = outNum%10;
		} 
		if (tmpNum != 0)
		{
			output->largeInt[i] = tmpNum;
			tmpNum = 0;
		}
		else
		{
			// 因為進位的關係, 預設會多給一位數
			// 可加可不加, 應該不會影響結果, 顯示可能會有差 
			output->length --;	
		}
	}
	// 如果還可以再切 
	else
	{
		largeIntNum NumX, NumY, NumW, NumZ;
		largeIntNum outPut1, outPut2, outPut3, outPut4;
		largeIntNum totalOutPut, *tmpLargeOutPut, *tmpSmallOutPut; 
		int CarryNum = 0;
		int AddFlag = 0;
		
		// initial
		initLargeIntNum(&NumX);
		initLargeIntNum(&NumY);
		initLargeIntNum(&NumW);
		initLargeIntNum(&NumZ);
		initLargeIntNum(&outPut1);
		initLargeIntNum(&outPut2);
		initLargeIntNum(&outPut3);
		initLargeIntNum(&outPut4);
		initLargeIntNum(&totalOutPut);
		
		// 展開式 
		NumX.largeInt = &num1->largeInt[divNum];
		NumX.length = num1->length - divNum;
		NumY.largeInt = &num1->largeInt[0];
		NumY.length = divNum;
		
		NumW.largeInt = &num2->largeInt[divNum];
		NumW.length = num2->length - divNum;
		NumZ.largeInt = &num2->largeInt[0];
		NumZ.length = divNum;

		//1 最高項 
			prod (&NumX, &NumW, &outPut1);
				
		//2 中間項 
			prod (&NumX, &NumZ, &outPut2);
			prod (&NumW, &NumY, &outPut3);
			
			// 大整數加法 (沒進位) 
			// 但因為可能會進位 所以多加一個flag
			// 在全部加總的時候再加進去 
			if(outPut2.length >= outPut3.length)
			{
				tmpLargeOutPut = &outPut2;
				tmpSmallOutPut = &outPut3;
			}
			else
			{
				tmpLargeOutPut = &outPut3;
				tmpSmallOutPut = &outPut2;
			}

			for (i=0; i< tmpSmallOutPut->length; i++)
			{
				int tmp = tmpLargeOutPut->largeInt[i] + tmpSmallOutPut->largeInt[i] + CarryNum;
				CarryNum = tmp / 10;
				tmpLargeOutPut->largeInt[i] = tmp%10;
			}
			if (CarryNum)
			{
				AddFlag = 1;
				CarryNum = 0;
			}
			
		//3 末項 
			prod (&NumY, &NumZ, &outPut4);
			
		//4 組合
			totalOutPut.length = outPut1.length+divNum*2;
			totalOutPut.largeInt = (int *) malloc(sizeof(int) * (totalOutPut.length));
			for (i=0; i<totalOutPut.length; i++)
			{
				int tmpNum =0;
				
				//
				// 大整數的加法的進位Flag
				// 
				if (AddFlag &  (i == tmpSmallOutPut->length + divNum))
				{ 
					CarryNum ++;
					AddFlag = 0;
				}
				
				if (i<divNum)
				{
					totalOutPut.largeInt[i] = outPut4.largeInt[i];
				}
				else if ((i >= divNum) && (i < (divNum *2)))
				{
					if (i < outPut4.length)
			 		{
			 			tmpNum = outPut4.largeInt[i] + tmpLargeOutPut->largeInt[i-divNum] + CarryNum;
	 					totalOutPut.largeInt[i] = tmpNum%10;
						CarryNum = tmpNum /10;
					}
					else
					{
						tmpNum = tmpLargeOutPut->largeInt[i-divNum];
						totalOutPut.largeInt[i] = tmpNum%10;
						CarryNum = tmpNum /10;
					}
			 	}
			 	else
			 	{
			 		if (i < tmpLargeOutPut->length + divNum)
			 		{

			 			tmpNum = tmpLargeOutPut->largeInt[i-divNum] + outPut1.largeInt[i-divNum*2] + CarryNum;
						totalOutPut.largeInt[i] = tmpNum%10;
						CarryNum = tmpNum /10;

			 		}
					else
					{

						tmpNum = outPut1.largeInt[i-divNum*2] + CarryNum;
						totalOutPut.largeInt[i] = tmpNum%10;
						CarryNum = tmpNum /10;
					}
			 	
			 	}
			}
			if (output->largeInt != NULL)
			{
				free(output->largeInt);
			}
			output->largeInt = totalOutPut.largeInt;
			output->length = totalOutPut.length;
	}
	return 0;
}

int main(int argc, char *argv[]) 
{
	largeIntNum A, B, outPut;
	unsigned int AInput = 3344573;
	unsigned int BInput = 66666;
	
	A.largeInt = NULL;
	A.length = 0;
	B.largeInt = NULL;
	B.length = 0;
	outPut.largeInt = NULL;
	outPut.length = 0;
	
	convertLargeInt (AInput, &A);
	printf ("A: ");
	printlnLargeInt (&A);
	
	
	convertLargeInt (BInput, &B);
	printf ("B: ");
	printlnLargeInt (&B);

	prod(&A, &B, &outPut);
	printf ("outPut: ");
	printlnLargeInt (&outPut);
	
	if (A.largeInt != NULL)
	{
		free(A.largeInt);
	}
	if (B.largeInt != NULL)
	{
		free(B.largeInt);
	}
	if (outPut.largeInt != NULL)
	{
		free(outPut.largeInt);
	}
	return 0;
}
