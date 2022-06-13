#ifndef PIPE_H
#define PIPE_H
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

typedef struct Pipe{
    int fd[2];
    pid_t pid;
} Pipe;

void inicializaPipe(Pipe *p);
void escrevePipe(Pipe *p, char instrucao);
void lerPipe(Pipe *p, char **instrucao);

#endif