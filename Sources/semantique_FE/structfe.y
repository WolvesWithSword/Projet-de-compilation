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
    ParameterType* parameterType;
	ExpressionTransit expr;
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

%type <expr> primary_expression
%type <expr> postfix_expression
%type <parameterType> argument_expression_list
%type <expr> unary_expression
%type <value> unary_operator
%type <expr> multiplicative_expression
%type <expr> additive_expression
%type <expr> relational_expression
%type <expr> equality_expression
%type <expr> logical_and_expression
%type <expr> logical_or_expression
%type <expr> expression
%type <expr> expression_statement

%start program
%%

primary_expression
    : IDENTIFIER {
    	Type* type = getLastDefineType(stack,$1);
        if(type==NULL) {fprintf(stderr,"\n %s : not defined\n",$1); yyerror("SEMANTIC ERROR");}
		ExpressionTransit exp; exp.nameId = $1; exp.isId = 1; exp.isAffectable = 1; typeCopy(&exp.type,type);$$ = exp;
	}
    | CONSTANT {
    	Type type = {0}; type.isUnary = 1; type.unaryType = INT_T;
		ExpressionTransit exp; exp.nameId = NULL; exp.isId = 0; exp.isAffectable = 0; exp.type = type; $$ = exp;
	}
    | '(' expression ')' {$$ = $2;}
    ;

postfix_expression
    : primary_expression {$$ = $1;}
    | postfix_expression '(' ')' {
    	if($1.type.isFunction){
            if($1.type.functionType->parameters ==NULL) {
				ExpressionTransit exp; exp.nameId = NULL; exp.isId = 0; typeCopy(&exp.type,$1.type.functionType->returnType);exp.isAffectable = 0;
				$$ = exp;
			}
            else{ fprintf(stderr,"\n%s :bad parameter to call function\n",$1.nameId); yyerror("SEMANTIC ERROR");}
        }
        else {fprintf(stderr,"\nnot a function: ");yyerror("SEMANTIC ERROR");}
    }
   	| postfix_expression '(' argument_expression_list ')' { /*TODO clear la argument_expression_list (après verif)*/
        if($1.type.isFunction){
            if(compareParameterType($1.type.functionType->parameters,$3)) {
				ExpressionTransit exp; exp.nameId = NULL; exp.isId = 0; typeCopy(&exp.type,$1.type.functionType->returnType); exp.isAffectable = 0;
				$$ = exp;
			}
            else{ fprintf(stderr,"\n%s : bad parameter to call function\n",$1.nameId); yyerror("SEMANTIC ERROR");}
        }
        else {fprintf(stderr,"\nnot a function\n"); yyerror("SEMANTIC ERROR");}
    }
    | postfix_expression '.' IDENTIFIER {fprintf(stderr,"\n struct must be use with pointer\n"); yyerror("SEMANTIC ERROR");}
	| postfix_expression PTR_OP IDENTIFIER {
        if($1.type.isUnary==0 && $1.type.isFunction==0){
            if($1.type.isPtr){
                Variable* var = getVariable($1.type.typeStruct->variables,$3);
                if(var==NULL){fprintf(stderr,"\n struct %s : not has %s\n",$1.type.typeStruct->name,$3); yyerror("SEMANTIC ERROR");}
                else{
					ExpressionTransit exp = $1;
					exp.isId = 0;
					exp.nameId = NULL;
					exp.isAffectable = 1;
					typeCopy(&exp.type,var->type);
					$$ = exp;
				}
            }
            else{fprintf(stderr,"\nuse -> without a struct pointer\n"); yyerror("SEMANTIC ERROR");}
        }
        else{fprintf(stderr,"\nuse -> not a structure\n"); yyerror("SEMANTIC ERROR");}  
	/* type structure il faut verifier si la structure a bien le champs identifieur*/
	}
    ;

argument_expression_list
    : expression {
       	Type* type = initType();typeCopy(type,&$1.type); ParameterType* pt = initParameterType();
        pt->type =type; ;$$=pt; /*TODO enlever le malloc*/
    }
   	| argument_expression_list ',' expression {
		addParameterType(&$1,&$3.type);/*Liste des argument d'un appelle de fonction*/
	}
    ;

