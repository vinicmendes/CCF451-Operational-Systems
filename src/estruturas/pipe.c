#include "pipe.h"

//olhar : https://www.programacaoprogressiva.net/2014/09/Pipes-em-C-Comunicao-entre-Processos-IPC-Interprocess-Communication.html

void inicializaPipe(Pipe *p) {
    pipe(p->fd);
}

bool verificaPipe(Pipe *p){
    if(pipe(p->fd) == -1){
        perror(pipe);
        return false;
    }
    return true;
}

void escrevePipe(Pipe *p, char instrucao) {
    write(p->fd[1], &instrucao, 1);
    if(instrucao == 'U'){

    }
    else if(instrucao == 'L'){

    }
    else if(instrucao == 'I'){

    }
    else if(instrucao == 'M'){

    }
    else{
        printf("Instrucao invalida");
    }
}

void lerPipe(Pipe *p, char **instrucao) {
    int n;
    char buffer[1025];
    
    if ((n = read ( p->fd[0], buffer, 1024 ) ) >= 0) {
        *instrucao = (char*) malloc(sizeof(char)*n);
        buffer[n] = 0;
        *instrucao = buffer;
    } else {
        **instrucao = '0';
        perror("read");
    }
}
