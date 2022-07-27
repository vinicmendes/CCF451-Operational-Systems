#include "virtual.h"
#include <stdlib.h>
#include <stdio.h>

#define FISICA  0
#define DISCO   1
#define MAGENTA "\e[0;33m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\e[0;33m"
#define BLUE "\e[0;36m"
#define RESET "\x1b[0m"

struct dados_processo {
    int id, bloco, *mem, f_d;

    struct dados_processo *proximo;
};

typedef struct dados_processo * lista_t;

void adiciona_lista(lista_t *lista, int id, int bloco, int *mem, int f_d) {
    lista_t no = (lista_t) malloc(sizeof(struct dados_processo));
    no->id = id;
    no->bloco = bloco;
    no->mem = mem;
    no->f_d = f_d;
    no->proximo = *lista;
    (*lista) = no;
}

void remove_processo_lista(lista_t *lista, int id) {
    lista_t ptr = *lista, prev = NULL;
    while(ptr != NULL) {
        if(ptr->id == id) {
            if(prev == NULL) {
                *lista = ptr->proximo;
                free(ptr);
                ptr = *lista;
            } else {
                prev->proximo = ptr->proximo;
                free(ptr);
                ptr = prev->proximo;
            }
        } else {
            prev = ptr;
            ptr = ptr->proximo;
        }
    }
}

void remove_pagina_lista(lista_t *lista, int id, int bloco) {
    lista_t ptr = *lista, prev = NULL;
    while(ptr != NULL) {
        if(ptr->id == id && ptr->bloco == bloco) {
            if(prev == NULL) {
                *lista = ptr->proximo;
                free(ptr);
                ptr = *lista;
            } else {
                prev->proximo = ptr->proximo;
                free(ptr);
                ptr = prev->proximo;
            }
        } else {
            prev = ptr;
            ptr = ptr->proximo;
        }
    }
}

void mostra_lista(lista_t *lista) {
    lista_t ptr = *lista;
    printf("pid \t bloco \t f_d\n");
    while(ptr != NULL) {
        printf("%d \t %d \t %d\n", ptr->id, ptr->bloco, ptr->f_d);
        ptr = ptr->proximo;
    }
}

struct dados_processo encontra_lista(lista_t *lista, int id, int bloco) {
    lista_t ptr = *lista;

    while(ptr != NULL) {
        if(ptr->id == id && ptr->bloco == bloco) return *ptr;
        ptr = ptr->proximo;
    }
}

struct dados_processo encontra_ultimo_fisica(lista_t *lista) {
    lista_t ptr = *lista;
    struct dados_processo ultimo;

    while(ptr != NULL) {
        if(ptr->f_d == FISICA) ultimo = *ptr;
        ptr = ptr->proximo;
    }

    return ultimo;
}

void move_bloco(int* destino, int* origem) {
    for(int i=0; i<TAMANHO_PAGINA; ++i) {
        destino[i] = origem[i];
    }
}

void mostra_memoriaVirtual(gerenciador_virtual_t *gv) {
    printf(GREEN"Blocos da memória física: [");
    for(int i=0; i<24; i += TAMANHO_PAGINA) {
        if(gv->fisica.marcador[i]) printf("#");
        else printf(".");
    }
    printf(GREEN"]\n"RESET);


    printf(GREEN"Blocos do disco: [");
    for(int i=0; i<256; i += TAMANHO_PAGINA) {
        if(gv->disco.marcador[i]) printf("#");
        else printf(".");
    }
    printf(GREEN"]\n"RESET);
}


void inicializa_gerenciador_virtual(gerenciador_virtual_t *gv, alocador_t fisica, alocador_t disco) {
    gv->fisica = fisica;
    gv->disco = disco;
    gv->lista  = NULL;
}

void aloca_memoria_virtual(gerenciador_virtual_t *gv, int pid, int tamanho) {
    for(int i=0; i<tamanho; i += TAMANHO_PAGINA) {
        int *pos = aloca_memoria_simulada(&gv->fisica, TAMANHO_PAGINA, first_fit);
        if(pos == NULL) {
            pos = aloca_memoria_simulada(&gv->disco, TAMANHO_PAGINA, first_fit);
            adiciona_lista(&gv->lista, pid, i/TAMANHO_PAGINA, pos, DISCO);
        } else {
            adiciona_lista(&gv->lista, pid, i/TAMANHO_PAGINA, pos, FISICA);
        }
    }
}

int* acessa_memoria_virtual(gerenciador_virtual_t *gv, int pid, int posicao) {
    int bloco = posicao / TAMANHO_PAGINA;
    int deslocamento = posicao % TAMANHO_PAGINA;
    struct dados_processo item = encontra_lista(&gv->lista, pid, bloco);
    int *pos = item.mem;
    if(item.f_d == DISCO) {
        pos = aloca_memoria_simulada(&gv->fisica, TAMANHO_PAGINA, first_fit);
        if(pos == NULL) {
            // não é possivel alocar na memoria fisica
            // tira o ultimo acessado
            struct dados_processo ultimo = encontra_ultimo_fisica(&gv->lista);
            int *disco = aloca_memoria_simulada(&gv->disco, TAMANHO_PAGINA, first_fit);
            move_bloco(disco, ultimo.mem);
            desaloca_memoria_simulada(&gv->fisica, ultimo.mem);
            remove_pagina_lista(&gv->lista, ultimo.id, ultimo.bloco);
            adiciona_lista(&gv->lista, ultimo.id, ultimo.bloco, disco, DISCO);

            // aloca novamente
            pos = aloca_memoria_simulada(&gv->fisica, TAMANHO_PAGINA, first_fit);

            // printf("Movendo um processo da memoria fisica para o disco\n");
        }
        move_bloco(pos, item.mem);
        desaloca_memoria_simulada(&gv->disco, item.mem);
        remove_pagina_lista(&gv->lista, pid, bloco);
        adiciona_lista(&gv->lista, pid, bloco, pos, FISICA);

        // printf("movendo do disco para memoria fisica\n");
    }

    return pos + deslocamento;
}

void desaloca_memoria_virtual(gerenciador_virtual_t *gv, int pid) {
    lista_t ptr = gv->lista;

    while(ptr != NULL) {
        if(ptr->id == pid) {
            if(ptr->f_d == FISICA) {
                desaloca_memoria_simulada(&gv->fisica, ptr->mem);
            } else {
                desaloca_memoria_simulada(&gv->disco, ptr->mem);
            }
        }
        ptr = ptr->proximo;
    }

    remove_processo_lista(&gv->lista, pid);

}
void mostrarVariaveisProcesso(gerenciador_virtual_t *gv,int pid,int tammem){
    for(int i=0;i<tammem;++i){
        struct dados_processo item = encontra_lista(&gv->lista,pid,i/TAMANHO_PAGINA);
        printf("Variavel %d: %d\n",i,item.mem[i%TAMANHO_PAGINA]);
    }
}