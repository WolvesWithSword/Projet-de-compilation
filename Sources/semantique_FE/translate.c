#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "variable.h"

typedef enum { VOID_PTR_BE, INT_BE, VOID_BE } TypeBE;

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

//###################### Gestion des type ######################

TypeBE typeToBackend(Type* type){
	if(type->isPtr){
		return VOID_PTR_BE;
	}
	if(type->isUnary){
		switch(type->unaryType){
			case VOID_T:
				return VOID_BE;
			case INT_T:
				return INT_BE;
			default : //normalement impossible
				fprintf(stderr,"wrong type");
				return VOID_PTR_BE;
		}
	}
	return VOID_PTR_BE;
}

char* toStringTypeBE(TypeBE type){
	switch(type){
		case VOID_BE: return "void ";
		case VOID_PTR_BE: return "void* ";
		case INT_BE: return "int ";
		default : //normalement impossible
			fprintf(stderr,"wrong type");
			return "void* ";
	}
}

//###################### ToWrite ###############################

ToWrite* initToWrite(){
	ToWrite* var = malloc(sizeof(ToWrite));
	memset(var,0,sizeof(ToWrite));
	return var;
}

Content* initContent(){
	Content* content = malloc(sizeof(Content));
	memset(content,0,sizeof(Content));
	return content;
}

void addToWriteContent(ToWrite* toWrite,Content* content){
	if(toWrite->last==NULL){
		toWrite->first = content;
	}
	else{
		toWrite->last->next = content;
	}
	//au cas ou on ajoute une liste de content
	while(content->next!=NULL){
		content = content->next;
	}
	toWrite->last = content;
}



//###################### TMP VAR ###############################
TmpVar* initTmpVar(){
	TmpVar* var = malloc(sizeof(TmpVar));
	memset(var,0,sizeof(TmpVar));
	return var;
}

TmpVar* getTmpVar(TmpVar* list, TypeBE type){
	TmpVar* current = list;
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
	strcat(newVar->name,"_t");
	char buff[5];
	sprintf(buff,"%d",nb);
	strcat(newVar->name,buff);
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

void makeAvailableTmpVarByName(TmpVar* list,char* name){
	TmpVar* current = list;
	while(current != NULL){
		if(strcmp(current->name,name)==0){
			current->isAvailable = 1;
			return;
		}
		current = current->next;
	}
}

//############################### STACK_BE ##############################

StackBE* newStackBE(){
	StackBE* stack = malloc(sizeof(StackBE));
	memset(stack,0,sizeof(StackBE));
	return stack;
}

NodeBE* initNodeBE(){
	NodeBE* node = malloc(sizeof(NodeBE));
	memset(node,0,sizeof(NodeBE));
	node->declaration = initToWrite();
	node->toWrite = initToWrite();
	return node;
}



void addStageToStackBE(StackBE* stack){
	NodeBE* newStage= initNodeBE();
	newStage->next = stack->top;
	stack->top = newStage;
}

void removeStageToStackBE(StackBE* stack){
	//NodeBE* deleteStage = stack->top;
	//stack->top = deleteStage->next;
	//TODO affcete au towrite du nouveau top toutes les declaration
	//puis ajouter le towrite des expression
}

TmpVar* getTmpVarStackBE(StackBE* stack,TypeBE type){
	TmpVar* tmpVar = getTmpVar(stack->top->tmpVarList,type);
	if(tmpVar==NULL){
		int n = tmpLen(stack->top->tmpVarList);
		tmpVar = createTmpVar(n,type);
		addTmpVar(&stack->top->tmpVarList,tmpVar);
	}
	tmpVar->isAvailable = 0;
	return tmpVar;
}

void addToWriteStackBEWithMalloc(StackBE* stack,char* write){
	int n = strlen(write)+2;
	char* str = malloc(sizeof(char)*n);
	strcpy(str,write);
	Content* elt = initContent();
	elt->data = str;
	addToWriteContent(stack->top->toWrite,elt);
}

void addToWriteStackBE(StackBE* stack,char* write){
	Content* elt = initContent();
	elt->data = write;
	addToWriteContent(stack->top->toWrite,elt);
	
}

void addDeclarationStackBE(StackBE* stack,char* write){
	Content* elt = initContent();
	elt->data = write;
	addToWriteContent(stack->top->declaration,elt);
	
}

char* concatenateForOperation(char* exp1,char* op,char* exp2,short endLine){
	int len = strlen(exp1)+strlen(op)+strlen(exp2)+4;
	char* res = malloc(sizeof(char)*len);
	strcpy(res,exp1);
	strcat(res,op);
	strcat(res,exp2);
	if(endLine){
		strcat(res,";\n");
	}
	return res;
}

void* printBackend(ToWrite* write){
	Content* current = write->first;
	while(current!=NULL){
		fprintf(stdout,"%s",current->data);
		current = current->next;
	}
	
}






	
	


