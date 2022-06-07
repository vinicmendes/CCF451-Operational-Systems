#include <stdio.h>
#include <stdlib.h>

//estrutura que representa uma unidade da fila de estado pronto e aponta para a posição onde está a proxima unidade
typedef struct elementoEP
{
    int idProcesso;
    int prioridadeProcesso;
    elementoEP* apProx;
} elementoEP;

//estrutura da lista apontando para a primeira e para a ultima posições
typedef struct Lista
{
    elementoEP *apPrimeiro, *apUltimo;
}lista;

void inicializaListaEP(lista *lista);
int lEhVaziaEP(lista *lista);
void insereItemOrdenadoEP(lista *lista, int item, int prioridade);
void insereItememFilaEP(lista *lista, int item, int prioridade);
int removeItemEP(lista *lista, int *item);
int imprimeItensEP(lista *lista);
void atualizaItensEP(lista *lista,int item);