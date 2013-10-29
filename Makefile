CC= gcc 
CFLAGS= -Wextra -Wall -std=c99 -fPIC
OPT_FLAGS = -O6
DEBUG_FLAGS= -g# -DDEBUG

.c.o :
	$(CC) $(CFLAGS) $(OPT_FLAGS) $(DEBUG_FLAGS) -c $<

LIB= libcontainer.a libcontainer.so

LIBOBJS= list.o stack.o queue.o hashtable.o
TESTOBJS= all_test.o list_test.o stack_test.o queue_test.o hashtable_test.o

OBJS= all_test.o $(TESTOBJS)  

all_test: $(OBJS) $(LIB)
	$(CC) -o all_test $(TESTOBJS) libcontainer.a -lpthread -lcunit 

libcontainer.a: $(LIBOBJS)
	ar rcs libcontainer.a $(LIBOBJS)

libcontainer.so: $(LIBOBJS)
	$(CC) -shared -o libcontainer.so $(LIBOBJS)
clean:	
	/bin/rm -f all_test $(LIBOBJS) $(TESTOBJS) $(LIB)
