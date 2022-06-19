#include "processo/processoControle.h"
#include "menu.h"

int main()
{

    Pipe p;
    processoControle gerenciador;

    menu(&p);
    inicializaProcessoC(&gerenciador);
    executaProcessoC(&gerenciador, &p);

    return 0;
}