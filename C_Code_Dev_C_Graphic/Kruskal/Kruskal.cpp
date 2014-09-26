#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG_OPEN  1
#define DEBUG_CLOSE 0

#define GET_ARRAY_COUNT(Array) (sizeof(Array) / sizeof(Array[0])) 

#pragma pack(1)
typedef struct _NODE NODE;
typedef struct _NODE_DISTANCE NODE_DISTANCE;

struct _NODE_DISTANCE
{
	NODE *Node;
	int   Distance;	
};

struct _NODE
{
	int NodeID;
	NODE_DISTANCE *PtrList;
	const char * NodeName;
	int LinkCount;
};

typedef struct _PATH
{
	NODE *Node[2];
	int Distance;
} PATH;

typedef enum 
{
	NODE_ID_A = 1,
	NODE_ID_B,
	NODE_ID_C,
	NODE_ID_D,
	NODE_ID_E,
	NODE_ID_F
} NODE_ID;

#pragma pack()

// 放多個變數的方法  __VA_ARGS__
#define INIT_NODE(X_NODE_ID, X_NODE, ...) \
		do{\
		NODE_DISTANCE TMP[] = {__VA_ARGS__}; \
		X_NODE.NodeID = X_NODE_ID;\
		X_NODE.PtrList = (NODE_DISTANCE*) malloc (sizeof(TMP));\
		X_NODE.NodeName = #X_NODE; \
		X_NODE.LinkCount = GET_ARRAY_COUNT(TMP);\
		memcpy ((void *)X_NODE.PtrList ,(void *)TMP, sizeof(TMP));\
		}while(0)
		
#define	FREE_NODE(X_NODE) \
		do{	free(X_NODE.PtrList);\
		X_NODE.PtrList = NULL;\
		}while(0)
	
// Test Data
NODE NodeA, NodeB, NodeC, NodeD, NodeE, NodeF;
							
void InitializeTestData()
{
	INIT_NODE(NODE_ID_A, NodeA , {&NodeC, 3}, {&NodeE, 1});
	INIT_NODE(NODE_ID_B, NodeB , {&NodeD, 3}, {&NodeE, 1}, {&NodeF, 8});
	INIT_NODE(NODE_ID_C, NodeC , {&NodeA, 3}, {&NodeD, 4}, {&NodeF, 5});	
	INIT_NODE(NODE_ID_D, NodeD , {&NodeB, 3}, {&NodeC, 4}, {&NodeE, 2}, {&NodeF, 6});
	INIT_NODE(NODE_ID_E, NodeE , {&NodeA, 1}, {&NodeB, 1}, {&NodeD, 2});
	INIT_NODE(NODE_ID_F, NodeF , {&NodeB, 8}, {&NodeC, 5}, {&NodeD, 6});
}

void FreeTestData()
{
	FREE_NODE(NodeA);
	FREE_NODE(NodeB);
	FREE_NODE(NodeC);
	FREE_NODE(NodeD);
	FREE_NODE(NodeE);
	FREE_NODE(NodeF);
}

bool IsChoosed(PATH * CheckPath, PATH *PathBuf, int BufCount)
{
	int Counter = BufCount;
	while(Counter--)
	{
		if (PathBuf[Counter].Node[0]==0 || PathBuf[Counter].Node[1]==0)
		{
			continue;
		}
		if (PathBuf[Counter].Node[0]->NodeID == CheckPath->Node[0]->NodeID 
			||PathBuf[Counter].Node[0]->NodeID == CheckPath->Node[1]->NodeID )
		{
			if (PathBuf[Counter].Node[1]->NodeID == CheckPath->Node[0]->NodeID 
				||PathBuf[Counter].Node[1]->NodeID == CheckPath->Node[1]->NodeID )
			{
				return true;
			}
		}
	}
	return false;
}

bool IsReturnToRoot(NODE * NextNode, NODE * RootNode, PATH *PathBuf, int BufCount, PATH * LastPath)
{
	int Counter = BufCount;
	while(Counter--)
	{
		if (PathBuf[Counter].Node[0]==0 || PathBuf[Counter].Node[1]==0 || &PathBuf[Counter] == LastPath)
		{
			continue;
		}

		if (PathBuf[Counter].Node[0]->NodeID == NextNode->NodeID )
		{
			if (PathBuf[Counter].Node[1]->NodeID == RootNode->NodeID )
			{
				return true;
			}
			else
			{
				if(IsReturnToRoot(PathBuf[Counter].Node[1], RootNode, PathBuf, BufCount, &PathBuf[Counter]))
				{
					return true;
				}
			}
		}
		
		if (PathBuf[Counter].Node[1]->NodeID == NextNode->NodeID )
		{
			if (PathBuf[Counter].Node[0]->NodeID == RootNode->NodeID )
			{
				return true;
			}
			else
			{
				if(IsReturnToRoot(PathBuf[Counter].Node[0], RootNode, PathBuf, BufCount, &PathBuf[Counter]))
				{
					return true;
				}
			}
		}	
	}
	return false;
}


int main(void)
{
	// Initailize testing data
	InitializeTestData ();	
	
	// Add all test data in an array
	NODE NodeArray[] = {NodeA, NodeB, NodeC, NodeD, NodeE, NodeF};
	
	//The path array count  will just have  NodeArray count minus 1.
	// Initialize Path Array.
	int PathArrayCount = GET_ARRAY_COUNT(NodeArray) -1;
	PATH ChoosedPath[PathArrayCount];
	memset (ChoosedPath, 0, sizeof(ChoosedPath));
	
	while(PathArrayCount--)
	{
		int Row = 0, Column = 0;
		
		// Initailize pointer
		PATH PtrPath;
		//PtrPath = &ChoosedPath[PathArrayCount];
		PtrPath.Distance = -1; //for checking
		
		// Double for loop for Array
		for(Row =0; Row< GET_ARRAY_COUNT(NodeArray); Row++)
		{
			NODE *Ptr = &NodeArray[Row];
			for (Column = 0; Column < Ptr->LinkCount; Column++)
			{
				// 現在的值 
				PATH TmpPath;
				TmpPath.Distance = Ptr->PtrList[Column].Distance;
				
				// 兩個Node Name 
				TmpPath.Node[0] = Ptr;
				TmpPath.Node[1] = Ptr->PtrList[Column].Node;
#if DEBUG_CLOSE				
				printf("TMP %s and %s: %d\n", TmpPath.Node[0].NodeName, TmpPath.Node[1].NodeName, TmpPath.Distance);
#endif				
				// Checking
				if (PtrPath.Distance == -1 || PtrPath.Distance> TmpPath.Distance)
				{
					if (!IsChoosed(&TmpPath, ChoosedPath, GET_ARRAY_COUNT(ChoosedPath) )
						 && !IsReturnToRoot(TmpPath.Node[1], TmpPath.Node[0], ChoosedPath, GET_ARRAY_COUNT(ChoosedPath), NULL)
						 )
				 	{
				 		PtrPath = TmpPath;
					}
				}
			}
		} 
		
		if (PtrPath.Distance != -1)
		{
			memcpy (&ChoosedPath[PathArrayCount], &PtrPath, sizeof (PATH));
			printf("===== Ans %s and %s : %d ========= \n", PtrPath.Node[0]->NodeName, PtrPath.Node[1]->NodeName, PtrPath.Distance);
		} 
		else
		{
			// Finish or error.
			break;
		}
	}
	
	FreeTestData();
	return 0;
}
