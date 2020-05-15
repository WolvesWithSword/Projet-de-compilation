#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*Struct declaration implicite*/
struct _TypeStruct;
struct _Variable;
struct _ParameterType;
struct _FunctionType;
/*====================Type des variable==============*/

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

typedef struct _Expression
{
	Type type;
	int isId;
	int isAffectable;
	char* nameId;
} ExpressionTransit;

//============================ implicite declaration========================================
void freeTypeStruct(TypeStruct* typeStruct);
void structPrint(TypeStruct* typeStruct);
int compareParameterType(ParameterType* type1,ParameterType* type2);
void freeParameterType(ParameterType* list);
void parameterTypePrint(ParameterType* paramList);
void functionTypePrint(FunctionType* funType);
void freeFunctionType(FunctionType* funType);
int compareFunctionType(FunctionType* typeFun1, FunctionType* typeFun2);
//==============================Type Fonction===============================================

Type* initType(){
	Type* type = malloc(sizeof(Type));
	memset(type,0,sizeof(Type));
	return type;
}

void typeCopy(Type* dest ,Type* source){
	memcpy(dest,source,sizeof(Type));
	}

void freeType(Type* type){
	if(type==NULL){ return;}
	if(type->typeStruct!=NULL){
		freeTypeStruct(type->typeStruct);
	}
	freeFunctionType(type->functionType);
	free(type);
}

void typePrint(Type* type){
	if(type==NULL){
		printf("NONE");
		return;
	}
	if(type->isFunction){
		printf("(");
		functionTypePrint(type->functionType);
		printf(")");
	}	
	else if(type->isUnary){
		switch(type->unaryType){
			case 0 :
				printf("void ");
				break;
			case 1 :
				printf("int ");
				break;
			default : 
				break;
		}
	}
	else{
		printf("Struct %s ",type->typeStruct->name);
	}
	if(type->isPtr) printf("*");
}




/*
compare 2 type 
*/
int compareType(Type* type1, Type* type2){

	if(type1->isFunction){
		return compareFunctionType(type1->functionType, type2->functionType);
	}
	if(type1->isPtr!=type2->isPtr) return 0;
	if(type1->isUnary!=type2->isUnary) return 0;
	if(type1->isFunction!=type2->isFunction) return 0;

	if(type1->isUnary==1){
		if(type1->unaryType != type2->unaryType)
			return 0;
		return 1;
	}
	//une structure peut avoir que une adresse (a verif)
	if(type1->typeStruct != type2->typeStruct) return 0;

	return 1;
	
}

/*
compare (dans une operation) 2 type 
type* ==type* ->1
type ==type ->1
type1* == type2* ->2 (type1!= type2!)
void -> 0 (-4 plus tard)
int == type* ->2
type* == int ->2
*/
int compareTypeForOp(Type* type1, Type* type2){


	if((type1->isPtr == 0) && (type2->isPtr == 0)){
		if((type1->isUnary == 1 && type1->unaryType == VOID_T) || (type2->isUnary == 1 && type2->unaryType == VOID_T)) return 0;//Aucune op possible avec void. 
		return compareType(type1, type2);	
	}
	else if((type1->isPtr == 1) && (type2->isPtr == 1)){ 
		if(compareType(type1,type2)){return 1;}
		else{return 2;}
	}
	else{
		if(type2->isPtr){//On met type2 a la place de type1
			Type* tmp = type1;
			type1 = type2;
			type2 = tmp;
		}
		if(type2->isUnary && type2->unaryType == INT_T){return 2;}
	}
	return 0;
}



//==============================Variable Fonction===========================================

Variable* initVariable(){
	Variable* variable = malloc(sizeof(Variable));
	memset(variable,0,sizeof(Variable));
	return variable;
}

void freeVariable(Variable* variable){
	Variable* current = variable;
	while(current!=NULL){
		Variable* tmp =  current;
		current = current->next;

		//freeType(tmp->type);
		free(tmp);
	}
}

