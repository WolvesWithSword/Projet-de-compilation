D                       [0-9]
L                       [a-zA-Z_]
H                       [a-fA-F0-9]
E                       [Ee][+-]?{D}+
FS                      (f|F|l|L)
IS                      (u|U|l|L)*
COM			"/*"([^*]|(\*[^/]))*"*/"

%{
#include <stdio.h>
#include <stdlib.h>
#include "y.tab.h"

int yylex();
int yyerror();
int verbose = 0;


/* Ne gère pas les commentaires. A rajouter */
/* Supprimer les lignes dont vous n'avez pas besoin. */

%}

%%
{COM}			{ if(verbose) fprintf(stdout, "comment : %s\n",yytext);}

"extern"                { if(verbose) printf("EXTERN"); return EXTERN;}

"goto"                  { if(verbose) printf("GOTO"); return GOTO;}
"if"                    { if(verbose) printf("IF"); return IF;}
"int"                   { if(verbose) printf("INT"); return INT;}

"return"                { if(verbose) printf("RETURN"); return RETURN;}

"void"                  { if(verbose) printf("VOID"); return VOID;}


{L}({L}|{D})*           { if(verbose) printf("IDENTIFIER:%s",yytext); return IDENTIFIER;}


{D}+{IS}?               { if(verbose) printf("CONSTANT:%s",yytext); return CONSTANT;}

"<="                    { if(verbose) printf(" LE_OP "); return LE_OP;}
">="                    { if(verbose) printf(" GE_OP "); return GE_OP;}
"=="                    { if(verbose) printf(" EQ_OP "); return EQ_OP;}
"!="                    { if(verbose) printf(" NE_OP "); return NE_OP;}

"("|")"|"{"|"}"   	{ if(verbose) printf(" %c ",yytext[0]);return yytext[0];}
"*"|"-"|"/"|"+"   	{ if(verbose) printf(" %c ",yytext[0]);return yytext[0];}
"<"|">"|"="|"&"   	{ if(verbose) printf(" %c ",yytext[0]);return yytext[0];}
","|";"|":"		{ if(verbose) printf(" %c ",yytext[0]);return yytext[0];}

[ \t\v\n\f\r]            { if(verbose) printf("%c",yytext[0]);}
.                      { fprintf(stderr,"%c\n",yytext[0]); yyerror("char not accepted"); }
%%