unary_expression
    : postfix_expression {$$ = $1;}
    | unary_operator unary_expression {
        /*TODO changer la gestion pointeur d epointeur*/
        if($1==1){
			if($2.type.isPtr==0){
				if($2.isId==1){ $2.type.isPtr=1; $2.isId = 0; $2.nameId=NULL; $2.isAffectable = 0; $$=$2;}
				else {fprintf(stderr,"\n& can be use only on identifieur\n"); yyerror("SEMANTIC ERROR");}
			}
			else {fprintf(stderr,"\ncan be use pointer of pointer\n"); yyerror("SEMANTIC ERROR");}
        }
		else if($1==0){
			if($2.type.isPtr==1) {$2.type.isPtr=0; $2.isId = 0; $2.isAffectable = 1; $2.nameId=NULL; $$=$2;}
			else{fprintf(stderr,"\n * can be use only on pointer\n"); yyerror("SEMANTIC ERROR");}
		}
        else if($1==2 && $2.type.isPtr==0 && $2.type.isUnary==1 && $2.type.unaryType == INT_T){
        	$2.isId = 0;$2.isAffectable = 0; $2.nameId=NULL ;$$ = $2;}
        else {fprintf(stderr,"\n bad use of unaryOperator\n"); yyerror("SEMANTIC ERROR");}
	}
    | SIZEOF unary_expression {
    	Type type = {0}; type.isUnary = 1; type.unaryType = INT_T;
    	ExpressionTransit exp; exp.nameId = NULL; exp.isId = 0;exp.isAffectable = 0; exp.type = type; $$ = exp; /*TODO peut etre un type*/
    }
    ;

unary_operator
    : '&' {$$ = 1;}
    | '*' {$$ = 0;}
    | '-' {$$ = 2;}
    ;

multiplicative_expression
    : unary_expression {$$ = $1;}
    | multiplicative_expression '*' unary_expression {
		if(($1.type.isUnary == 1 && $1.type.unaryType == INT_T) && ($3.type.isUnary == 1 && $3.type.unaryType == INT_T)) {
			$1.isId = 0;$1.isAffectable = 0; $$ = $1;
		}
		else {fprintf(stderr,"\nmultiplication between wrong types\n"); yyerror("SEMANTIC ERROR");} 
	}
    | multiplicative_expression '/' unary_expression {
		if(($1.type.isUnary == 1 && $1.type.unaryType == INT_T) && ($3.type.isUnary == 1 && $3.type.unaryType == INT_T)) {
			$1.isId = 0; $1.isAffectable = 0; $$ = $1;}
		else {fprintf(stderr,"\ndivision between wrong types\n"); yyerror("SEMANTIC ERROR");} 
	}
    ; 	

additive_expression
    : multiplicative_expression {$$ = $1;}
    | additive_expression '+' multiplicative_expression{
		if(($1.type.isUnary == 1 && $1.type.unaryType == INT_T) && ($3.type.isUnary == 1 && $3.type.unaryType == INT_T)) {
			$1.isId = 0; $1.isAffectable = 0; $$ = $1;} /* int + int = int */
		else if(($1.type.isPtr == 1) && ($3.type.isUnary == 1 && $3.type.unaryType == INT_T)) {
			$1.isId = 0; $1.isAffectable = 0; $$ = $1;} /* ptr + int = ptr */
		else {fprintf(stderr,"\n+ :  bad use of pointers\n"); yyerror("SEMANTIC ERROR");} 
	}
    | additive_expression '-' multiplicative_expression{
		if(($1.type.isUnary == 1 && $1.type.unaryType == INT_T) && ($3.type.isUnary == 1 && $3.type.unaryType == INT_T)) {
			$1.isId = 0;$$ = $1;} /* int + int = int */
		else if(($1.type.isPtr == 1) && ($3.type.isUnary == 1 && $3.type.unaryType == INT_T)) {
			$1.isId = 0; $1.isAffectable = 0; $$ = $1;} /* ptr + int = ptr */
		else if(($1.type.isPtr == 1) && ($3.type.isPtr == 1)) {
			Type type = {0}; type.isUnary = 1; type.unaryType = INT_T;
			ExpressionTransit exp; exp.nameId = NULL; exp.isId = 0; exp.type = type; $$ = exp;}/* ptr - ptr = int */
		else {fprintf(stderr,"\n- :  bad use of pointers\n"); yyerror("SEMANTIC ERROR");} 
	}
    ;

