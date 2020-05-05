#ifndef VARIABLE_H
#define VARIABLE_H

typedef enum { VOID_T, INT_T } UnaryType;

typedef struct _Type
{
	int isUnary;
	TypeUnaire unaryType;
	TypeStruct typeStruct;
} Type;

typedef struct _TypeStruct
{
	char* name; //nom de la structure
	Variable* variables; //list des variable dans la structure

	struct _TypeStruct *next;
} TypeStruct;


typedef struct _Variable
{
	Type* type;
	char* name;

	struct _Variable *next;
} Variable; 


typedef struct _Fonction
{
	Type* type;
	char *name;

	Variable *variables; //list de variable
	struct _Fonction *next;
} Fonction; 

typedef struct _LinkedListNode
{
	Fonction* fonctionList; 
	Variable* variableList;
	TypeStruct* typeStructList;

	struct _LinkedListNode* next;
} LinkedListNode; 


typedef struct _Stack
{
	LinkedListNode* top;

} Stack; 

//==============================Type Fonction===========================================

Type* initType();
void freeType(Type* type);
void typePrint(Type* type);

//==============================Variable Fonction===========================================

Variable* initVariable();
void freeVariable(Variable* variable);
void addVariable(Variable** list,Variable* variable);
Variable* getVariable(Variable* list,char* name);
void variablePrint(Variable* variable);
void variableListPrint(Variable* list);

//==============================Fonction Fonction===========================================

Fonction* initFonction();
voiid freeFonction(Fonction* fonction);
void addFonction(Fonction** list,Fonction* fonction);
Fonction* getFonction(Fonction* list,char* name);
void fonctionPrint(Fonction* fonction);
void fonctionListPrint(Fonction* list);

//==============================typeStruct Fonction===========================================

TypeStruct* initTypeStruct();
void freeTypeStruct(TypeStruct* typeStruct);
void addTypeStruct(TypeStruct** list,TypeStruct* typeStruct);
TypeStruct* getFonction(TypeStruct* list,char* name);
void structPrint(TypeStruct* typeStruct);
void structListPrint(TypeStruct* list);

//==============================LinkedListNode function===========================================

LinkedListNode* initList();
void freeList(LinkedListNode* listNode);
void printListNode(LinkedListNode* listNode);

//==============================Stack function===========================================
Stack* stack;


Stack* newStack();
void addStageToStack(Stack* stack);
void removeStageToStack(Stack* stack);
void printStack(Stack* stack);
void addVariableToStack(Stack* stack, Variable* variable);
void addFonctionToStack(Stack* stack, Fonction* fonction);
void addTypeStructToStack(Stack* stack, TypeStruct* typeStruct);

#endif
