#ifndef PIPE_H
#define PIPE_H

#include "fork.h"

typedef struct Pipe{
    int fd[2];
    pid_t pid;
} Pipe;

void inicializaPipe(Pipe *p);
bool verificaPipe(Pipe *p);
void escrevePipe(Pipe *p, char instrucao);
void lerPipe(Pipe *p, char **instrucao);

#endif