relational_expression
    : additive_expression {$$ = $1;}
    | relational_expression '<' additive_expression{
		if(($1.type.isUnary == 1 && $1.type.unaryType == INT_T) && ($3.type.isUnary == 1 && $3.type.unaryType == INT_T)) {
			$1.isId = 0; $1.isAffectable = 0; $$ = $1;}
		else {fprintf(stderr,"\n< : logical expression between wrong types\n"); yyerror("SEMANTIC ERROR");} 
	}
    | relational_expression '>' additive_expression{
		if(($1.type.isUnary == 1 && $1.type.unaryType == INT_T) && ($3.type.isUnary == 1 && $3.type.unaryType == INT_T)) {
			$1.isId = 0; $1.isAffectable = 0; $$ = $1;}
		else {fprintf(stderr,"\n> :logical expression between wrong types\n"); yyerror("SEMANTIC ERROR");} 
	}
    | relational_expression LE_OP additive_expression{
		if(($1.type.isUnary == 1 && $1.type.unaryType == INT_T) && ($3.type.isUnary == 1 && $3.type.unaryType == INT_T)) {
			$1.isId = 0; $1.isAffectable = 0; $$ = $1;}
		else {fprintf(stderr,"\n<= : logical expression between wrong types\n"); yyerror("SEMANTIC ERROR");} 
	}
    | relational_expression GE_OP additive_expression{
		if(($1.type.isUnary == 1 && $1.type.unaryType == INT_T) && ($3.type.isUnary == 1 && $3.type.unaryType == INT_T)) {
			$1.isId = 0; $1.isAffectable = 0; $$ = $1;}
		else {fprintf(stderr,"\n>= : logical expression between wrong types\n"); yyerror("SEMANTIC ERROR");} 
	}
    ;

equality_expression
    : relational_expression {$$ = $1;}
    | equality_expression EQ_OP relational_expression{
		if(($1.type.isUnary == 1 && $1.type.unaryType == INT_T) && ($3.type.isUnary == 1 && $3.type.unaryType == INT_T)) {
            $1.isId = 0; $1.isAffectable = 0; $$ = $1;}
		else {fprintf(stderr,"\n== : logical expression between wrong types\n"); yyerror("SEMANTIC ERROR");} 
	}
    | equality_expression NE_OP relational_expression{
		if(($1.type.isUnary == 1 && $1.type.unaryType == INT_T) && ($3.type.isUnary == 1 && $3.type.unaryType == INT_T)) {
            $1.isId = 0; $1.isAffectable = 0; $$ = $1;}
		else {fprintf(stderr,"\n!= : logical expression between wrong types\n"); yyerror("SEMANTIC ERROR");} 
	}
    ;

logical_and_expression
    : equality_expression {$$ = $1;}
    | logical_and_expression AND_OP equality_expression{
		if(($1.type.isUnary == 1 && $1.type.unaryType == INT_T) && ($3.type.isUnary == 1 && $3.type.unaryType == INT_T)) {
            $1.isId = 0; $1.isAffectable = 0; $$ = $1;}
		else {fprintf(stderr,"\n&& : logical expression between wrong types\n"); yyerror("SEMANTIC ERROR");} 
	}
    ;

logical_or_expression
    : logical_and_expression {$$ = $1;}
    | logical_or_expression OR_OP logical_and_expression{
	   if(($1.type.isUnary == 1 && $1.type.unaryType == INT_T) && ($3.type.isUnary == 1 && $3.type.unaryType == INT_T)) {
            $1.isId = 0; $1.isAffectable = 0; $$ = $1;}
		else {fprintf(stderr,"\n|| : logical expression between wrong types\n"); yyerror("SEMANTIC ERROR");} 
	}
    ;

expression
    : logical_or_expression {$$ = $1;/* je sais pas si il faut remonter */}
    | unary_expression '=' expression {/* possible d'ecrire function(truc , truc) = truc ????? */
		if($1.isAffectable){
			int res = compareTypeForOp(&$1.type,&$3.type);
			if(res == 1){
				$$.isAffectable = 0; $$ = $3;
			}
			else if(res == 2){fprintf(stderr,"\nWarning : makes integer from pointer\n");}
            else{fprintf(stderr,"\nError: incompatible types when assigning\n"); yyerror("SEMANTIC ERROR");}
		}
        else{fprintf(stderr,"\nError: lvalue required as left operand of assignement\n"); yyerror("SEMANTIC ERROR");}
	}
    ;

