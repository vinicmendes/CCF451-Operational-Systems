#ifndef MARCADOR_H
#define MARCADOR_H

typedef unsigned char * marcador_t;

void inicializa_marcador(marcador_t *marcador, int tamanho);
void destroi_marcador(marcador_t *marcador);

void marcar_posicoes(marcador_t *marcador, int inicio, int fim);
void desmarcar_posicoes(marcador_t *marcador, int inicio, int fim);

#endif