
#ifndef __FILEIO_H
#define __FILEIO_H

#include <stdio.h>

int readDisk(void* n, FILE* f, size_t recordSize, unsigned long recordNum); 
 	
int writeDisk(void* n, FILE* f, size_t recordSize, unsigned long recordNum); 

int readMem(void* n, void* addr, size_t recordSize, unsigned long recordNum); 

int writeMem(void* n, void* addr, size_t recordSize, unsigned long recordNum);

void* mmapFile(FILE* f, off_t offset, size_t length);

int munmapFile(void* addr, size_t length); 

#endif
