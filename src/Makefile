CFLAGS = -Wall -Wextra -pedantic -g
OBJECTS = main.o utils.o lex.yy.o golite.tab.o pretty.o weed.o symbol.o symbolprint.o type.o python.o

winnipeg: $(OBJECTS)
	gcc $(OBJECTS) -o winnipeg -lfl

main.o: golite.tab.h

pp.o: golite.tab.h

python.o: base_py.h

base_py.h: base.py
	xxd -i base.py base_py.h

lex.yy.c: golite.l
	flex golite.l

golite.tab.c golite.tab.h: golite.y
	bison --defines=golite.tab.h golite.y

.PHONY: clean
clean:
	rm -f golite.tab.c golite.tab.h lex.yy.c $(OBJECTS) base_py.h winnipeg
