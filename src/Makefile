FFLAGS=
BFLAGS=-d -v -y 
CFLAGS=-Wall -Wextra -std=c++11 -Wno-write-strings

FLEX=flex
BISON=bison
CC=g++

SRCS=y.tab.c ast.cpp tokentable.cpp symboltable.cpp main.cpp lex.yy.c
OBJS=y.tab.o ast.o tokentable.o symboltable.o main.o lex.yy.o

parser : ${OBJS}
	${CC} ${CFLAGS} -o $@

${OBJS} : ${SRCS}
	${CC} ${CFLAGS} -c ${SRCS}

y.tab.c : cool.y
	${BISON} ${BFLAGS} cool.y

lex.yy.c : cool.flex
	${FLEX} ${FFLAGS} cool.flex

