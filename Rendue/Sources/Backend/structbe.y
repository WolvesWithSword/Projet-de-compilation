%{
#include <stdio.h>
#include <stdlib.h>

int yylex();
int yyerror();


/* Ne gère pas les commentaires. A rajouter */
/* Supprimer les lignes dont vous n'avez pas besoin. */
%}


%token IDENTIFIER CONSTANT 
%token LE_OP GE_OP EQ_OP NE_OP
%token EXTERN
%token INT VOID
%token IF RETURN GOTO


%start program
%%

primary_expression
        : IDENTIFIER
        | CONSTANT
        ;

postfix_expression
        : primary_expression
        | postfix_expression '(' ')'
        | postfix_expression '(' argument_expression_list ')'
        ;

argument_expression_list
        : primary_expression
        | argument_expression_list ',' primary_expression
        ;

unary_expression
        : postfix_expression
        | unary_operator primary_expression
        ;

unary_operator
        : '&'
        | '*'
        | '-'
        ;

multiplicative_expression
        : unary_expression
        | primary_expression '*' primary_expression
        | primary_expression '/' primary_expression
        ;

additive_expression
        : multiplicative_expression
        | primary_expression '+' primary_expression
        | primary_expression '-' primary_expression
        ;

relational_expression
        : additive_expression
        | primary_expression '<' primary_expression
        | primary_expression '>' primary_expression
        | primary_expression LE_OP primary_expression
        | primary_expression GE_OP primary_expression
        ;

equality_expression
        : relational_expression
        | primary_expression EQ_OP primary_expression
        | primary_expression NE_OP primary_expression
        ;

expression
        : equality_expression
        | unary_operator primary_expression '=' primary_expression
        | primary_expression '=' additive_expression
        ;

declaration
        : declaration_specifiers declarator ';'
        ;

declaration_specifiers
        : EXTERN type_specifier
        | type_specifier
        ;

type_specifier
        : VOID
        | INT
        ;

declarator
        : '*' direct_declarator
        | direct_declarator
        ;

direct_declarator
        : IDENTIFIER
	| '(' declarator ')'
        | direct_declarator '(' parameter_list ')'
        | direct_declarator '(' ')'
        ;

parameter_list
        : parameter_declaration
        | parameter_list ',' parameter_declaration
        ;

parameter_declaration
        : declaration_specifiers declarator
        ;

statement
        : compound_statement
        | labeled_statement
        | expression_statement
        | selection_statement
        | jump_statement 
        ;

compound_statement
        : '{' '}'
        | '{' statement_list '}'
        | '{' declaration_list '}'
        | '{' declaration_list statement_list '}'
        ;

declaration_list
        : declaration
        | declaration_list declaration
        ;

statement_list
        : statement
        | statement_list statement
        ;

labeled_statement
        : IDENTIFIER ':' 
        ;

expression_statement
        : ';'
        | expression ';'
        ;

selection_statement
        : IF '(' equality_expression ')' GOTO IDENTIFIER ';'
        ;
jump_statement
        : RETURN ';'
        | RETURN expression ';'
        | GOTO IDENTIFIER ';'
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
        yyparse();
        printf("Parse done\n");
    }
    return 0;
}

