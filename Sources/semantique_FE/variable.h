#ifndef VARIABLE_H
#define VARIABLE_H

typedef enum { VOID_T, INT_T } Type;

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
	int isPointer;

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

Stack* stack;
Fonction* initFonction();
Variable* initVariable();
Node* initNode();
void freeNode(Node* node);

LinkedListNode* initList();
void freeList(LinkedListNode* listNode);

Node* getNode(LinkedListNode* listNode, char* name);
void addNode(LinkedListNode* listNode, Node* var);
void printListNode(LinkedListNode* listNode);
Stack* newStack();
Node* getNodeStack(Stack* stack, char* name);
int isAlreadyDefine(Stack* stack,Node* var);
void addNodeStack(Stack* stack,Node* var);
void addStageToStack(Stack* stack);
void removeStageToStack(Stack* stack);
void printStack(Stack* stack);
int test();



#endif
