#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { VOID_PTR_B, INT_B } BackEndType;

typedef struct _DataStorage
{
	TmpType type; 
	char* nameChar;

	struct _DeclarationStorage* next; 
}DataStorage    

typedef struct _TmpVar
{
	TmpType type; 
	char name[10];
	int isAvailable;	
	
	struct _TmpVar* next;
} TmpVar

//###################### TMP VAR ###############################"
TmpVar* initTmpVar(){
	TmpVar* var = malloc(sizeof(TmpVar));
	memset(var,0,sizeof(TmpVar));
	return var;
}

TmpVar* getTmpVar(TmpVar** list, TmpType type){
	TmpVar* current = *list;
	while(current != NULL){
		if(current->type == type && current->isAvailable == 1){
			return current;
		}	
		current = current->next;
	}
	return createTmpVar(list, type);
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

TmpVar* createTmpVar(TmpVar** list, TmpType type){
	int i = 1;
	TmpVar* current = *list;
	while(current != NULL){
		i++;
		current = current->next;
	}
	TmpVar* newVar = initTmpVar();
	newVar->name = "_t";
	strcat(newVar->name,i);
	newVar->type = type;
	newVar->isAvailable = 1;

	addTmpVar(list, newVar);
	return newVar;
}

void makeAvailableTmpVar(TmpVar* list){
	TmpVar* current = list;
	while(current != NULL){
		current->isAvailable = 1;
		current = current->next;
	}
}
