
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <assert.h>

#include "fileio.h"

int readDisk(void* n, FILE* f, size_t recordSize, unsigned long recordNum) {
	if ((n == NULL) || (f == NULL) || (recordSize == 0)) {
		return EINVAL;
	}
	if (fseek(f, recordNum*(recordSize+sizeof(long)), SEEK_SET) != 0) {
		return errno;
	}
	unsigned long recNum;
	if (fread(&recNum, sizeof(long), 1, f) != 1) {
		return ENODATA;
	}
	assert (recNum == recordNum);
	if (fread(n, recordSize, 1, f) != 1) {
		return ENODATA;
	}
	return 0;
}
 	
int writeDisk(void* n, FILE* f, size_t recordSize, unsigned long recordNum) {
	if ((n == NULL) || (f == NULL) || (recordSize == 0)) {
		return EINVAL;
	}
	if (fseek(f, recordNum*(recordSize+sizeof(long)), SEEK_SET) != 0) {
		return errno;
	}
	if (fwrite(&recordNum, sizeof(long), 1, f) != 1) {
		return ENODATA;
	}
	if (fwrite(n, recordSize, 1, f) != 1) {
		return ENODATA;
	}
	return 0;
}

int readMem(void* n, void* addr, size_t recordSize, unsigned long recordNum) {
	if ((n == NULL) || (addr == NULL) || (recordSize == 0)) {
		return EINVAL;
	}
	void* mem = addr+recordNum*(recordSize+sizeof(long));	
	unsigned long recNum;
	memcpy(&recNum, mem, sizeof(long));	
	assert(recNum == recordNum);
	memcpy(n, mem+sizeof(long), recordSize);	
	return 0;
}

int writeMem(void* n, void* addr, size_t recordSize, unsigned long recordNum) {
	if ((n == NULL) || (addr == NULL) || (recordSize == 0)) {
		return EINVAL;
	}
	void* mem = addr+recordNum*(recordSize+sizeof(long));	
	memcpy(mem, &recordNum, sizeof(long));	
	memcpy(mem + sizeof(long), n, recordSize);	
	return 0;
}

void* mmapFile(FILE* f, off_t offset, size_t length) {
	if ((f == NULL) || (length == NULL)) {
		return NULL;
	}
 	void* addr = mmap(NULL, length, PROT_READ|PROT_WRITE, MAP_PRIVATE, fileno(f), offset);
	if (addr == NULL) {
		perror("mmapFile");
	}
	return addr;	
}

int munmapFile(void* addr, size_t length) {
 	int ret = munmap(addr, length);
	if (ret != 0) {
		perror("munmapFile");
	}
	return ret;	
}

