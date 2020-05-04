%{

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "variable.h"

%}

%union {
	char* name;
	int value;
	Node *tmpNode;
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

%type <tmpNode> direct_declarator
%type <tmpNode> declarator

%start program
%%

primary_expression
        : IDENTIFIER
        | CONSTANT
        | '(' expression ')'
        ;

postfix_expression
        : primary_expression
        | postfix_expression '(' ')'
        | postfix_expression '(' argument_expression_list ')'
        | postfix_expression '.' IDENTIFIER
        | postfix_expression PTR_OP IDENTIFIER
        ;

argument_expression_list
        : expression
        | argument_expression_list ',' expression
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
        | multiplicative_expression '*' unary_expression
        | multiplicative_expression '/' unary_expression
        ;

additive_expression
        : multiplicative_expression
        | additive_expression '+' multiplicative_expression
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
        : declaration_specifiers declarator ';' {addNodeStack(stack,$2);}
        | struct_specifier ';'
        ;

declaration_specifiers
        : EXTERN type_specifier
        | type_specifier
        ;

type_specifier
        : VOID
        | INT
        | struct_specifier
        ;

struct_specifier
        : STRUCT IDENTIFIER '{' struct_declaration_list '}'
        | STRUCT '{' struct_declaration_list '}'
        | STRUCT IDENTIFIER
        ;

struct_declaration_list
        : struct_declaration
        | struct_declaration_list struct_declaration
        ;

struct_declaration
        : type_specifier declarator ';'
        ;

declarator
        : '*' direct_declarator {$$ = $2; /*c'est un pointeur a fair*/}
        | direct_declarator {$$ = $1;}
        ;

direct_declarator
        : IDENTIFIER {Node* node = initNode(); node->name = $1; $$ = node;}
        | '(' declarator ')' {$$=$2;}  
        | direct_declarator '(' parameter_list ')' {Node* node = $1;node->isFonction = 1;/*affecter les parametre*/$$ = node;}
        | direct_declarator '(' ')' {Node* node = $1;node->isFonction = 1;$$ = node;}
        ;

parameter_list
        : parameter_declaration {/*creation d'une liste de parametre*/}
        | parameter_list ',' parameter_declaration {/*concatener les liste*/}
        ;

parameter_declaration
        : declaration_specifiers declarator
        ;

statement
        : compound_statement
        | expression_statement
        | selection_statement
        | iteration_statement
        | jump_statement 
        ;
new_stage
	: {addStageToStack(stack);};

remove_stage
	: {printf("\n");printStack(stack);removeStageToStack(stack);};

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
        : declaration_specifiers declarator compound_statement
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

