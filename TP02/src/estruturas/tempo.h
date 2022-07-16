#ifndef TEMPO_H
#define TEMPO_H

#include <stdio.h>
#include <stdlib.h>

// estrutura para contar uma unidade de tempo
typedef struct Tempo
{
    int valor_tempo;
} tempo;

void inicializaTempo(tempo *tempo);
void atualizaTempo(tempo *tempo);
void desalocaStrTempo(tempo *tempo);

#endif