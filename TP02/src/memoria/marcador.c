#include "marcador.h"
#include <stdlib.h>

void inicializa_marcador(marcador_t *marcador, int tamanho) {
    int i;
    (*marcador) = (unsigned char*) malloc(tamanho * sizeof(unsigned char));
    for(i=0; i<tamanho; ++i) (*marcador)[i] = 0;
}

void destroi_marcador(marcador_t *marcador) {
    free(*marcador);
}

void marcar_posicoes(marcador_t *marcador, int inicio, int fim) {
    int i;
    for(i=inicio; i<=fim; ++i) (*marcador)[i] = 1;
}

void desmarcar_posicoes(marcador_t *marcador, int inicio, int fim) {
    int i;
    for(i=inicio; i<=fim; ++i) (*marcador)[i] = 0;
}