void addVariable(Variable** list,Variable* variable){
	if(*list==NULL){
		*list = variable;
		return;
	}
	Variable* current = *list;
	while(current->next!=NULL){
		current = current->next;
	}
	current->next = variable;
		
}

Variable* getVariable(Variable* list,char* name){
	Variable* current = list;
	while(current!=NULL){
		if(strcmp(name,current->name)==0){
			return current;
		}
		current = current->next;
	}
	return NULL;
}

void variablePrint(Variable* variable){
	typePrint(variable->type);
	printf("%s;\n",variable->name );
}

void variableListPrint(Variable* list){
		Variable* current = list;
		while(current!=NULL){
			variablePrint(current);
			current = current->next;
		}
}

//==============================Fonction Fonction===========================================

Fonction* initFonction(){
	Fonction* fonction = malloc(sizeof(Fonction));
	memset(fonction,0,sizeof(Fonction));
	return fonction;
}

void freeFonction(Fonction* fonction){
	Fonction* current = fonction;
	while(current!=NULL){
		Fonction* tmp =  current;
		current = current->next;

		//freeType(tmp->type);
		freeVariable(tmp->variables);
		free(tmp);
	}
}

void addFonction(Fonction** list,Fonction* fonction){
	fonction->next = *list;
	*list = fonction;
}

Fonction* getFonction(Fonction* list,char* name){
	Fonction* current = list;
	while(current!=NULL){
		if(strcmp(name,current->name)==0){
			return current;
		}
		current = current->next;
	}
	return NULL;
}

void fonctionPrint(Fonction* fonction){
		typePrint(fonction->type);
		printf("%s (\n",fonction->name );
		variableListPrint(fonction->variables);
		printf(");\n");
}

void fonctionListPrint(Fonction* list){
	Fonction* current = list;
	while(current!=NULL){
		fonctionPrint(current);
		current = current->next;
	}
}

//==============================typeStruct Fonction===========================================

TypeStruct* initTypeStruct(){
	TypeStruct* typeStruct = malloc(sizeof(TypeStruct));
	memset(typeStruct,0,sizeof(TypeStruct));
	return typeStruct;
}


void freeTypeStruct(TypeStruct* typeStruct){
	TypeStruct* current = typeStruct;
	while(current!=NULL){
		TypeStruct* tmp = current;
		current=current->next;

		freeVariable(tmp->variables);
		free(tmp);
	}
}

void addTypeStruct(TypeStruct** list,TypeStruct* typeStruct){
	typeStruct->next = *list;
	*list = typeStruct;
}

TypeStruct* getTypeStruct(TypeStruct* list,char* name){
	TypeStruct* current = list;
	while(current!=NULL){
		if(strcmp(name,current->name)==0){
			return current;
		}
		current = current->next;
	}
	return NULL;
}

void structPrint(TypeStruct* typeStruct){
	printf("Struct %s {\n",typeStruct->name);
	variableListPrint(typeStruct->variables);
	printf("}\n");
}

void structListPrint(TypeStruct* list){
	TypeStruct* current = list;
	while(current!=NULL){
		structPrint(current);
		current = current->next;
	}
}

//==============================parameterType list===========================================
ParameterType* initParameterType(){
	ParameterType* parameterType = malloc(sizeof(ParameterType));
	memset(parameterType,0,sizeof(parameterType));
	return parameterType;
}

void freeParameterType(ParameterType* list){
	ParameterType* current = list;
	while(current!=NULL){
		ParameterType* tmp = current;
		current=current->next;

		freeType(tmp->type);
		free(tmp);
	}
}
	

void addParameterType(ParameterType** list,Type* type){
	ParameterType* newParam = initParameterType();
	newParam->type = type;
	if(*list==NULL){
		*list = newParam;
		return;
	}
	ParameterType* current = *list;
	while(current->next!=NULL){
		current = current->next;
	}
	current->next = newParam;
}

int compareParameterType(ParameterType* type1, ParameterType* type2){
	while(type1!=NULL || type2!=NULL){
		if(compareType(type1->type,type2->type)){
			type1 = type1->next;
			type2 = type2->next;
		}
		else return 0;
	}
	if(type1!=NULL && type2==NULL || type1==NULL && type2!=NULL) return 0;
	return 1;
	
}

