#ifndef PIPE_H
#define PIPE_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdbool.h>

typedef struct {
    int filedes[2];
} Pipe;

void inicializaPipe(Pipe *p);
void escrevePipe(Pipe *p, char instrucao);
int lerPipe(Pipe *p, char *instrucao, int size);
bool verificaPipe(Pipe *p);

#endif