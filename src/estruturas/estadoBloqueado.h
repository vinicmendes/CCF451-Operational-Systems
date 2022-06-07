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
} lista;

void inicializaListaEB(lista *lista);
int lEhVaziaEB(lista *lista);
void insereItemOrdenadoEB(lista *lista, int item, int prioridade);
void insereItememFilaEB(lista *lista, int item, int prioridade);
int removeItemEB(lista *lista, int *item);
int imprimeItensEB(lista *lista);
void atualizaItensEB(lista *lista, int item)