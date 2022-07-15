#ifndef MEMORYALOCATION_H
#define MEMORYALOCATION_H
#include <stdio.h>
#include <stdlib.h>

#define MAX 100

typedef struct memory
{
    int idProcess[MAX];
} Memory;

void allocateMemory(Memory *mem, int idProcess[MAX]);

#endif