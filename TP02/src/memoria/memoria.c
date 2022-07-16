#include "memoria.h"
#include <stdlib.h>

void inicializa_memoria(memoria_t *memo, int tamanho) {
    (*memo) = (int*) malloc(tamanho * sizeof(int));
}

void destroi_memoria(memoria_t *memo) {
    free(*memo);
}