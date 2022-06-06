#include "tempo.h"

void inicializatempo(tempo *tempo){
    tempo = malloc(sizeof(tempo));
    tempo->valor_tempo = 0;
}

void atualizatempo(tempo *tempo){
    tempo->valor_tempo+=1;
}
void desalocaStrtempo(tempo *tempo){
    free(tempo);
}