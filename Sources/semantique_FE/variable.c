#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef enum { VOID, INT } Type;



/*
Represente la declaration d'une variable
*/
typedef struct _Variable
{
	int value;
} Variable; 

/*
Represente la declaration d'une fonction
*/
typedef struct _Fonction
{
	Variable *listVar;
} Fonction; 

/*
Represente un noeud de la liste chainée
*/
typedef struct _Node
{
	int isFonction;

	Fonction *fonction;
	Variable *var;

	Type type;
	char *name;

	struct _Node* next;
} Node; 

/*
Represente la list chainée + un noeud du block de la pile
*/
typedef struct _LinkedListNode
{
	Node* first;
	struct _LinkedListNode* next;
} LinkedListNode; 

/*
Represente la pile qui contient la liste des block
*/
typedef struct _Stack
{
	LinkedListNode* top;
} Stack; 

//==============================Fonction function===========================================

Fonction* initFonction(){
	Fonction* fonction = malloc(sizeof(Fonction));
	memset(fonction,0,sizeof(Fonction));
	return fonction;
}


//==============================Variable function===========================================

Variable* initVariable(){
	Variable* variable = malloc(sizeof(Variable));
	memset(variable,0,sizeof(Variable));
	return variable;
}
//==============================Node function===========================================

Node* initNode(){
	Node* node = malloc(sizeof(Node));
	memset(node,0,sizeof(Node));
	return node;
}

void freeNode(Node* node){
	if(node->isFonction && node->fonction!=NULL){
		free(node->fonction);
	}
	else if(node->var!=NULL){
		free(node->var);
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
	Node* current = listNode->first;
	while(current!=NULL){
		Node* tmp = current;
		current = current->next;
		freeNode(tmp);
	}
	free(listNode);
		
}

/*
Permet de recuperer un element de la list (null si il existe pas)
*/
Node* getNode(LinkedListNode* listNode, char* name){
	Node* current = listNode->first;
	while(current!=NULL){
		if( strcmp( current->name, name) == 0)
			return current;
		current = current->next;
	}
	return NULL;
}


void addNode(LinkedListNode* listNode, Node* var){
	var->next = listNode->first;
	listNode->first = var;
	return;
}

void printListNode(LinkedListNode* listNode){
	Node* current = listNode->first;
	while(current!=NULL){
		printf("%s\n",current->name );
		current = current->next;
	}
	printf("end list\n");
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