#ifndef ESTADOPRONTO_H
#define ESTADOPRONTO_H

#include <stdio.h>
#include <stdlib.h>

// estrutura que representa uma unidade da fila de estado pronto e aponta para a posição onde está a proxima unidade
typedef struct elementoEP
{
    int idProcesso;
    int prioridadeProcesso;
    struct elementoEP *apProx;
} elementoEP;

// estrutura da lista apontando para a primeira e para a ultima posições
typedef struct
{
    elementoEP *apPrimeiro, *apUltimo;
} listaEP;

void inicializaListaEP(listaEP *lista);
int lEhVaziaEP(listaEP *lista);
void insereItemOrdenadoEP(listaEP *lista, int item, int prioridade);
void insereItememFilaEP(listaEP *lista, int item, int prioridade);
int removeItemEP(listaEP *lista, int *item);
int imprimeItensEP(listaEP *lista);
void atualizaItensEP(listaEP *lista, int item);

#endif