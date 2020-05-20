CC = gcc
yaccFile = structfe.y
lexFile = ANSI-C.l
exec = parser
testDir = ../../Tests


all : compile

compile_yacc:
	yacc $(yaccFile) -d

compile_lex: y.tab.c
	lex $(lexFile)

y.tab.c: compile_yacc
	


compile: lex.yy.c
	$(CC) -o $(exec) $(shell find . -name "*.c") -lfl

lex.yy.c: compile_lex
	
execTest:
	chmod +x execTest.sh
	./execTest.sh $(testDir) $(exec)

clean:
	rm -f y.tab.h y.tab.c parser lex.yy.c

removeBackend:
	find $(testDir)/ -name *_backend.c -exec rm {} \;

