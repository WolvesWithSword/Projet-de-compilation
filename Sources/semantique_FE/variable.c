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

	struct _Content* next; 
} Content; 

typedef struct _BackendTransit{
	int hasOp;
	Content* expression;
	int isTmpVar;
	TmpVar* tmpVar;
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
	int NumElse;
	int NumIf;

} Label;



typedef struct _ToWrite
{
	Content* first;
	Content* last;

} ToWrite; 




typedef struct _NodeBE
{
	TmpVar* tmpVarList;
	ToWrite* declaration; 
	ToWrite* toWrite;

	struct _NodeBE* next;
} NodeBE; 

typedef struct _StackBE
{
	NodeBE* top;
} StackBE;

//============================ implicite declaration========================================
void freeTypeStruct(TypeStruct* typeStruct);
void structPrint(TypeStruct* typeStruct);
int compareParameterType(ParameterType* type1,ParameterType* type2);
void freeParameterType(ParameterType* list);
void parameterTypePrint(ParameterType* paramList);
void functionTypePrint(FunctionType* funType);
void freeFunctionType(FunctionType* funType);
int compareFunctionType(FunctionType* typeFun1, FunctionType* typeFun2);
void concatContent(Content* content, char* toAdd);
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


//###################### Gestion des type ######################

TypeBE typeToBackend(Type* type){
	if(type->isPtr){
		return VOID_PTR_BE;
	}
	if(type->isUnary){
		switch(type->unaryType){
			case VOID_T:
				return VOID_BE;
			case INT_T:
				return INT_BE;
			default : //normalement impossible
				fprintf(stderr,"wrong type");
				return VOID_PTR_BE;
		}
	}
	return VOID_PTR_BE;
}

char* toStringTypeBE(TypeBE type){
	switch(type){
		case VOID_BE: return "void ";
		case VOID_PTR_BE: return "void* ";
		case INT_BE: return "int ";
		default : //normalement impossible
			fprintf(stderr,"wrong type");
			return "void* ";
	}
}

//###################### ToWrite ###############################

ToWrite* initToWrite(){
	ToWrite* var = malloc(sizeof(ToWrite));
	memset(var,0,sizeof(ToWrite));
	return var;
}

Content* initContent(){
	Content* content = malloc(sizeof(Content));
	memset(content,0,sizeof(Content));
	content->data = calloc(10,sizeof(char));
	content->size = 10;
	return content;
}

void freeContent(Content* content){
	if(content == NULL) return;
	free(content->data);
	free(content);
}

void addToWriteContent(ToWrite* toWrite,Content* content){
	if(toWrite->last==NULL){
		toWrite->first = content;
	}
	else{
		toWrite->last->next = content;
	}
	//au cas ou on ajoute une liste de content
	while(content->next!=NULL){
		content = content->next;
	}
	toWrite->last = content;
}

void addToWriteToWrite(ToWrite* toWrite,ToWrite* toWrite2){
	if(toWrite2->first==NULL) return;
	if(toWrite->first==NULL){
		toWrite->first = toWrite2->first;
		toWrite->last = toWrite2->last;
		return;
	}
	else{
		toWrite->last->next = toWrite2->first;
		toWrite->last = toWrite2->last;
	}
}

//###################### TMP VAR ###############################
TmpVar* initTmpVar(){
	TmpVar* var = malloc(sizeof(TmpVar));
	memset(var,0,sizeof(TmpVar));
	return var;
}

TmpVar* getTmpVar(TmpVar* list, TypeBE type){
	TmpVar* current = list;
	while(current != NULL){
		if(current->type == type && current->isAvailable == 1){
			return current;
		}	
		current = current->next;
	}
	return NULL;
}

void addTmpVar(TmpVar** list,TmpVar* tmpVar){
	if(*list==NULL){
		*list = tmpVar;
		return;
	}
	TmpVar* current = *list;
	while(current->next!=NULL){
		current = current->next;
	}
	current->next = tmpVar;	
}

int tmpLen(TmpVar* list){

	if(list == NULL) return 0;

	int i = 0;
	TmpVar* current = list;
	while(current != NULL){
		i++;
		current = current->next;
	}
	return i;
}

TmpVar* createTmpVar(int nb, TypeBE type){
	TmpVar* newVar = initTmpVar();
	strcat(newVar->name,"_t");
	char buff[5];
	sprintf(buff,"%d",nb);
	strcat(newVar->name,buff);
	newVar->type = type;
	newVar->isAvailable = 1;

	return newVar;
}

