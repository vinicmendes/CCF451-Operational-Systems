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
    if (pid == 0) {
        inicializaProcessoC(&gerenciador);
        executaProcessoC(&gerenciador, &p);
    }
  
    else {
        menu(&p);
        wait(NULL);
    }

    return 0;
}