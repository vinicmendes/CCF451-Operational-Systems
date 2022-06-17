#ifndef ESTADOBLOQUEADO_H
#define ESTADOBLOQUEADO_H


#include <stdio.h>
#include <stdlib.h>

// estrutura que representa uma unidade da fila de estado bloqueado e aponta para a posição onde está a proxima unidade
typedef struct elementoEB
{
    int idProcesso;
    int prioridadeProcesso;
    elementoEB *apProx;
} elementoEB;

// estrutura da lista apontando para a primeira e para a ultima posições
typedef struct Lista
{
    elementoEB *apPrimeiro, *apUltimo;
} listaEB;

void inicializaListaEB(listaEB *lista);
int lEhVaziaEB(listaEB *lista);
void insereItemOrdenadoEB(listaEB *lista, int item, int prioridade);
void insereItememFilaEB(listaEB *lista, int item, int prioridade);
int removeItemEB(listaEB *lista, int *item);
int imprimeItensEB(listaEB *lista);
void atualizaItensEB(listaEB *lista, int item);


#endif