void makeAvailableTmpVar(TmpVar* list){
	TmpVar* current = list;
	while(current != NULL){
		current->isAvailable = 1;
		current = current->next;
	}
}

void makeAvailableTmpVarByName(TmpVar* list,char* name){
	TmpVar* current = list;
	while(current != NULL){
		if(strcmp(current->name,name)==0){
			current->isAvailable = 1;
			return;
		}
		current = current->next;
	}
}

Content* tmpToContent(TmpVar* tmpVar){
	char* typeString = toStringTypeBE(tmpVar->type);
	Content* content = initContent();
	concatContent(content,typeString);
	concatContent(content,tmpVar->name);
	concatContent(content,";\n");
	return content;
}

//############################### STACK_BE ##############################

StackBE* newStackBE(){
	StackBE* stack = malloc(sizeof(StackBE));
	memset(stack,0,sizeof(StackBE));
	return stack;
}

NodeBE* initNodeBE(){
	NodeBE* node = malloc(sizeof(NodeBE));
	memset(node,0,sizeof(NodeBE));
	node->declaration = initToWrite();
	node->toWrite = initToWrite();
	return node;
}



void addStageToStackBE(StackBE* stack){
	NodeBE* newStage= initNodeBE();
	newStage->next = stack->top;
	stack->top = newStage;
}

void removeStageToStackBE(StackBE* stack){
	NodeBE* deleteStage = stack->top;
	stack->top = deleteStage->next;

	addToWriteToWrite(stack->top->toWrite,deleteStage->declaration);
	
	TmpVar* current = deleteStage->tmpVarList;
	while(current != NULL){
		addToWriteContent(stack->top->toWrite,tmpToContent(current));
		current = current->next;
	}
	addToWriteToWrite(stack->top->toWrite,deleteStage->toWrite);
	//TODO faire les free
	free(deleteStage);
}

TmpVar* getTmpVarStackBE(StackBE* stack,TypeBE type){
	TmpVar* tmpVar = getTmpVar(stack->top->tmpVarList,type);
	if(tmpVar==NULL){
		int n = tmpLen(stack->top->tmpVarList);
		tmpVar = createTmpVar(n,type);
		addTmpVar(&stack->top->tmpVarList,tmpVar);
	}
	tmpVar->isAvailable = 0;
	return tmpVar;
}


void addToWriteStackBE(StackBE* stack,Content* content){
	addToWriteContent(stack->top->toWrite,content);
}

void addDeclarationStackBE(StackBE* stack,Content* content){
	addToWriteContent(stack->top->declaration,content);
	
}

//######################## char* ######################################"

void printBackend(ToWrite* write){
	Content* current = write->first;
	while(current!=NULL){
		fprintf(stdout,"%s",current->data);
		current = current->next;
	}
}

void dynamiqueAlloc(Content* content){
	content->size *= 2;
	content->data = realloc(content->data,content->size);
}

void concatContent(Content* content, char* toAdd){
	while(strlen(content->data) + strlen(toAdd) >= content->size){
		dynamiqueAlloc(content);
	}
	strcat(content->data,toAdd);
}

void concatBeforeContent(Content* content, char* toAdd){
	while(strlen(content->data) + strlen(toAdd) >= content->size){
		dynamiqueAlloc(content);
	}
	char buff[content->size];
	sprintf(buff,"%s",content->data);
	strcpy(content->data,toAdd);
	strcat(content->data,buff);
}

void copyContent(Content* content, char* toCopy){
	while(strlen(content->data) + strlen(toCopy) >= content->size){
		dynamiqueAlloc(content);
	}
	strcpy(content->data,toCopy);
}


void affectToTmp(StackBE* stack, BackendTransit* bt, TypeBE type){
	if(bt->hasOp == 1){
		TmpVar* tmpVar = getTmpVarStackBE(stack,type);
		Content* content = initContent();

		concatContent(content,tmpVar->name);
		concatContent(content," = ");
		concatContent(content, bt->expression->data);
		concatContent(content,";\n");

		addToWriteStackBE(stack,content);
		copyContent(bt->expression,tmpVar->name);
		bt->isTmpVar = 1;
		bt->tmpVar = tmpVar;
		bt->hasOp = 0;
	}
}

void operationTraitement(StackBE* stack, BackendTransit* left, TypeBE leftType, BackendTransit* right, TypeBE rightType, char* op){
	affectToTmp(stack,right,rightType);
	affectToTmp(stack,left,leftType);

	concatContent(left->expression, op);
	concatContent(left->expression, right->expression->data);

	freeContent(right->expression);	
	left->hasOp = 1;
	//TODO : opti du isTmpVar...
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
