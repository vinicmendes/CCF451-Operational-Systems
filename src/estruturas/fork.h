#ifndef FORK_H
#define FORK_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdbool.h>

#define BUFFER 256

typedef struct Fork{
    pid_t pid;
} Fork;


void inicializaFork(Fork *f);
void escreveFork(Fork *f, char instrucao);
void lerFork(Fork *f, char **instrucao);
bool verificaFork(Fork *f);

#endif // FORK_H