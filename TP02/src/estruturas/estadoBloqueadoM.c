#include "estadoBloqueadoM.h"


void inicializaListaEBM(listaEBM *lista){
    lista->apPrimeiro = (elementoEBM *)malloc(sizeof(elementoEBM));
    lista->apUltimo = lista->apPrimeiro;
    lista->apPrimeiro->apProx = NULL;
    lista->tam=0;
}
void insereItememFilaEBM(listaEBM *lista, int item){
    lista->apUltimo->apProx = (elementoEBM *)malloc(sizeof(elementoEBM));
    lista->apUltimo = lista->apUltimo->apProx;
    lista->apUltimo->idProcesso = item;
    lista->apUltimo->apProx = NULL;
    lista->tam +=1;
}
int lEhVaziaEBM(listaEBM *lista)
{
    if (lista->apPrimeiro == lista->apUltimo)
        return 1;
    else
        return 0;
}
int removeItemEBM(listaEBM *lista, int *item){
    elementoEBM *apAux;
    if (lEhVaziaEBM(lista))
        return -1;
    *item = lista->apPrimeiro->apProx->idProcesso;
    apAux = lista->apPrimeiro;
    lista->apPrimeiro = lista->apPrimeiro->apProx;
    lista->tam--;
    free(apAux);
    return 1;
}