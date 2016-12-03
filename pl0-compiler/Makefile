CC = gcc
LEX = flex
LDLIBS = -ll
YACC = bison

EXT_TOKENTABLE = ../scanner/tokentable.h

all: pl0-ll-parser01 pl0-ll-parser02 pl0-lr-parser01
clean:
	rm -f pl0-ll-parser01 pl0-ll-parser01.o
	rm -f pl0-ll-parser02 pl0-ll-parser02.o
	rm -f pl0-scanner-ll.c pl0-scanner-ll.o
	rm -f misc.o
	rm -f pl0-lr-parser01 pl0-lr-parser01.c pl0-lr-parser01.o
	rm -f pl0-lr-main.o
	rm -f pl0-scanner-lr.c pl0-scanner-lr.o tokentable_bison.h

pl0-ll-parser01: pl0-ll-parser01.o pl0-scanner-ll.o misc.o
pl0-ll-parser01.o: pl0-ll-parser01.c misc.h
pl0-ll-parser02: pl0-ll-parser02.o pl0-scanner-ll.o misc.o
pl0-ll-parser02.o: pl0-ll-parser02.c misc.h

misc.o: misc.c misc.h

pl0-lr-parser01: pl0-lr-main.o pl0-lr-parser01.o pl0-scanner-lr.o
pl0-lr-parser01.o: pl0-lr-parser01.c tokentable_bison.h

pl0-scanner-ll.o: pl0-scanner-ll.c $(EXT_TOKENTABLE)
	$(CC) $(CFLAGS) -c -o $@ $<
pl0-scanner-ll.c: pl0-scanner.l
	$(LEX) $(LFLAGS) -t $< > $@

pl0-scanner-lr.c: pl0-scanner.l
	$(LEX) -DYACC $(LFLAGS) -t $< > $@
pl0-scanner-lr.o: pl0-scanner-lr.c tokentable_bison.h
	$(CC) $(CFLAGS) -DYACC -c -o $@ $<
pl0-lr-parser01.c + tokentable_bison.h: pl0-lr-parser01.y
	$(YACC) --defines=tokentable_bison.h -o pl0-lr-parser01.c $<
