#ifndef ALOCADOR_H
#define ALOCADOR_H

#include "memoria.h"
#include "marcador.h"

typedef int enderecoVirtual;

typedef struct
{
    marcador_t marcador;
    memoria_t memoria;
    enderecoVirtual *espacodeEnderecos;
    int *ultima_alocacao;
    int *alocacoes;
    int pos_ultima_alocacao;
    int qtfragmentos;
    int tempoaloc;
    int qtalocsnegadas;
    int qtalocs;
    int tamanho;
    int ultimoEndereco;
} alocador_t;

typedef int *(*funcao_alocacao)(alocador_t *alocador, int tam_alocar);

void inicializa_alocador(alocador_t *alocador, marcador_t marcador, memoria_t memoria, int tamanho, int MAX_PROCESSOS);
void destroi_alocador(alocador_t *alocador);

int *aloca_memoria_simulada(alocador_t *alocador, int tamanho, funcao_alocacao aloc);
void desaloca_memoria_simulada(alocador_t *alocador, int *posicao);

void exibe_memoria(alocador_t *alocador);
void calc_qtfragmentos(alocador_t *alocador);

// funções de alocação - apenas retornam a primeira posição do bloco livre
int *first_fit(alocador_t *alocador, int tam_alocar);
int *next_fit(alocador_t *alocador, int tam_alocar);
int *best_fit(alocador_t *alocador, int tam_alocar);
int *worst_fit(alocador_t *alocador, int tam_alocar);

#endif