D                       [0-9]
L                       [a-zA-Z_]
H                       [a-fA-F0-9]
E                       [Ee][+-]?{D}+
FS                      (f|F|l|L)
IS                      (u|U|l|L)*
COM			"/*"[^"*/"]*"*/"

%{
#include <stdio.h>
#include <stdlib.h>


/* Ne gère pas les commentaires. A rajouter */
/* Supprimer les lignes dont vous n'avez pas besoin. */

%}

%%
{COM}			{ fprintf(stdout, "comment : %s\n",yytext);}
"auto"                  { /* A completer */ }
"break"                 { /* A completer */ }
"case"                  { /* A completer */ }
"char"                  { /* A completer */ }
"const"                 { /* A completer */ }
"continue"              { /* A completer */ }
"default"               { /* A completer */ }
"do"                    { /* A completer */ }
"double"                { /* A completer */ }
"else"                  { printf("ELSE");  }
"enum"                  { /* A completer */ }
"extern"                { printf("EXTERN"); }
"float"                 { /* A completer */ }
"for"                   { printf("FOR"); }
"goto"                  { /* A completer */ }
"if"                    { printf("IF"); }
"int"                   { printf("INT");}
"long"                  { /* A completer */ }
"register"              { /* A completer */ }
"return"                { printf("RETURN"); }
"short"                 { /* A completer */ }
"signed"                { /* A completer */ }
"sizeof"                { printf("SIZEOF"); }
"static"                { /* A completer */ }
"struct"                { printf("STRUCT"); }
"switch"                { /* A completer */ }
"typedef"               { /* A completer */ }
"union"                 { /* A completer */ }
"unsigned"              { /* A completer */ }
"void"                  { printf("VOID");}
"volatile"              { /* A completer */ }
"while"                 { printf("WHILE");}

{L}({L}|{D})*           { printf("IDENTIFIER");}

0[xX]{H}+{IS}?          { /* A completer */ }
0{D}+{IS}?              { /* A completer */ }
{D}+{IS}?               { printf("CONSTANT");}
L?'(\\.|[^\\'])+'       { /* A completer */ }

{D}+{E}{FS}?            { /* A completer */ }
{D}*"."{D}+({E})?{FS}?  { /* A completer */ }
{D}+"."{D}*({E})?{FS}?  { /* A completer */ }

L?\"(\\.|[^\\"])*\"     { /* A completer */ }

"->"                    { printf("PTR_OP"); }
"&&"                    { printf("AND_OP"); }
"||"                    { printf("OR_OP"); }
"<="                    { printf("LE_OP"); }
">="                    { printf("GE_OP"); }
"=="                    { printf("EQ_OP"); }
"!="                    { printf("NE_OP"); }

"("|")"|"{"|"}"   { printf("%c",yytext[0]);}
"*"|"-"|"/"|"+"   { printf("%c",yytext[0]);}
"<"|">"|"="|"&"   { printf("%c",yytext[0]);}
","|";"|"."       { printf("%c",yytext[0]);}

[ \t\v\n\f\r]     { printf("%c",yytext[0]);}

.                       { fprintf(stderr,"%c\n",yytext[0]); yyerror("char not accepted"); }
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
        yylex();
        printf("Parse done\n");
    }
    return 0;
}
