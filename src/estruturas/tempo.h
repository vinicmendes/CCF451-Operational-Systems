#include <stdio.h>
#include <stdlib.h>

//estrutura para contar uma unidade de tempo
typedef struct Tempo{
    int valor_tempo;
}tempo;

void inicializatempo(tempo *tempo);
void atualizatempo(tempo *tempo);
void desalocaStrtempo(tempo *tempo);