%{

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "variable.h"
	
	int yylex();	
	int yyerror();
%}

%union {
	char* name;
	int value;
	Transit transit;
	Type* type;
	Variable* var;
	TypeStruct* typeStruct;
}

%token <name> IDENTIFIER
%token <value> CONSTANT
%token SIZEOF
%token PTR_OP LE_OP GE_OP EQ_OP NE_OP
%token AND_OP OR_OP
%token EXTERN
%token INT VOID
%token STRUCT 
%token IF ELSE WHILE FOR RETURN

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%type <transit> direct_declarator
%type <transit> declarator
%type <typeStruct> struct_specifier
%type <type> declaration_specifiers
%type <type> type_specifier
%type <var> parameter_list
%type <var> parameter_declaration
%type <var> struct_declaration_list
%type <var> struct_declaration

%type <type> primary_expression


%start program
%%

primary_expression
        : IDENTIFIER {Type* type = getLastDefineType(stack,$1);
			if(type==NULL) fprintf(stderr,"\n %s : not defined\n",$1); yyerror("ERROR");
			$$ = type;}
        | CONSTANT {Type* type = initType(); type->isUnary = 1; type->unaryType = INT_T; $$ = type;}
        | '(' expression ')' {$$ = $2;}
        ;

postfix_expression
        : primary_expression {}
        | postfix_expression '(' ')' {/*appelle d'une fonction type de cette exp = type retour de la fonction*/}
        | postfix_expression '(' argument_expression_list ')' {/*appelle d'une fonction*/}
        | postfix_expression '.' IDENTIFIER {/* type structure il faut verifier si la structure a bien le champs identifieur*/}
        | postfix_expression PTR_OP IDENTIFIER {/* type structure il faut verifier si la structure a bien le champs identifieur*/}
        ;

argument_expression_list
        : expression 
        | argument_expression_list ',' expression {/*Liste des argument d'un appelle de fonction*/}
        ;

unary_expression
        : postfix_expression
        | unary_operator unary_expression
        | SIZEOF unary_expression
        ;

unary_operator
        : '&'
        | '*'
        | '-'
        ;

multiplicative_expression
        : unary_expression
        | multiplicative_expression '*' unary_expression {/*verification 2 expression 2 int*/}
        | multiplicative_expression '/' unary_expression
        ;

additive_expression
        : multiplicative_expression
        | additive_expression '+' multiplicative_expression{/*verification 2 expression regle int / ptr*/}
        | additive_expression '-' multiplicative_expression
        ;

relational_expression
        : additive_expression
        | relational_expression '<' additive_expression
        | relational_expression '>' additive_expression
        | relational_expression LE_OP additive_expression
        | relational_expression GE_OP additive_expression
        ;

equality_expression
        : relational_expression
        | equality_expression EQ_OP relational_expression
        | equality_expression NE_OP relational_expression
        ;

logical_and_expression
        : equality_expression
        | logical_and_expression AND_OP equality_expression
        ;

logical_or_expression
        : logical_and_expression
        | logical_or_expression OR_OP logical_and_expression
        ;

expression
        : logical_or_expression
        | unary_expression '=' expression
        ;

declaration
        : declaration_specifiers declarator ';' {
		if($2.variableD == NULL && $2.fonctionD == NULL && $2.name != NULL) {

			if(isExistingInStageName(stack,$2.name)) {fprintf(stderr,"\nPrevious declaration of %s was here\n",$2.name); yyerror("ERROR");} 	
		
			$1->isPtr = $2.isPtr;
			Variable* var = initVariable();
			var->name = $2.name;
			var->type = $1;
			addVariableToStack(stack,var);
		}
		if($2.variableD == NULL && $2.fonctionD != NULL && $2.name != NULL) {

			if(isExistingInStageFunction(stack,$2.fonctionD)) {fprintf(stderr,"\nPrevious declaration of %s was here\n",$2.name); yyerror("ERROR");} 
				
			
			$2.fonctionD->type->isPtr = $2.isPtr;
			$2.fonctionD->type->isFunction = 1;

			$1->isPtr = $2.isPtr;
			$2.fonctionD->type->functionType = initFunctionType();
			$2.fonctionD->type->functionType->returnType = $1;
			$2.fonctionD->type->functionType->parameters = variableToParameterType($2.fonctionD->variables);
			
				
			addFonctionToStack(stack,$2.fonctionD);
		}
		else{}
	}
        | struct_specifier ';'
        ;

declaration_specifiers
        : EXTERN type_specifier {$$ = $2;}
        | type_specifier {$$ = $1;}
        ;

type_specifier
        : VOID {Type* tp = initType(); tp->unaryType = VOID_T; tp->isUnary = 1; $$ = tp;}
        | INT  {Type* tp = initType(); tp->unaryType = INT_T; tp->isUnary = 1; $$ = tp;}
        | struct_specifier  {Type* tp = initType(); tp->typeStruct = $1; $$ = tp;}
        ;

struct_specifier
        : STRUCT IDENTIFIER {TypeStruct* ts = initTypeStruct(); ts->name = $2; 
		if(isExistingInStageStruct(stack,ts)) {fprintf(stderr,"\n %s : Struct already declared\n",$2); yyerror("ERROR");}
		addTypeStructToStack(stack,ts); $<typeStruct>$ = ts;} 
	'{' struct_declaration_list '}' 
		{$<typeStruct>3->variables = $5; $$ = $<typeStruct>3;}
        | STRUCT '{' struct_declaration_list '}' {printf("		NOT IMPLEMENTED		");/*TypeStruct* ts = initTypeStruct(); ts->name = "_";
		Transit t; t.variableD = NULL; t.fonctionD = NULL; t.typeStructD = ts; t.name = "_"; $$ = t;*/}
        | STRUCT IDENTIFIER {TypeStruct* ts = isCreatedStruct(stack,$2); 
		if(ts == NULL) {fprintf(stderr,"\n %s : Struct not declared before\n",$2); yyerror("ERROR");}; $$ = ts;}
        ;



