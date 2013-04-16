all: gen

to_be_removed=*.o *.tab.* lex.yy.c  gen

clean:
	-rm -rf $(to_be_removed)

AST.o: AST.cpp AST.h Label.h Address.h
Address.o: Address.cpp Address.h
Label.o: Label.cpp Label.h


gen: gen.ypp gen.l AST.o AST.h Label.h Address.h Label.o Address.o
	bison -d gen.ypp
	flex gen.l
	g++ gen.tab.cpp lex.yy.c AST.o Label.o Address.o -lfl -o gen 
