#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*Struct declaration implicite*/
struct _TypeStruct;
struct _Variable;
/*====================Type des variable==============*/

typedef enum { VOID_T, INT_T } UnaryType;

typedef struct _Type
{
	int isUnary;
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
} Transit;

//============================ implicite declaration========================================
void freeTypeStruct(TypeStruct* typeStruct);
void structPrint(TypeStruct* typeStruct);
//==============================Type Fonction===============================================

Type* initType(){
	Type* type = malloc(sizeof(Type));
	memset(type,0,sizeof(Type));
	return type;
}

void freeType(Type* type){
	if(type==NULL){ return;}
	if(type->typeStruct!=NULL){
		freeTypeStruct(type->typeStruct);
	}
	free(type);
}

void typePrint(Type* type){
	if(type==NULL){
		printf("NONE");
		return;
	}
	if(type->isUnary){
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
		structPrint(type->typeStruct);
	}
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
	variable->next = *list;
	*list = variable;
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
		printf("%s (",fonction->name );
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
	printf("Struct %s {",typeStruct->name);
	variableListPrint(typeStruct->variables);
	printf("};\n");
}

void structListPrint(TypeStruct* list){
	TypeStruct* current = list;
	while(current!=NULL){
		structPrint(current);
		current = current->next;
	}
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
	memset(stack,0,sizeof(stack));
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
