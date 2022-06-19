#include "pipe.h"


void inicializaPipe(Pipe *p) {
    pipe(p->filedes);
}

void escrevePipe(Pipe *p, char instrucao) {
    write(p->filedes[1], &instrucao, 1);
}

void lerPipe(Pipe *p, char **instrucao)
{
    int n;
    char buffer[1025];

    if ((n = read(p->filedes[0], buffer, 1024)) >= 0)
    {
        *instrucao = (char *)malloc(sizeof(char) * n);
        buffer[n] = 0;
        *instrucao = buffer;
    }
    else
    {
        **instrucao = '0';
        perror("read");
    }
}
