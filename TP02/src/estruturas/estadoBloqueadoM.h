#ifndef ESTADOBLOQUEADOM_H
#define ESTADOBLOQUEADOM_H

#include <stdio.h>
#include <stdlib.h>

// estrutura que representa uma unidade da fila de estado bloqueado e aponta para a posição onde está a proxima unidade
typedef struct elementoEBM
{
    int idProcesso;
    int prioridadeProcesso;
    struct elementoEBM *apProx;
} elementoEBM;

// estrutura da lista apontando para a primeira e para a ultima posições
typedef struct
{
    elementoEBM *apPrimeiro, *apUltimo;
    int tam;
} listaEBM;

void inicializaListaEBM(listaEBM *lista);
int lEhVaziaEBM(listaEBM *lista);
void insereItememFilaEBM(listaEBM *lista, int item);
int removeItemEBM(listaEBM *lista, int *item);

#endif