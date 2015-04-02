
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>

#include <CUnit/Basic.h>

#include "fileio.h"
#include "fileio_test.h"

int init_fileio_suite(void) {
	return 0;
}

int clean_fileio_suite(void) {
	return 0;
}

#define MAX_LEN 16
typedef struct teststruct_t teststruct_t;
struct teststruct_t {
	unsigned long id;
	unsigned long value;
	char string[MAX_LEN];
};

teststruct_t* buildTestStruct() {
	teststruct_t* n = (teststruct_t*)malloc(sizeof(teststruct_t));
	if (n != NULL) {
		memset(n, 0, sizeof(teststruct_t));
	}
	return n;	
}

void fileio_test(void) {

	unsigned int pgsize = getpagesize();

	FILE* f = fopen("file.bin", "w+");

	teststruct_t* n = buildTestStruct();
	for (unsigned int i=0;i<1000;i++) {
		n->id = i;
		n->value = i+0x10000000;
		sprintf(n->string, "str%d", i);
		CU_ASSERT( writeDisk(n, f, sizeof(teststruct_t), i) == 0 );
	}
	free(n);
	
	teststruct_t* o = buildTestStruct();
	void* addr = mmapFile(f, 0, 8*pgsize);
	CU_ASSERT( addr != NULL );
	for (unsigned int i=0;i<1000;i++) {
		CU_ASSERT( readDisk(o, f, sizeof(teststruct_t), i) == 0 );
		CU_ASSERT( o->id == i );
		CU_ASSERT( o->value == 0x10000000+i );

		CU_ASSERT( readMem(o, addr, sizeof(teststruct_t), i) == 0);
		CU_ASSERT( o->id == i );
		CU_ASSERT( o->value == 0x10000000+i );
	}
	free(o);

	CU_ASSERT( munmapFile(addr, 8*pgsize) == 0 );
	fclose(f);
}
