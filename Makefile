CC= gcc 
CFLAGS= -Wextra -Wall -std=gnu99 -fPIC
OPT_FLAGS = -O6
DEBUG_FLAGS= -g # -DDEBUG1 # -DDEBUG2
PROFILE_FLAGS=

.c.o :
	$(CC) $(CFLAGS) $(OPT_FLAGS) $(DEBUG_FLAGS) $(PROFILE_FLAGS) -c $<

LIB= 		libcontainer.a libcontainer.so
LIBOBJS= 	list.o stack.o queue.o hashtable.o binarytree.o radixtree.o cbuffer.o pqueue.o
TESTOBJS= 	all_test.o list_test.o stack_test.o queue_test.o hashtable_test.o \
		     binarytree_test.o radixtree_test.o cbuffer_test.o pqueue_test.o
WORDSOBJ=	words.o words_radix.o words_list.o

BIN= 		all_test words 

OBJS= $(TESTOBJS) $(LIBOBJS) $(BINOBJS) $(WORDSOBJ)


all: $(BIN)

all_test: $(TESTOBJS) $(LIB)
	$(CC) -o all_test $(TESTOBJS) libcontainer.a -lcunit -lgcov

words: $(WORDSOBJ) $(LIB)
	$(CC) -o words $(WORDSOBJ) libcontainer.a -lgcov

libcontainer.a: $(LIBOBJS)
	ar rcs libcontainer.a $(LIBOBJS)

libcontainer.so: $(LIBOBJS)
	$(CC) -shared -o libcontainer.so $(LIBOBJS)

clean:	
	/bin/rm -f $(BIN) $(OBJS) $(LIB) cachegrind.out.* *.gcov *.gcda *.gcno *.bak

test: all_test
	./all_test

cppcheck: 
	cppcheck --quiet -I. --inconclusive --std=c99 --language=c --platform=unix64 --enable=all --force  *.c *.h

valgrind: 
	make clean
	make OPT_FLAGS='' 
	valgrind -v --show-leak-kinds=all --read-var-info=yes --leak-check=full ./all_test

cachegrind: 
	make clean
	make OPT_FLAGS='-O6'
	valgrind --tool=cachegrind ./all_test

profile:
	make clean
	make OPT_FLAGS='' PROFILE_FLAGS="-fprofile-arcs -ftest-coverage"
	make

