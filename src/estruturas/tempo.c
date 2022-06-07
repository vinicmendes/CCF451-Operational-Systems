#include "tempo.h"

//inicio da contagem de tempo começando de 0
void inicializatempo(tempo *tempo){
    tempo = malloc(sizeof(tempo));
    tempo->valor_tempo = 0;
}

//incremento de tempo
void atualizatempo(tempo *tempo){
    tempo->valor_tempo+=1;
}

//removendo estrutura da memória
void desalocaStrtempo(tempo *tempo){
    free(tempo);
}