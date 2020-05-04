CC = gcc
yaccFile = structfe.y
lexFile = ANSI-C.l
exec = parser
testDir = ../../Tests
testFile := $(shell find $(testDir) -name "*.c")


compile_yacc:
	yacc $(yaccFile) -d

compile_lex: y.tab.c
	lex $(lexFile)

y.tab.c: compile_yacc
	


compile: lex.yy.c
	$(CC) -o $(exec) $(shell find . -name "*.c") -lfl

lex.yy.c: compile_lex
	

clean:
	rm -f y.tab.h y.tab.c parser lex.yy.c


