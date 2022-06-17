#include "estadoBloqueado.h"

//inicializando lista encadeada
void inicializaListaEB(listaEB *lista)
{
    lista->apPrimeiro = (elementoEB *)malloc(sizeof(elementoEB));
    lista->apUltimo = lista->apPrimeiro;
    lista->apPrimeiro->apProx = NULL;
}

//verificando se a lista possui itens
int lEhVaziaEB(listaEB *lista)
{
    return lista->apPrimeiro == lista->apUltimo ? 1 : 0;
}

//inserindo item ordenado por prioridade
void insereItemOrdenadoEB(listaEB *lista, int item, int prioridade)
{
    elementoEB *atual, *approximo;
    atual = lista->apPrimeiro;
    //encontrando posição do item na lista por prioridade
    while (atual->apProx != NULL && atual->apProx->prioridadeProcesso <= prioridade)
    {
        atual = atual->apProx;
    }
    //caso o item seja maior que todos os atuais da lista, é inserido no final
    if (atual == lista->apUltimo)
    {
        lista->apUltimo->apProx = (elementoEB *)malloc(sizeof(elementoEB));
        lista->apUltimo = lista->apUltimo->apProx;
        lista->apUltimo->idProcesso = item;
        lista->apUltimo->prioridadeProcesso = prioridade;
        lista->apUltimo->apProx = NULL;
    }
    //senão é inserido na posição adequada a sua prioridade
    else
    {
        approximo = atual->apProx;
        atual->apProx = (elementoEB *)malloc(sizeof(elementoEB));
        atual = atual->apProx;
        atual->apProx = approximo;
        atual->idProcesso = item;
        atual->prioridadeProcesso = prioridade;
    }
}

//inserindo o item na primeira posição segundo o algoritmo para fila
void insereItememFilaEB(listaEB *lista, int item, int prioridade)
{
    lista->apUltimo->apProx = (elementoEB *)malloc(sizeof(elementoEB));
    lista->apUltimo = lista->apUltimo->apProx;
    lista->apUltimo->idProcesso = item;
    lista->apUltimo->prioridadeProcesso = prioridade;
    lista->apUltimo->apProx = NULL;
}

//removendo o primeiro item da fila
int removeItemEB(listaEB *lista, int *item)
{
    elementoEB *apAux;
    if (lEhVazia(lista))
        return 1;
    *item = lista->apPrimeiro->apProx->idProcesso;
    apAux = lista->apPrimeiro;
    lista->apPrimeiro = lista->apPrimeiro->apProx;
    return 1;
}

//imprimindo os itens da fila um a um 
int imprimeItensEB(listaEB *lista)
{
    elementoEB *apAux;
    apAux = lista->apPrimeiro->apProx;
    while (apAux != NULL)
    {
        printf("%d", apAux->idProcesso);
        apAux = apAux->apProx;
    }
}

//atualizando os indices dos itens da fila
void atualizaItensEB(listaEB *lista, int item)
{
    elementoEB *apAux;
    apAux = lista->apPrimeiro->apProx;
    while (apAux != NULL)
    {
        if (apAux->idProcesso > item)
            apAux->idProcesso -= 1;
        apAux = apAux->apProx;
    }
}