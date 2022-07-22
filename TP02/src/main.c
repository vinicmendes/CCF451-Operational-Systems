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
    int tipoescalonamento = 0;
    int tipotecmemoria = 0;
    inicializaPipe(&p);

    // define o tipo de escalonamento aqui
    while (tipoescalonamento != 1 && tipoescalonamento != 2)
    {
        printf("\nEscolha o metodo de escalonamento\n");
        printf("1. Escalonamento por Chaveamento Circular\n");
        printf("2. Escalonamento por prioridades\n");
        scanf("%d", &tipoescalonamento);
    }
    
    // define o tipo de escalonamento aqui
    while (tipotecmemoria != 1 && tipotecmemoria != 2 && tipotecmemoria != 3 && tipotecmemoria != 4)
    {
        printf("\nEscolha a tecnica para alocar o processo:\n");
        printf("1. First fit\n");
        printf("2. Next fit\n");
        printf("3. Best fit\n");
        printf("4. Worst fit\n");
        scanf("%d", &tipotecmemoria);
    }

    // criado fork para executar 1 processo real pai para ler o menu
    // e um processo real filho para executar o Processo Controle e os Processos Simulados
    pid = fork();
    if (pid == 0)
    {
        // processo real filho executando
        inicializaProcessoC(&gerenciador, tipoescalonamento, tipotecmemoria);
        executaProcessoC(&gerenciador, &p);
    }
    else
    {
        // processo real pai executando
        menu(&p);
        wait(NULL);
    }

    return 0;
}