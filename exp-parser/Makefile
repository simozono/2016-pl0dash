CC = gcc
LEX = flex
LDLIBS = -ll
YACC = bison

EXT_TOKENTABLE = ../scanner/tokentable.h

all: exp-ll-parser01
clean:
	rm -f exp-ll-parser01 exp-ll-parser01.o
	rm -f exp-scanner-ll.c exp-scanner-ll.o

exp-ll-parser01: exp-ll-parser01.o exp-scanner-ll.o
exp-ll-parser01.o: exp-ll-parser01.c $(EXT_TOKENTABLE)

exp-scanner-ll.o: exp-scanner-ll.c $(EXT_TOKENTABLE)
	$(CC) $(CFLAGS) -c -o $@ $<
exp-scanner-ll.c: exp-scanner.l
	$(LEX) $(LFLAGS) -t $< > $@
