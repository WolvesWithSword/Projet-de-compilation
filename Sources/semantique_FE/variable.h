#ifndef VARIABLE_H
#define VARIABLE_H

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

Stack* stack;

void freeVar(Variable* var);
LinkedListVariable* initList();
void freeList(LinkedListVariable* listVariable);
Variable* getVariable(LinkedListVariable* listVariable, char* name);
void addVariable(LinkedListVariable* listVariable, Variable* var);
void printListVariable(LinkedListVariable* listVariable);
Stack* newStack();
Variable* getVariableStack(Stack* stack, char* name);
int isAlreadyDefine(Stack* stack,Variable* var);
void addVariableStack(Stack* stack,Variable* var);
void addStageToStack(Stack* stack);
void removeStageToStack(Stack* stack);
void printStack(Stack* stack);
int test();



#endif