ParameterType* variableToParameterType(Variable* varList){
	ParameterType* allType = NULL;
	while(varList!=NULL){
		addParameterType(&allType,varList->type);
		varList = varList->next;	
	}
	return allType;
}

void parameterTypePrint(ParameterType* paramList){
	printf("(");
	while(paramList!=NULL){
		typePrint(paramList->type);
		paramList = paramList->next;
		if(paramList!=NULL) printf(",");
	}
	printf(")");

}

//==============================FunctionType Fonction===============================================

FunctionType* initFunctionType(){
	FunctionType* functionType = malloc(sizeof(FunctionType));
	memset(functionType,0,sizeof(FunctionType));
	return functionType;
}

void freeFunctionType(FunctionType* funType){
	freeType(funType->returnType);
	freeParameterType(funType->parameters);
	free(funType);
}

void functionTypePrint(FunctionType* funType){
	parameterTypePrint(funType->parameters);
	printf("=>");
	typePrint(funType->returnType);
}

int compareFunctionType(FunctionType* typeFun1, FunctionType* typeFun2){
	//si c'est pas le meme type de retour
	if(!compareType(typeFun1->returnType,typeFun2->returnType)) return 0;
	//si les parametre n'on pas le meme type
	if(!compareParameterType(typeFun1->parameters,typeFun2->parameters)) return 0;
	return 1;
}	

//==============================LinkedListNode function===========================================
/*
Permet d'initialiser une list
*/
LinkedListNode* initList(){
	LinkedListNode* list = malloc(sizeof(LinkedListNode));
	memset(list,0,sizeof(LinkedListNode));
	return list;
}

/*
Permet de free une list
*/
void freeList(LinkedListNode* listNode){
	if(listNode->fonctionList!=NULL)
		freeFonction(listNode->fonctionList);
	if(listNode->variableList!=NULL)
		freeVariable(listNode->variableList);
	if(listNode->typeStructList!=NULL)
		freeTypeStruct(listNode->typeStructList);
	free(listNode);
}




void printListNode(LinkedListNode* listNode){
	printf("-------- Function --------\n");
	fonctionListPrint(listNode->fonctionList);
	printf("-------- Variables --------\n");
	variableListPrint(listNode->variableList);
	printf("-------- Struct --------\n");
	structListPrint(listNode->typeStructList);	
	return;
}



//==============================Stack function===========================================


Stack* newStack(){
	Stack* stack = malloc(sizeof(Stack));
	memset(stack,0,sizeof(Stack));
	return stack;
}


void addStageToStack(Stack* stack){
	LinkedListNode* newStage= initList();
	newStage->next = stack->top;
	stack->top = newStage;
}


void removeStageToStack(Stack* stack){
	LinkedListNode* deleteStage = stack->top;
	stack->top = deleteStage->next;
	freeList(deleteStage);
}

void printStack(Stack* stack){
	int i = 0;
	LinkedListNode* current = stack->top;
	while(current!=NULL){
		printf("------------------ stage : %d ---------------\n",i );
		printListNode(current);
		printf("------------------end stage -----------------\n");
		current = current->next;
		i++;
	}
	printf("end stack\n");
	return;
}

void addVariableToStack(Stack* stack, Variable* variable){
	 addVariable(&(stack->top->variableList),variable);
}

void addFonctionToStack(Stack* stack, Fonction* fonction){
	addFonction(&(stack->top->fonctionList), fonction);
}

void addTypeStructToStack(Stack* stack, TypeStruct* typeStruct){
	addTypeStruct(&(stack->top->typeStructList),typeStruct);
}

Variable* isInstanciateVariable(Stack* stack, char* name){
	LinkedListNode* current = stack->top;
	//verification dans la fonction courrante si elle existe
	if(current->currentFunction!=NULL){
		Variable* res = getVariable(current->currentFunction->variables,name);
		if(res != NULL){
			return res;
		}
	}

	//verification dans la stack
	while(current!=NULL){
		Variable* res = getVariable(current->variableList,name);
		if(res != NULL){
			return res;
		}
		current = current->next;
	}
	return NULL;
}

