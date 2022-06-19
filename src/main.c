#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "processo/processoControle.h"
#include "menu.h"
#include <stdio.h>

int main()
{
    int pid;
    Pipe p;
    processoControle gerenciador;

    if ((pid = fork()) < 0)
    {
        perror("fork");
        exit(1);
    }
    if (pid > 0)
    {
        menu(&p);
        wait(NULL);
    }
    else
    {
        inicializaProcessoC(&gerenciador);
        executaProcessoC(&gerenciador, &p);
    }

    return 0;
}