declaration
    : declaration_specifiers declarator ';' {
    	if($2.variableD == NULL && $2.fonctionD == NULL && $2.name != NULL) {

            if(isExistingInStageName(stack,$2.name)) {fprintf(stderr,"\nPrevious declaration of %s was here\n",$2.name); yyerror("SEMANTIC ERROR");}     
        
            $1->isPtr = $2.isPtr;
            Variable* var = initVariable();
            var->name = $2.name;
            var->type = $1;
            addVariableToStack(stack,var);
        }
        else if($2.variableD == NULL && $2.fonctionD != NULL && $2.name != NULL) {

            if(isExistingInStageFunction(stack,$2.fonctionD)) {fprintf(stderr,"\nPrevious declaration of %s was here\n",$2.name); yyerror("SEMANTIC ERROR");} 
                
            $2.fonctionD->type->isFunction = 1;
            $1->isPtr = $2.isPtr;
            $2.fonctionD->type->functionType = initFunctionType();
            $2.fonctionD->type->functionType->returnType = $1;
			$2.fonctionD->type->functionType->returnType->isPtr = $2.isPtr;
            $2.fonctionD->type->functionType->parameters = variableToParameterType($2.fonctionD->variables);
               
            addFonctionToStack(stack,$2.fonctionD);
        }
        else{fprintf(stderr,"\ndeclaration error\n"); yyerror("SEMANTIC ERROR");}
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
    : STRUCT IDENTIFIER 
    {
        TypeStruct* ts = initTypeStruct(); ts->name = $2; 
        if(isExistingInStageStruct(stack,ts)) {fprintf(stderr,"\n %s : Struct already declared\n",$2); yyerror("SEMANTIC ERROR");}
        addTypeStructToStack(stack,ts); $<typeStruct>$ = ts;
    } 
    '{' struct_declaration_list '}' 
    {
        $<typeStruct>3->variables = $5; $$ = $<typeStruct>3;
    }
    | STRUCT '{' struct_declaration_list '}' {
        printf("      NOT IMPLEMENTED     ");/*TypeStruct* ts = initTypeStruct(); ts->name = "_";
        Transit t; t.variableD = NULL; t.fonctionD = NULL; t.typeStructD = ts; t.name = "_"; $$ = t;*/
    }
    | STRUCT IDENTIFIER {
        TypeStruct* ts = isCreatedStruct(stack,$2); 
        if(ts == NULL) {fprintf(stderr,"\n %s : Struct not declared before\n",$2); yyerror("SEMANTIC ERROR");}
        $$ = ts;
    }
    ;



struct_declaration_list
    : struct_declaration {$$ = $1;}
    | struct_declaration_list struct_declaration {addVariable(&$1,$2); $$ = $1;}
    ;

struct_declaration
    : type_specifier declarator ';' {
        if($2.variableD == NULL && $2.fonctionD == NULL && $2.name != NULL) { 
            $1->isPtr = $2.isPtr;
            Variable* var = initVariable(); var->type = $1; var->name = $2.name; $$ = var;
        }
        else{fprintf(stderr,"\n %s : Struct parameters can't be functions declaration\n",$2.name); yyerror("SEMANTIC ERROR");}
    }
    ;

declarator
    : '*' direct_declarator {
        if($2.isPtr){ /*c'est un pointeur de pointeur*/
            fprintf(stderr,"\n %s : Can be use pointeur of pointeur\n",$2.name); yyerror("SEMANTIC ERROR");  
        }
        $2.isPtr=1;
        $$ = $2;
    }
    | direct_declarator {$$ = $1;}
    ;

direct_declarator
    : IDENTIFIER {Transit t; t.isPtr = 0;  t.variableD = NULL; t.fonctionD = NULL; t.name = $1; $$ = t;}
    | '(' declarator ')' {$$=$2;}  
    | direct_declarator '(' parameter_list ')' {
        Transit t1 = $1; Fonction* f = initFonction(); f->name = t1.name; f->variables = $3;
        f->type = initType(); f->type->isFunction = 1; f->type->isPtr =t1.isPtr;
        Transit t2; t2.isPtr = 0; t2.variableD = NULL; t2.fonctionD = f; t2.name = t1.name; $$ = t2;
    }
    | direct_declarator '(' ')' {
        Transit t1 = $1; Fonction* f = initFonction(); f->name = t1.name;
        f->type = initType(); f->type->isFunction = 1; f->type->isPtr =t1.isPtr;
        Transit t2; t2.isPtr = 0; t2.variableD = NULL; t2.fonctionD = f; t2.name = t1.name; $$ = t2; 
    }
    ;

parameter_list
    : parameter_declaration {$$ = $1;}
    | parameter_list ',' parameter_declaration {addVariable(&$3,$1); $$ = $3;}
    ;

parameter_declaration
    : declaration_specifiers declarator {
        if($2.variableD == NULL && $2.fonctionD == NULL && $2.name != NULL) { 
            $1->isPtr = $2.isPtr;
            Variable* var = initVariable(); var->type = $1; var->name = $2.name; $$ = var;
        }
        else{fprintf(stderr,"\n %s : Parameters can't be functions declaration\n",$2.name); yyerror("SEMANTIC ERROR");}
    }
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
    | '{' new_stage declaration_statement_list '}' remove_stage
    ;

declaration_statement_list
	: declaration_statement_list declaration
	| declaration_statement_list statement
	| declaration
	| statement {
	/*declaration_list
    : declaration
    | declaration_list declaration
    ;

	statement_list
    : statement
    | statement_list statement
    ; */ 
	}
	;


expression_statement
    : ';'{/*return int?*/}
    | expression ';' {$$ = $1;}
    ;

selection_statement
    : IF '(' expression ')' statement %prec LOWER_THAN_ELSE {
		if($3.type.isPtr==1){printf("\nWarning: use pointer instead of int\n");}
		if(!($3.type.isPtr==1 || ($3.type.isPtr==0 && $3.type.isUnary==1 && $3.type.unaryType == INT_T))){
			fprintf(stderr,"\nif need a int value\n"); yyerror("SEMANTIC ERROR"); 
		}
	}	
    | IF '(' expression ')' statement ELSE statement{
		if($3.type.isPtr==1){printf("\nWarning: use pointer instead of int\n");}
		if(!($3.type.isPtr==1 || ($3.type.isPtr==0 && $3.type.isUnary==1 && $3.type.unaryType == INT_T))){
			fprintf(stderr,"\nif need a int value\n"); yyerror("SEMANTIC ERROR"); 
		}
	}
    ;

iteration_statement
    : WHILE '(' expression ')' statement {
		if($3.type.isPtr==1){printf("\nWarning: use pointer instead of int\n");}
		if(!($3.type.isPtr==1 || ($3.type.isPtr==0 && $3.type.isUnary==1 && $3.type.unaryType == INT_T))){
			fprintf(stderr,"\nif need a int value\n"); yyerror("SEMANTIC ERROR"); 
		}
	}
    | FOR '(' expression_statement expression_statement expression ')' statement {
		if($4.type.isPtr==1){printf("\nWarning: use pointer instead of int\n");}
		if(!($4.type.isPtr==1 || ($4.type.isPtr==0 && $4.type.isUnary==1 && $4.type.unaryType == INT_T))){
			fprintf(stderr,"\nif need a int value\n"); yyerror("SEMANTIC ERROR"); 
		}
	}
	| FOR '(' expression_statement expression_statement ')' statement{
		if($4.type.isPtr==1){printf("\nWarning: use pointer instead of int\n");}
		if(!($4.type.isPtr==1 || ($4.type.isPtr==0 && $4.type.isUnary==1 && $4.type.unaryType == INT_T))){
			fprintf(stderr,"\nif need a int value\n"); yyerror("SEMANTIC ERROR"); 
		}
	}
    ;

jump_statement
    : RETURN ';' {/*si pas void warning int x(){return;}*/}
    | RETURN expression ';' {
		Fonction* currentFonction = getCurrentFonction(stack);
		if(currentFonction == NULL){ fprintf(stderr,"\n You can't return outside a function.\n"); yyerror("SEMANTIC ERROR");}
		if(!compareType(currentFonction->type->functionType->returnType,&$2.type)){fprintf(stderr,"\n bad return type\n"); yyerror("SEMANTIC ERROR");}
	}		
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
    : declaration_specifiers declarator 
    {
        if($2.fonctionD == NULL) {fprintf(stderr,"\n %s : Not a function definition \n",$2.name); yyerror("SEMANTIC ERROR");};

        if(isExistingInStageFunction(stack,$2.fonctionD)) {fprintf(stderr,"\nPrevious declaration of %s was here\n",$2.name); yyerror("SEMANTIC ERROR");} 

        
		$2.fonctionD->type->isFunction = 1;
        $1->isPtr = $2.isPtr;
        $2.fonctionD->type->functionType = initFunctionType();
        $2.fonctionD->type->functionType->returnType = $1;
        $2.fonctionD->type->functionType->returnType->isPtr = $2.isPtr;
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

