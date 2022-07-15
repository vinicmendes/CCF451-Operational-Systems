#ifndef MEMORIA_H
#define MEMORIA_H

typedef int* memoria_t;

void inicializa_memoria(memoria_t *memo, int tamanho);
void destroi_memoria(memoria_t *memo);

#endif