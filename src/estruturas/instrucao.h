#ifndef INSTRUCAO_H
#define INSTRUCAO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Instrucao{
    char id; // Id da instrução. Ex: N, D, V, A, etc.
    int var1;
    int var2;
    char arqv[20];
} instrucao;

#endif
