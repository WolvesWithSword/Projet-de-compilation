#ifndef VARIABLE_H
#define VARIABLE_H

/*Struct declaration implicite*/
struct _TypeStruct;
struct _Variable;
struct _ParameterType;
struct _FunctionType;
/*====================Type des variable==============*/

typedef enum { VOID_T, INT_T } UnaryType;
typedef enum { VOID_PTR_BE, INT_BE, VOID_BE } TypeBE;

typedef struct _Type
{
	int isUnary;
	int isPtr;
	int isFunction;
	struct _FunctionType* functionType;
	UnaryType unaryType;
	struct _TypeStruct* typeStruct;
} Type;

/*
type d'une structure
type1 variable1; -> node avec isFonction a 0;
type2 variable2;
type3 variable3;

struct abc a;
a->x => x doit etre dans struct abc 
*/


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


/*
Represente la declaration d'une variable
*/
typedef struct _Variable
{
	Type* type;
	char* name;

	struct _Variable *next;
} Variable; 


/*
Represente la declaration d'une fonction
*/
typedef struct _Fonction
{
	Type* type;
	char *name;

	Variable *variables; //list de variable
	struct _Fonction *next;
} Fonction; 


/*
Represente la list chainée + un noeud du block de la pile
*/
typedef struct _LinkedListNode
{
	Fonction* fonctionList; 
	Variable* variableList;
	TypeStruct* typeStructList;
	Fonction* currentFunction;

	struct _LinkedListNode* next;
} LinkedListNode; 


/*
Represente la pile qui contient la liste des block
*/
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

typedef struct _TmpVar
{
	TypeBE type; 
	char name[10];
	int isAvailable;	
	
	struct _TmpVar* next;
} TmpVar;

typedef struct _Content
{
	char* data;
	int size; 

	short tabulation;

	struct _Content* next; 
} Content; 



typedef struct _ToWrite
{
	Content* first;
	Content* last;

} ToWrite; 


typedef struct _BackendTransit{
	int hasOp;
	Content* expression;
	int isTmpVar;
	TmpVar* tmpVar;

	ToWrite toWrite;
} BackendTransit;


typedef struct _Expression
{
	Type type;
	int isId;
	int isAffectable;
	char* nameId;
	
	BackendTransit backend;
	
} ExpressionTransit;



typedef struct _Label
{
	int numElse;
	int numIf;
	int numContinue;
	int numWhile;
	int numBody;
	int numTest;
	int numFor;

} Label;




typedef struct _NodeBE
{
	TmpVar* tmpVarList;
	ToWrite* declaration; 
	ToWrite* toWrite;
	short stageNb;
	struct _NodeBE* next;
} NodeBE; 

typedef struct _StackBE
{
	NodeBE* top;
	Label label;

	int hasOr;
	int hasAnd;
	int hasEq;
	int hasNoEq;
	int hasSup;
	int hasSupEq;
	int hasInf;
	int hasInfEq;
} StackBE;

typedef struct _TransitParameter{
	ParameterType* parameters;
	Content* content;

	ToWrite toWrite;
} TransitParameter;


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

//################################ back end #######################
StackBE* stackBE;

//###################### Gestion des type ######################

TypeBE typeToBackend(Type* type);
char* toStringTypeBE(TypeBE type);
void freeContent(Content* content);
void addToWriteContent(ToWrite* toWrite,Content* content);
//###################### ToWrite ###############################

ToWrite* initToWrite();
Content* initContent();
void addToWriteToWrite(ToWrite* toWrite,ToWrite* toWrite2);
void addTabulationToWrite(ToWrite* toWrite,int number);
//###################### TMP VAR ###############################
TmpVar* initTmpVar();
TmpVar* getTmpVar(TmpVar** list, TypeBE type);
void addTmpVar(TmpVar** list,TmpVar* tmpVar);
int tmpLen(TmpVar* list);
TmpVar* createTmpVar(int nb, TypeBE type);
void makeAvailableTmpVar(TmpVar* list);
void makeAvailableTmpVarByName(TmpVar* list,char* name);
Content* tmpToContent(TmpVar* tmpVar);

//############################### STACK_BE ##############################

StackBE* newStackBE();
NodeBE* initNodeBE();
void addStageToStackBE(StackBE* stack);
ToWrite removeStageToStackBE(StackBE* stack);
TmpVar* getTmpVarStackBE(StackBE* stack,TypeBE type);
void addToWriteStackBE(StackBE* stack,ToWrite* toWrite);
void addDeclarationStackBE(StackBE* stack,Content* content);

void* printBackend(StackBE* write);

void dynamiqueAlloc(Content* content);
void concatContent(Content* content, char* toAdd);
void concatBeforeContent(Content* content, char* toAdd);
void copyContent(Content* content, char* toCopy);
void affectToTmp(StackBE* stack, BackendTransit* bt, TypeBE type);
void operationTraitement(StackBE* stack, BackendTransit* left, TypeBE leftType, BackendTransit* right, TypeBE rightType, char* op);
Content* variableDeclarationToBE(Variable* variable);
Content* fonctionDeclarationToBE(Fonction* fonction);


//###############################else if ########################################"
ToWrite createIfBackend(StackBE* stack, BackendTransit* cnd,TypeBE cndType, ToWrite* corps, char* ifLabel, char* elseLabel);
ToWrite createIfElseBackend(StackBE* stack, BackendTransit* cnd,TypeBE cndType, ToWrite* ifCorps,ToWrite* elseCorps, char* ifLabel, char* elseLabel,char* continueLabel);
ToWrite createWhileBackend(StackBE* stack, BackendTransit* cnd,TypeBE cndType, ToWrite* corps, char* whileLabel,char* bodyLabel, char* continueLabel);
ToWrite createForBackend(StackBE* stack,ToWrite* init, BackendTransit* cnd,TypeBE cndType, ToWrite* incrmt, ToWrite* corps, char* forLabel, char* testLabel);
void callBackendFonction(StackBE* stack, BackendTransit* left, TypeBE leftType, BackendTransit* right, TypeBE rightType,char* nameFonction, int* op);
char* generateIfLabel(StackBE* stack);
char* generateElseLabel(StackBE* stack);
char* generateWhileLabel(StackBE* stack);
char* generateContinueLabel(StackBE* stack);
char* generateBodyLabel(StackBE* stack);
char* generateTestLabel(StackBE* stack);
char* generateForLabel(StackBE* stack);

Content* andFun();
Content* orFun();
Content* comparaisonFonction(char* name,char* cnd);

#endif
