#include "estadoPronto.h"

// inicializando lista encadeada
void inicializaListaEP(listaEP *lista)
{
    lista->apPrimeiro = (elementoEP *)malloc(sizeof(elementoEP));
    lista->apUltimo = lista->apPrimeiro;
    lista->apPrimeiro->apProx = NULL;
}

// verificando se a lista possui itens
int lEhVaziaEP(listaEP *lista)
{
    if (lista->apPrimeiro == lista->apUltimo)
        return 1;
    else
        return 0;
}

// inserindo item ordenado por prioridade
void insereItemOrdenadoEP(listaEP *lista, int item, int prioridade)
{
    elementoEP *atual, *approximo;
    atual = lista->apPrimeiro;
    // encontrando posição do item na lista por prioridade
    while (atual->apProx != NULL && atual->apProx->prioridadeProcesso <= prioridade)
    {
        atual = atual->apProx;
    }
    // caso o item seja maior que todos os atuais da lista, é inserido no final
    if (atual == lista->apUltimo)
    {
        lista->apUltimo->apProx = (elementoEP *)malloc(sizeof(elementoEP));
        lista->apUltimo = lista->apUltimo->apProx;
        lista->apUltimo->idProcesso = item;
        lista->apUltimo->prioridadeProcesso = prioridade;
        lista->apUltimo->apProx = NULL;
    }
    // senão é inserido na posição adequada a sua prioridade
    else
    {
        approximo = atual->apProx;
        atual->apProx = (elementoEP *)malloc(sizeof(elementoEP));
        atual = atual->apProx;
        atual->apProx = approximo;
        atual->idProcesso = item;
        atual->prioridadeProcesso = prioridade;
    }
}

// inserindo o item na primeira posição segundo o algoritmo para fila
void insereItememFilaEP(listaEP *lista, int item, int prioridade)
{
    lista->apUltimo->apProx = (elementoEP *)malloc(sizeof(elementoEP));
    lista->apUltimo = lista->apUltimo->apProx;
    lista->apUltimo->idProcesso = item;
    lista->apUltimo->prioridadeProcesso = prioridade;
    lista->apUltimo->apProx = NULL;
}

// removendo o primeiro item da fila
int removeItemEP(listaEP *lista, int *item)
{
    elementoEP *apAux;
    if (lEhVaziaEP(lista))
        return -1;
    *item = lista->apPrimeiro->apProx->idProcesso;
    apAux = lista->apPrimeiro;
    lista->apPrimeiro = lista->apPrimeiro->apProx;
    return 1;
}

// imprimindo os itens da fila um a um
int imprimeItensEP(listaEP *lista)
{
    elementoEP *apAux;
    apAux = lista->apPrimeiro->apProx;
    while (apAux != NULL)
    {
        printf("%d", apAux->idProcesso);
        apAux = apAux->apProx;
    }
}

// atualizando os indices dos itens da fila
void atualizaItensEP(listaEP *lista, int item)
{
    elementoEP *apAux;
    apAux = lista->apPrimeiro->apProx;
    while (apAux != NULL)
    {
        if (apAux->idProcesso > item)
            apAux->idProcesso -= 1;
        apAux = apAux->apProx;
    }
}