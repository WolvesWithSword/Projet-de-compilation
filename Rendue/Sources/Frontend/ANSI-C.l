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
#include "variable.h"
#include "y.tab.h"

int yylex();
int yyerror();
int verbose = 0;



/* Ne gère pas les commentaires. A rajouter */
/* Supprimer les lignes dont vous n'avez pas besoin. */

%}

%%
{COM}			{ if(verbose) fprintf(stdout, "comment : %s\n",yytext);}

"else"                  { if(verbose) printf("ELSE");return ELSE;  }

"extern"                { if(verbose) printf("EXTERN");return EXTERN; }

"for"                   { if(verbose) printf("FOR");return FOR;}

"if"                    { if(verbose) printf("IF");return IF; }
"int"                   { if(verbose) printf("INT");return INT; }

"return"                { if(verbose) printf("RETURN"); return RETURN; }

"sizeof"                { if(verbose) printf("SIZEOF"); return SIZEOF; }
"struct"                { if(verbose) printf("STRUCT"); return STRUCT; }

"void"                  { if(verbose) printf("VOID"); return VOID;}
"while"                 { if(verbose) printf("WHILE"); return WHILE;}

{L}({L}|{D})*           {yylval.name = strdup(yytext); if(verbose) printf("IDENTIFIER:%s",yytext); return IDENTIFIER;}

{D}+{IS}?               {yylval.value = atoi(yytext); if(verbose) printf("CONSTANT:%s",yytext); return CONSTANT;}



"->"                    { if(verbose) printf(" PTR_OP ");return PTR_OP; }
"&&"                    { if(verbose) printf(" AND_OP ");return AND_OP; }
"||"                    { if(verbose) printf(" OR_OP ");return OR_OP; }
"<="                    { if(verbose) printf(" LE_OP "); return LE_OP;}
">="                    { if(verbose) printf(" GE_OP "); return GE_OP;}
"=="                    { if(verbose) printf(" EQ_OP "); return EQ_OP;}
"!="                    { if(verbose) printf(" NE_OP "); return NE_OP;}

"("|")"|"{"|"}"   { if(verbose) printf(" %c ",yytext[0]);return yytext[0];}
"*"|"-"|"/"|"+"   { if(verbose) printf(" %c ",yytext[0]);return yytext[0];}
"<"|">"|"="|"&"   { if(verbose) printf(" %c ",yytext[0]);return yytext[0];}
","|";"|"."       { if(verbose) printf(" %c ",yytext[0]);return yytext[0];}

[ \t\v\n\f\r]     { if(verbose) printf("%c",yytext[0]);}

.                       { fprintf(stderr,"%c\n",yytext[0]); yyerror("char not accepted"); }
%%

