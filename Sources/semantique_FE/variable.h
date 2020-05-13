#ifndef VARIABLE_H
#define VARIABLE_H

/*Struct declaration implicite*/
struct _TypeStruct;
struct _Variable;
struct _ParameterType;
struct _FunctionType;

typedef enum { VOID_T, INT_T } UnaryType;

typedef struct _Type
{
	int isUnary;
	int isPtr;
	int isFunction;
	struct _FunctionType* functionType;
	UnaryType unaryType;
	struct _TypeStruct* typeStruct;
} Type;

typedef struct _TypeStruct
{
	char* name; //nom de la structure
	struct _Variable* variables; //list des variable dans la structure

	struct _TypeStruct *next;
} TypeStruct;

/*
list des type des parametre d'une fonction
*/
typedef struct _ParameterType
{
	Type* type;
	struct _ParameterType *next;
} ParameterType;

/*
Type d'une fonction
*/
typedef struct _FunctionType
{
	Type* returnType;
	ParameterType* parameters;
} FunctionType;


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
	Fonction* currentFunction;

	struct _LinkedListNode* next;
} LinkedListNode; 


typedef struct _Stack
{
	LinkedListNode* top;

} Stack; 

typedef struct _Transit
{
	Variable* variableD;
	Fonction* fonctionD;
	char* name;
	int isPtr;
} Transit;

typedef struct _Expression
{
	Type type;
	int isId;
	int isAffectable;
	char* nameId;
} ExpressionTransit;

//==============================Type Fonction===========================================

Type* initType();
void typeCopy(Type* dest ,Type* source);
void freeType(Type* type);
void typePrint(Type* type);
int compareType(Type* type1, Type* type2);
int compareTypeForOp(Type* type1, Type* type2);

//==============================Variable Fonction===========================================

Variable* initVariable();
void freeVariable(Variable* variable);
void addVariable(Variable** list,Variable* variable);
Variable* getVariable(Variable* list,char* name);
void variablePrint(Variable* variable);
void variableListPrint(Variable* list);

//==============================Fonction Fonction===========================================

Fonction* initFonction();
void freeFonction(Fonction* fonction);
void addFonction(Fonction** list,Fonction* fonction);
Fonction* getFonction(Fonction* list,char* name);
void fonctionPrint(Fonction* fonction);
void fonctionListPrint(Fonction* list);

//==============================typeStruct Fonction===========================================

TypeStruct* initTypeStruct();
void freeTypeStruct(TypeStruct* typeStruct);
void addTypeStruct(TypeStruct** list,TypeStruct* typeStruct);
TypeStruct* getTypeStruct(TypeStruct* list,char* name);
void structPrint(TypeStruct* typeStruct);
void structListPrint(TypeStruct* list);

//==============================parameterType list===========================================
ParameterType* initParameterType();
void freeParameterType(ParameterType* list);
void addParameterType(ParameterType** list,Type* type);
int compareParameterType(ParameterType* type1, ParameterType* type2);
ParameterType* variableToParameterType(Variable* varList);
void parameterTypePrint(ParameterType* paramList);

//==============================FunctionType Fonction===============================================

FunctionType* initFunctionType();
void freeFunctionType(FunctionType* funType);
void functionTypePrint(FunctionType* funType);

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

Variable* isInstanciateVariable(Stack* stack, char* name);
Fonction* isDefinedFonction(Stack* stack, char* name);
TypeStruct* isCreatedStruct(Stack* stack, char* name);

int isExistingInStageName(Stack* stack, char* name);
int isExistingInStageStruct(Stack* stack, TypeStruct* ts);
int isExistingInStageFunction(Stack* stack, Fonction* fonction);
Type* getLastDefineType(Stack* stack, char* name);
Fonction* getCurrentFonction(Stack* stack);

int mySizeOf(Type* type);
int relativeAdress(TypeStruct* ts, char* name);

#endif
