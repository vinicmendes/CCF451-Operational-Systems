#include "fork.h"

void inicializaFork(Fork *f){
    f->pid = 0;
}

bool verificaFork(Fork *f){
    if ((f->pid = fork()) < 0)
    {
        perror("fork");
        exit(1);
    }
    if (f->pid == 0)
    {
        //O código aqui dentro será executado no processo filho
        printf("pid do Filho: %d\n", getpid());
    }
    else
    {
        //O código neste trecho será executado no processo pai
        printf("pid do Pai: %d\n", getpid());
    }

    printf("Esta regiao sera executada por ambos processos\n\n");
    exit(0);
}