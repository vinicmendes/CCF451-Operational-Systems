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
        printf("\nEscolha o metodo de escalonamento\n");
        printf("1. Escalonamento por Chaveamento Circular\n");
        printf("2. Escalonamento por prioridades\n");
        scanf("%d", &tipo);
    }
    
    //criado fork para executar 1 processo real pai para ler o menu 
    //e um processo real filho para executar o Processo Controle e os Processos Simulados
    pid=fork();
    if (pid == 0)
    {
        //processo real filho executando
        inicializaProcessoC(&gerenciador,tipo);
        executaProcessoC(&gerenciador, &p);
    }
    else
    {
        //processo real pai executando
        menu(&p);     
        wait(NULL);
    }

    return 0;
}