struct_declaration_list
        : struct_declaration {$$ = $1;}
        | struct_declaration_list struct_declaration {addVariable(&$2,$1); $$ = $2;}
        ;

struct_declaration
        : type_specifier declarator ';' {if($2.variableD == NULL && $2.fonctionD == NULL && $2.name != NULL) { 

		$1->isPtr = $2.isPtr;
		Variable* var = initVariable(); var->type = $1; var->name = $2.name; $$ = var;
	}else{
		fprintf(stderr,"\n %s : Struct parameters can't be functions declaration\n",$2.name); yyerror("ERROR");	
	}}
        ;

declarator
        : '*' direct_declarator {
		if($2.isPtr){ /*c'est un pointeur de pointeur*/
			fprintf(stderr,"\n %s : Can be use pointeur of pointeur\n",$2.name); yyerror("ERROR");	
		}
		$2.isPtr=1;
		$$ = $2;
	}
        | direct_declarator {$$ = $1;}
        ;

direct_declarator
        : IDENTIFIER {Transit t; t.isPtr = 0;  t.variableD = NULL; t.fonctionD = NULL; t.name = $1; $$ = t; }
        | '(' declarator ')' {$$=$2;}  
        | direct_declarator '(' parameter_list ')' {Transit t1 = $1; Fonction* f = initFonction(); f->name = t1.name; f->variables = $3;
		f->type = initType(); f->type->isFunction = 1; f->type->isPtr =t1.isPtr;
		Transit t2; t2.isPtr = 0; t2.variableD = NULL; t2.fonctionD = f; t2.name = t1.name; $$ = t2; }
        | direct_declarator '(' ')' {Transit t1 = $1; Fonction* f = initFonction(); f->name = t1.name;
		f->type = initType(); f->type->isFunction = 1; f->type->isPtr =t1.isPtr;
		Transit t2; t2.isPtr = 0; t2.variableD = NULL; t2.fonctionD = f; t2.name = t1.name; $$ = t2; }
        ;

parameter_list
        : parameter_declaration {$$ = $1;}
        | parameter_list ',' parameter_declaration {addVariable(&$3,$1); $$ = $3;}
        ;

parameter_declaration
        : declaration_specifiers declarator {if($2.variableD == NULL && $2.fonctionD == NULL && $2.name != NULL) { 
		$1->isPtr = $2.isPtr;
		Variable* var = initVariable(); var->type = $1; var->name = $2.name; $$ = var;
	}else{
		fprintf(stderr,"\n %s : Parameters can't be functions declaration\n",$2.name); yyerror("ERROR");	
	}}
        ;

statement
        : compound_statement
        | expression_statement
        | selection_statement
        | iteration_statement
        | jump_statement 
        ;
new_stage
	: {printf("\nadd stage\n");addStageToStack(stack);};

remove_stage
	: {printf("\nremove stage\n");printStack(stack);removeStageToStack(stack);};

compound_statement
        : '{' '}'
        | '{' new_stage statement_list '}' remove_stage
        | '{' new_stage declaration_list '}' remove_stage
        | '{' new_stage declaration_list statement_list '}' remove_stage
        ;

declaration_list
        : declaration
        | declaration_list declaration
        ;

statement_list
        : statement
        | statement_list statement
        ;

expression_statement
        : ';'
        | expression ';'
        ;

selection_statement
        : IF '(' expression ')' statement %prec LOWER_THAN_ELSE
        | IF '(' expression ')' statement ELSE statement
        ;

iteration_statement
        : WHILE '(' expression ')' statement
        | FOR '(' expression_statement expression_statement expression ')' statement
        ;

jump_statement
        : RETURN ';'
        | RETURN expression ';' 
        ;

program
        : external_declaration
        | program external_declaration
        ;

external_declaration
        : function_definition
        | declaration
        ;

function_definition
        : declaration_specifiers declarator {
		if($2.fonctionD == NULL) {fprintf(stderr,"\n %s : Not a function definition \n",$2.name); yyerror("ERROR");};

		if(isExistingInStageFunction(stack,$2.fonctionD)) {fprintf(stderr,"\nPrevious declaration of %s was here\n",$2.name); yyerror("ERROR");} 

		
		$2.fonctionD->type->isPtr = $2.isPtr;
		$2.fonctionD->type->isFunction = 1;

		$1->isPtr = $2.isPtr;
		$2.fonctionD->type->functionType = initFunctionType();
		$2.fonctionD->type->functionType->returnType = $1;
		$2.fonctionD->type->functionType->parameters = variableToParameterType($2.fonctionD->variables);
		addFonctionToStack(stack,$2.fonctionD);
		stack->top->currentFunction = $2.fonctionD;
		} 
		compound_statement
        ;

%%

extern FILE *yyin;
int yyerror( char *s ){
	fprintf(stderr,"%s\n",s);
	exit(1);
}

int main(int argc, char *argv[])
{
	if((yyin = fopen(argv[1],"r"))==NULL){
		fprintf(stderr,"File not found\n");
		exit(1);
	} 
	else {
		stack = newStack();
		addStageToStack(stack);
		yyparse();
        	printf("Parse done\n");
	}
    	return 0;
}

