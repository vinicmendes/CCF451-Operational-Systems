#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "processo/processoControle.h"
#include "menu.h"
#include <stdio.h>

int main()
{
    pid_t pid;
    Pipe p;
    processoControle gerenciador;
    int tipo = 0;
    inicializaPipe(&p);

    // define o tipo de escalonamento aqui
     while (tipo != 1 && tipo != 2)
    {
        printf("\nQual o tipo de escalonamento você deseja?\n");
        printf("1. Escalonamento por Chaveamento Circular\n");
        printf("2. Escalonamento por Filas ordenadas por prioridades\n");
        scanf("%d", &tipo);
    }
    

    pid=fork();
    if (pid == 0)
    {
        inicializaProcessoC(&gerenciador,tipo);
        executaProcessoC(&gerenciador, &p);
    }
    else
    {
        menu(&p);     
        wait(NULL);
    }

    return 0;
}