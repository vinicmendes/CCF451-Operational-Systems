#include "tempo.h"

// inicio da contagem de tempo começando de 0
void inicializaTempo(tempo *tempo)
{
    tempo = malloc(sizeof(tempo));
    tempo->valor_tempo = 0;
}

// incremento de tempo
void atualizaTempo(tempo *tempo)
{
    tempo->valor_tempo += 1;
}

// removendo estrutura da memória
void desalocaStrTempo(tempo *tempo)
{
    free(tempo);
}