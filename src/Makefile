CFLAGS = -Wall -Wextra -pedantic -g
OBJECTS = main.o utils.o lex.yy.o golite.tab.o pretty.o weed.o symbol.o symbolprint.o type.o c.o

winnipeg: $(OBJECTS)
	gcc $(OBJECTS) -o winnipeg -lfl

main.o: golite.tab.h

pp.o: golite.tab.h

python.o: base_py.h

c.o: base_c.h

base_c.h: base.c
	xxd -i base.c base_c.h

lex.yy.c: golite.l
	flex golite.l

golite.tab.c golite.tab.h: golite.y
	bison --defines=golite.tab.h golite.y

.PHONY: clean
clean:
	rm -f golite.tab.c golite.tab.h lex.yy.c $(OBJECTS) base_c.h winnipeg
