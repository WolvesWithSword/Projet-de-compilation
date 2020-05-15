#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

//###################### TMP VAR ###############################"
TmpVar* initTmpVar(){
	TmpVar* var = malloc(sizeof(TmpVar));
	memset(var,0,sizeof(TmpVar));
	return var;
}

TmpVar* getTmpVar(TmpVar** list, TypeBE type){
	TmpVar* current = *list;
	while(current != NULL){
		if(current->type == type && current->isAvailable == 1){
			return current;
		}	
		current = current->next;
	}
	return NULL;
}

void addTmpVar(TmpVar** list,TmpVar* tmpVar){
	if(*list==NULL){
		*list = tmpVar;
		return;
	}
	TmpVar* current = *list;
	while(current->next!=NULL){
		current = current->next;
	}
	current->next = tmpVar;	
}

int tmpLen(TmpVar* list){

	if(list == NULL) return 0;

	int i = 1;
	TmpVar* current = list;
	while(current != NULL){
		i++;
		current = current->next;
	}
	return i;
}

TmpVar* createTmpVar(int nb, TypeBE type){
	TmpVar* newVar = initTmpVar();
	newVar->name = "_t";
	strcat(newVar->name,nb);
	newVar->type = type;
	newVar->isAvailable = 1;

	return newVar;
}

void makeAvailableTmpVar(TmpVar* list){
	TmpVar* current = list;
	while(current != NULL){
		current->isAvailable = 1;
		current = current->next;
	}
}

//############################### STACK_BE ##############################

Stack* newStackBE(){
	StackBE* stack = malloc(sizeof(StackBE));
	memset(stack,0,sizeof(StackBE));
	return stack;
}

NodeBE* initNodeBE(){
	NodeBE* node = malloc(sizeof(NodeBE));
	memeset(node,0,sizeof(NodeBE));
	return node;
}



void addStageToStackBE(StackBE* stack){
	NodeBE* newStage= initNodeBE();
	newStage->next = stack->top;
	stack->top = newStage;
}

void removeStageToStackBE(StackBE* stack){
	NodeBE* deleteStage = stack->top;
	stack->top = deleteStage->next;
	//TODO : ecrire(tostring) et free tmpVAR
}

