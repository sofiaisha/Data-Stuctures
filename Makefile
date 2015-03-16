CC= gcc 
CFLAGS= -Wextra -Wall -std=gnu99 -fPIC
OPT_FLAGS = -O6
DEBUG_FLAGS= -g # -DDEBUG1 # -DDEBUG2

.c.o :
	$(CC) $(CFLAGS) $(OPT_FLAGS) $(DEBUG_FLAGS) -c $<

LIB= 		libcontainer.a libcontainer.so
LIBOBJS= 	list.o stack.o queue.o hashtable.o binarytree.o radixtree.o cbuffer.o
TESTOBJS= 	all_test.o list_test.o stack_test.o queue_test.o hashtable_test.o \
		     binarytree_test.o radixtree_test.o cbuffer_test.o
WORDSOBJ=	words.o words_radix.o

BIN= 		all_test words 

OBJS= $(TESTOBJS) $(LIBOBJS) $(BINOBJS) $(WORDSOBJ)


all: $(BIN)

all_test: $(TESTOBJS) $(LIB)
	$(CC) -o all_test $(TESTOBJS) libcontainer.a -lpthread -lcunit 

words: $(WORDSOBJ) $(LIB)
	$(CC) -o words $(WORDSOBJ) libcontainer.a 

libcontainer.a: $(LIBOBJS)
	ar rcs libcontainer.a $(LIBOBJS)

libcontainer.so: $(LIBOBJS)
	$(CC) -shared -o libcontainer.so $(LIBOBJS)

clean:	
	/bin/rm -f $(BIN) $(OBJS) $(LIB) cachegrind.out.*

test: all_test
	./all_test

cppcheck: 
	cppcheck --quiet -I. --std=c99 --language=c --platform=unix64 --enable=all --force  *.c *.h

valgrind: 
	make clean
	make OPT_FLAGS='' 
	valgrind --leak-check=full ./all_test

cachegrind: 
	make clean
	make OPT_FLAGS='-O6'
	valgrind --tool=cachegrind ./all_test
