FFLAGS=
BFLAGS=-d -v -y
CFLAGS=-Wall -Wextra -std=c++11 -stdlib=libc++ -Wno-write-strings

FLEX=flex
BISON=bison
# CC=g++
CC=clang++

SRCS=y.tab.c ast.cpp tokentable.cpp symboltable.cpp main.cpp lex.yy.c semanticanalyzer.cpp astnodevisitor.cpp constants.cpp astnodetypechecker.cpp utility.cpp astnodecodegenerator.cpp
OBJS=y.tab.o ast.o tokentable.o symboltable.o main.o lex.yy.o semanticanalyzer.o astnodevisitor.o constants.o astnodetypechecker.o utility.o astnodecodegenerator.o

cooc : ${OBJS}
	${CC} ${CFLAGS} -o $@ ${OBJS}

%.o : %.cpp
	${CC} ${CFLAGS} -c $<

y.tab.c : cool.y
	${BISON} ${BFLAGS} cool.y

lex.yy.c : cool.flex
	${FLEX} ${FFLAGS} cool.flex

.depend: ${SRCS}
	rm -f ./.depend
	${CC} ${CFLAGS} -MM $^>>./.depend

include .depend
