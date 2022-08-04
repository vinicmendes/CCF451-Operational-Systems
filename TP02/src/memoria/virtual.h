#ifndef VIRTUAL_H
#define VIRTUAL_H

#include "alocador.h"

#define TAMANHO_PAGINA      8

struct dados_processo;

typedef struct {
    alocador_t fisica, disco;
    struct dados_processo *lista;
} gerenciador_virtual_t;


void inicializa_gerenciador_virtual(gerenciador_virtual_t *gv, alocador_t fisica, alocador_t disco);

void aloca_memoria_virtual(gerenciador_virtual_t *gv, int pid, int tamanho);
int* acessa_memoria_virtual(gerenciador_virtual_t *gv, int pid, int posicao);
void desaloca_memoria_virtual(gerenciador_virtual_t *gv, int pid);
void mostra_memoriaVirtual(gerenciador_virtual_t *gv);
void mostrarVariaveisProcesso(gerenciador_virtual_t *gv,int pid,int tammem);

#endif