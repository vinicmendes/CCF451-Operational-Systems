#ifndef INSTRUCAO_H
#define INSTRUCAO_H

typedef struct Instrucao{
    char id; // Id da instrução. Ex: N, D, V, A, etc.
    int var1;
    int var2;
    char arqv[20];
} instrucao;

#endif
