#include "menu.h"

void menu(Pipe *p)
{
    int op = -1;

    while (op != 0)
    {
        printf("\n\n   -- Menu de escolha de entrada --       \n");
        printf("1. Terminal       \n");
        printf("2. Arquivo pipe.txt             \n");
        printf("0. Sair                 \n");
        scanf(" %d", &op);
        fprintf(stderr, "menu.c - menu - opcao=%d\n", op);

        switch (op)
        {
        case 1:
            lerTerminal(p);
            return;
        case 2:
            if (leArquivoPipe(p,"files/pipe.txt") == 1)
                return;
            break;
        case 0:
            break;
        default:
            break;
        }
    }
}