Fonction* isDefinedFonction(Stack* stack, char* name){
	LinkedListNode* current = stack->top;
	while(current!=NULL){
		Fonction* res = getFonction(current->fonctionList,name);
		if(res != NULL){
			return res;
		}
		current = current->next;
	}
	return NULL;
}

TypeStruct* isCreatedStruct(Stack* stack, char* name){
	LinkedListNode* current = stack->top;
	while(current!=NULL){
		TypeStruct* res = getTypeStruct(current->typeStructList,name);
		if(res != NULL){
			return res;
		}
		current = current->next;
	}
	return NULL;
}

int isExistingInStageName(Stack* stack, char* name){
	LinkedListNode* current = stack->top;

	Variable* res1 = getVariable(current->variableList, name);
	Fonction* res2 = getFonction(current->fonctionList, name);
	//defini dans la fonction courrante
	Variable* res3 = NULL;
	if(current->currentFunction!=NULL) res3 = getVariable(current->currentFunction->variables,name);

	if(res1 == NULL && res2 == NULL && res3 == NULL) return 0;
	return 1;
}

int isExistingInStageStruct(Stack* stack, TypeStruct* ts){
	TypeStruct* res1 = getTypeStruct(stack->top->typeStructList, ts->name);

	if(res1 == NULL) return 0;
	return 1;
}

int isExistingInStageFunction(Stack* stack, Fonction* fonction){
	//TODO Check les parametres, la predef...
	return isExistingInStageName(stack, fonction->name);
}

Type* getLastDefineType(Stack* stack, char* name){
	LinkedListNode* current = stack->top;

	while(current!=NULL){
		
		//defini dans la fonction courante;
		if(current->currentFunction!=NULL){
			Variable* res1 = getVariable(current->currentFunction->variables,name);
			if(res1!=NULL) return res1->type;	
		}

		//c'est une variable
		Variable* res = getVariable(current->variableList, name);
		if(res!=NULL) return res->type;
	
		//c'est une fonction
		Fonction* res2 = getFonction(current->fonctionList, name);
		if(res2!=NULL) return res2->type;
		current = current->next;
	}
	return NULL;
}

Fonction* getCurrentFonction(Stack* stack){
	LinkedListNode* current = stack->top;
	while(current!=NULL){
		//On cherche la fonction la plus haute dans la table des symbole (fonction courrante);
		Fonction* res = current->currentFunction;
		if(res!=NULL) return res;	
		current = current->next;
	}
	return NULL;
}

int mySizeOf(Type* type){
	if(type->isPtr == 1){return sizeof(void*);}
	else if(type->isUnary == 1){
		if(type->unaryType == INT_T){return sizeof(int);}
		else{return sizeof(void);}
	}
	else if(type->isFunction == 1) {return sizeof(void);}
	else {/* struct */
		Variable* current = type->typeStruct->variables;
		int structSize = 0;
		while(current != NULL){
			structSize += mySizeOf(current->type);
			current = current->next;
		}
		return structSize;
	}
}

int relativeAdress(TypeStruct* ts, char* name){
	Variable* current = ts->variables;
	int relAdd = 0;
	while(strcmp(name,current->name)!=0){
		relAdd += mySizeOf(current->type);
		current = current->next;
	} 
	return relAdd;
}


int test(){
	Stack* stack = newStack();

	Variable* var1 = initVariable();
	var1->name = "tedzqsst";

	Variable* var2= initVariable();
	var2->name = "dssdfdq";

	Variable* var3 = initVariable();
	var3->name = "third";
	
	addStageToStack(stack);
	addVariableToStack(stack,var1);
	addStageToStack(stack);
	addStageToStack(stack);
	addVariableToStack(stack,var2);
	addVariableToStack(stack,var3);
	printStack(stack);

		
	removeStageToStack(stack);
	printStack(stack);
	removeStageToStack(stack);
	printStack(stack);
	

	return 0;

}
