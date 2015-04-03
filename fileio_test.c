
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

void fileio_test_invalid(void) {

	FILE* f = fopen("file.bin", "w+");
	teststruct_t* n = buildTestStruct();
	void* addr = 0x1234;

	CU_ASSERT( readDisk(NULL, f, sizeof(teststruct_t), 1) == EINVAL );
	CU_ASSERT( readDisk(n, NULL, sizeof(teststruct_t), 1) == EINVAL );
	CU_ASSERT( readDisk(n, f, 0, 1) == EINVAL );

	CU_ASSERT( writeDisk(NULL, f, sizeof(teststruct_t), 1) == EINVAL );
	CU_ASSERT( writeDisk(n, NULL, sizeof(teststruct_t), 1) == EINVAL );
	CU_ASSERT( writeDisk(n, f, 0, 1) == EINVAL );

	CU_ASSERT( readMem(NULL, addr, sizeof(teststruct_t), 1) == EINVAL);
	CU_ASSERT( readMem(n, NULL, sizeof(teststruct_t), 1) == EINVAL);
	CU_ASSERT( readMem(n, addr, 0, 1) == EINVAL);
	
	CU_ASSERT( writeMem(NULL, addr, sizeof(teststruct_t), 1) == EINVAL);
	CU_ASSERT( writeMem(n, NULL, sizeof(teststruct_t), 1) == EINVAL);
	CU_ASSERT( writeMem(n, addr, 0, 1) == EINVAL);

	CU_ASSERT( mmapFile(NULL, 0, 1) == NULL);
	CU_ASSERT( mmapFile(f, 0, 0) == NULL);

	free(n);
	fclose(f);
}

void fileio_test(void) {

	unsigned int pgsize = getpagesize();

	FILE* f = fopen("file.bin", "w+");

	teststruct_t* n = buildTestStruct();
	for (unsigned int i=0;i<10;i++) {
		n->id    = i+0x10000000;
		n->value = i+0x20000000;
		sprintf(n->string, "str%u", i);
		CU_ASSERT( writeDisk(n, f, sizeof(teststruct_t), i) == 0 );
	}
	free(n);
	
	teststruct_t* o = buildTestStruct();
	void* addr = mmapFile(f, 0, 8*pgsize);
	CU_ASSERT( addr != NULL );
	for (unsigned int i=0;i<10;i++) {
		CU_ASSERT( readDisk(o, f, sizeof(teststruct_t), i) == 0 );
		CU_ASSERT( o->id == 0x10000000+i );
		CU_ASSERT( o->value == 0x20000000+i );
	}

	for (unsigned int i=0;i<10;i++) {
		o->id    = i+0x30000000;
		o->value = i+0x40000000;
		CU_ASSERT( writeMem(o, addr, sizeof(teststruct_t), i) == 0);
	}

	for (unsigned int i=0;i<10;i++) {
		CU_ASSERT( readMem(o, addr, sizeof(teststruct_t), i) == 0);
		CU_ASSERT( o->id == 0x30000000+i );
		CU_ASSERT( o->value == 0x40000000+i );
	}

	free(o);

	CU_ASSERT( munmapFile(addr, 8*pgsize) == 0 );
	fclose(f);
}
