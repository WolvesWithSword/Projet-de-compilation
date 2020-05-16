#ifndef TRANSLATE_H
#define TRANSLATE_H


typedef enum { VOID_PTR_BE, INT_BE } TypeBE;

typedef struct _Label
{
	int NumElse;
	int NumIf;

} Label;

typedef struct _Content
{
	char* data;

	struct _Content* next; 
} Content; 

typedef struct _ToWrite
{
	Content* first;
	Content* last;

} ToWrite; 

typedef struct _TmpVar
{
	TypeBE type; 
	char name[10];
	int isAvailable;	
	
	struct _TmpVar* next;
} TmpVar;


typedef struct _NodeBE
{
	TmpVar* tmpVarList;
	ToWrite* declaration; 
	ToWrite* toWrite;

	struct _NodeBE* next;
} NodeBE; 

typedef struct _StackBE
{
	NodeBE* top;
} StackBE;

StackBE* stackBE;
//###################### Gestion des type ######################

TypeBE typeToBackend(Type* type);
char* toStringTypeBE(TypeBE type);

//###################### ToWrite ###############################

ToWrite* initToWrite();


//###################### TMP VAR ###############################
TmpVar* initTmpVar();
TmpVar* getTmpVar(TmpVar** list, TypeBE type);
void addTmpVar(TmpVar** list,TmpVar* tmpVar);
int tmpLen(TmpVar* list);
TmpVar* createTmpVar(int nb, TypeBE type);
void makeAvailableTmpVar(TmpVar* list);
void makeAvailableTmpVarByName(TmpVar* list,char* name);

//############################### STACK_BE ##############################

StackBE* newStackBE();
NodeBE* initNodeBE();
void addStageToStackBE(StackBE* stack);
void removeStageToStackBE(StackBE* stack);
TmpVar* getTmpVarStackBE(StackBE* stack,TypeBE type);
void addToWriteStackBEWithMalloc(StackBE* stack,char* write);

void addToWriteStackBE(StackBE* stack,char* write);
char* concatenateForOperation(char* exp1,char* op,char* exp2,short endLine);

void* printBackend(ToWrite* write);
#endif
