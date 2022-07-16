#include "pipe.h"


void inicializaPipe(Pipe *p) {
    pipe(p->filedes);
}

void escrevePipe(Pipe *p, char instrucao) {
    write(p->filedes[1], &instrucao, 1);
}

int lerPipe(Pipe *p, char *instrucao, int size)
{
    int n;
    n = read(p->filedes[0], instrucao, size);
    fprintf(stderr, "pipe.c - pipe - n = %d\n", n);
    return n;
}
