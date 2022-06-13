#include "pipe.h"

//olhar : https://www.programacaoprogressiva.net/2014/09/Pipes-em-C-Comunicao-entre-Processos-IPC-Interprocess-Communication.html

void inicializaPipe(Pipe *p) {
    pipe(p->fd);
}

void escrevePipe(Pipe *p, char instrucao) {
    write(p->fd[1], &instrucao, 1);
}

void lerPipe(Pipe *p, char **instrucao) {
    int n;
    char buffer[1025];
    
    if ((n = read ( p->fd[0], buffer, 1024 ) ) >= 0) {
        *instrucao = (char*) malloc(sizeof(char)*n);
        buffer[n] = 0;
        *instrucao = buffer;
        // printf("read %d bytes from the pipe: %s\n", n, *inst);
    } else {
        **instrucao = '0';
        perror("read");
    }
}