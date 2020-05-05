#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*====================Type des variable==============*/

typedef enum { VOID_T, INT_T } UnaryType;

typedef struct _Type
{
	int isUnary;
	TypeUnaire unaryType;
	TypeStruct structType;
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
	Variable* variables; //list des variable dans la structure

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
	TypeStruct* structTypeList;

	struct _LinkedListNode* next;
} LinkedListNode; 


/*
Represente la pile qui contient la liste des block
*/
typedef struct _Stack
{
	LinkedListNode* top;

} Stack; 

//==============================Type Fonction===============================================

Type* initType(){
	Type* type = malloc(sizeof(Type));
	memset(type,0,sizeof(Type));
	return type;
}

void freeType(Type* type){
	if(type->structType!=NULL){
		freeTypeStruct(structType);
	}
	free(type);

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

		freeType(tmp->type);
		free(tmp);
	}
}

void addVariable(Variable* list,Variable* variable){
	variable->next = list;
	list = variable;
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

voiid freeFonction(Fonction* fonction){
	Fonction* current = variable;
	while(current!=NULL){
		Fonction* tmp =  current;
		current = current->next;

		freeType(tmp->type);
		freeVariable(tmp->variables);
		free(tmp);
	}
}

void addFonction(Fonction* list,Fonction* fonction){
	fonction->next = list;
	list = fonction;
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

void addTypeStruct(TypeStruct* list,TypeStruct* typeStruct){
	typeStruct->next = list;
	list = typeStruct;
}

TypeStruct* getFonction(TypeStruct* list,char* name){
	TypeStruct* current = list;
	while(current!=NULL){
		if(strcmp(name,current->name)==0){
			return current;
		}
		current = current->next;
	}
	return NULL;
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
	Node* current = listNode->first;
	while(current!=NULL){
		Node* tmp = current;
		current = current->next;
		freeNode(tmp);
	}
	free(listNode);
		
}




void printListNode(LinkedListNode* listNode){
	Node* current = listNode->first;
	while(current!=NULL){
		printf("%d",current->type);
		if(current->isPointer) {
			printf("*");
		}
		printf(" %s",current->name );
		
		if(current->isFonction) {
			printf("()");
		}
		printf("\n");
		current = current->next;
	}
	printf("end stage\n");
	return;
}



//==============================Stack function===========================================


Stack* newStack(){
	Stack* stack = malloc(sizeof(Stack));
	memset(stack,0,sizeof(stack));
	return stack;
}


Node* getNodeStack(Stack* stack, char* name){
	LinkedListNode* currentList = stack->top;

	while(currentList!=NULL){
		Node* find = getNode(currentList,name);
		if(find!=NULL)
			return find;
		currentList = currentList->next;
	}
	return NULL;
}

int isAlreadyDefine(Stack* stack,Node* var){
	Node* find = getNode(stack->top,var->name);
	if(find==NULL)
		return 0;
	return 1;
}

void addNodeStack(Stack* stack,Node* var){
	if(stack->top==NULL){
		fprintf(stderr,"stack not initialized\n");
		return;
	}
	addNode(stack->top,var);
	return;
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
		printf("stage : %d ------------\n",i );
		printListNode(current);
		current = current->next;
		i++;
	}
	printf("end stack\n");
	return;
}



int test(){
	Stack* stack = newStack();

	Node* var1 = initNode();
	var1->name = "tedzqsst";

	Node* var2 = initNode();
	var2->name = "dssdfdq";

	Node* var3 = initNode();
	var3->name = "third";

	
	addStageToStack(stack);
	addNodeStack(stack,var1);
	addStageToStack(stack);
	addStageToStack(stack);
	addNodeStack(stack,var2);
	addNodeStack(stack,var3);
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