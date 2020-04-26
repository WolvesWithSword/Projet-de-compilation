#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _variable
{
	int value;
	char *name;
	struct _variable* next;
} Variable; 

typedef struct _LinkedListVariable
{
	Variable* first;
	struct _LinkedListVariable* next;
} LinkedListVariable; 

typedef struct _Stack
{
	LinkedListVariable* top;
} Stack; 

LinkedListVariable* initList(){
	LinkedListVariable* list = malloc(sizeof(LinkedListVariable));
	memset(list,0,sizeof(LinkedListVariable));
	return list;
}

void freeList(LinkedListVariable* listVariable){
	Variable* current = listVariable->first;
	while(current!=NULL){
		Variable* tmp = current;
		current = current->next;
		free(tmp);
	}
	free(listVariable);
		
}


Variable* getVariable(LinkedListVariable* listVariable, char* name){
	Variable* current = listVariable->first;
	while(current!=NULL){
		if( strcmp( current->name, name) == 0)
			return current;
		current = current->next;
	}
	return NULL;
}


void addVariable(LinkedListVariable* listVariable, Variable* var){
	var->next = listVariable->first;
	listVariable->first = var;
	return;
}

void printListVariable(LinkedListVariable* listVariable){
	Variable* current = listVariable->first;
	while(current!=NULL){
		printf("%s\n",current->name );
		current = current->next;
	}
	printf("end list\n");
	return;
}



Stack* newStack(){
	Stack* stack = malloc(sizeof(Stack));
	memset(stack,0,sizeof(stack));
	return stack;
}


Variable* getVariableStack(Stack* stack, char* name){
	LinkedListVariable* currentList = stack->top;

	while(currentList!=NULL){
		Variable* find = getVariable(currentList,name);
		if(find!=NULL)
			return find;
		currentList = currentList->next;
	}
	return NULL;
}

int isAlreadyDefine(Stack* stack,Variable* var){
	Variable* find = getVariable(stack->top,var->name);
	if(find==NULL)
		return 0;
	return 1;
}

void addVariableStack(Stack* stack,Variable* var){
	if(stack->top==NULL){
		fprintf(stderr,"stack not initialized\n");
		return;
	}
	addVariable(stack->top,var);
	return;
}

void addStageToStack(Stack* stack){
	LinkedListVariable* newStage= initList();
	newStage->next = stack->top;
	stack->top = newStage;
}


void removeStageToStack(Stack* stack){
	LinkedListVariable* deleteStage = stack->top;
	stack->top = deleteStage->next;
	freeList(deleteStage);
}

void printStack(Stack* stack){
	int i = 0;
	LinkedListVariable* current = stack->top;
	while(current!=NULL){
		printf("stage : %d ------------\n",i );
		printListVariable(current);
		current = current->next;
		i++;
	}
	printf("end stack\n");
	return;
}


int test(){
	Stack* stack = newStack();

	Variable* var1 = malloc(sizeof(Variable));
	memset(var1,0,sizeof(Variable));
	var1->name = "tedzqsst";

	Variable* var2 = malloc(sizeof(Variable));
	memset(var2,0,sizeof(Variable));
	var2->name = "dssdfdq";

	Variable* var3 = malloc(sizeof(Variable));
	memset(var3,0,sizeof(Variable));
	var3->name = "third";

	
	addStageToStack(stack);
	addVariableStack(stack,var1);
	addStageToStack(stack);
	addStageToStack(stack);
	addVariableStack(stack,var2);
	addVariableStack(stack,var3);
	printf("%d\n", isAlreadyDefine(stack,var1));
	printf("%d\n", isAlreadyDefine(stack,var2));
	printf("%d\n", isAlreadyDefine(stack,var3));

	printStack(stack);

	removeStageToStack(stack);
	printStack(stack);
	printf("%d\n", isAlreadyDefine(stack,var1));
	printf("%d\n", isAlreadyDefine(stack,var2));
	printf("%d\n", isAlreadyDefine(stack,var3));

	removeStageToStack(stack);
	printStack(stack);
	printf("%d\n", isAlreadyDefine(stack,var1));
	printf("%d\n", isAlreadyDefine(stack,var2));
	printf("%d\n", isAlreadyDefine(stack,var3));

	return 0;

}