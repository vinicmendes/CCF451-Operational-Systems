#include "alocador.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define GREEN "\x1b[32m"
#define YELLOW "\e[0;33m"
#define BLUE "\e[0;36m"
#define RESET "\x1b[0m"

void inicializa_alocador(alocador_t *alocador, marcador_t marcador, memoria_t memoria, int tamanho, int MAX_PROCESSOS)
{
    int i;
    alocador->memoria = memoria;
    alocador->marcador = marcador;
    alocador->tamanho = tamanho;
    alocador->ultima_alocacao = NULL;
    alocador->pos_ultima_alocacao = 0;
    alocador->qtfragmentos = 1;
    alocador->tempoaloc = 0;
    alocador->qtalocs = 0;
    alocador->qtalocsnegadas = 0;
    alocador->alocacoes = (int *)malloc(tamanho * sizeof(int));
    for (i = 0; i < tamanho; ++i)
        alocador->alocacoes[i] = 0;
}

void destroi_alocador(alocador_t *alocador)
{
    free(alocador->alocacoes);
}

int *aloca_memoria_simulada(alocador_t *alocador, int tamanho, funcao_alocacao aloc)
{
    int *position = aloc(alocador, tamanho);
    int index_base = position - alocador->memoria;
    if (position)
    {
        marcar_posicoes(&alocador->marcador, index_base, index_base + tamanho - 1);
        alocador->alocacoes[index_base] = tamanho;
    }
    return position;
}

void desaloca_memoria_simulada(alocador_t *alocador, int *posicao)
{
    int index_base = posicao - alocador->memoria;
    desmarcar_posicoes(&alocador->marcador, index_base, index_base + alocador->alocacoes[index_base] - 1);
    alocador->alocacoes[index_base] = 0;
}

void calc_qtfragmentos(alocador_t *alocador)
{
    int frag = 0;
    for (int i = 0; i < alocador->tamanho; ++i)
    {
        if (alocador->marcador[i] == 0)
        {
            frag = 1;
        }
        if (alocador->marcador[i] == 1 && frag)
        {
            alocador->qtfragmentos++;
            frag = 0;
            break;
        }
    }
    if (frag)
        alocador->qtfragmentos++;
}

void exibe_memoria(alocador_t *alocador)
{
    int livres = 0;
    int ocupados = 0;
    printf("%s\n\nMemoria:%s\n", BLUE, RESET);
    printf("Numero total de fragmentos na memoria %d\n", alocador->qtfragmentos);
    printf("Numero de alocacoes: %d\n", alocador->qtalocs);
    printf("Numero medio de fragmentos: %.2lf\n", alocador->qtfragmentos / (double)alocador->qtalocs);
    printf("Tempo de alocacao: %d\n", alocador->tempoaloc);
    printf("Tempo medio de alocacao: %.2lf\n", alocador->tempoaloc / (double)alocador->qtalocs);
    printf("Percentual de negacao de requisicao de alocacao: %.2lf porcento!\n", alocador->qtalocsnegadas / (double)alocador->qtalocs * 100);
    printf(GREEN"Situacao da memoria atual: ");
    printf(GREEN "[" RESET);
    for (int i = 0; i < alocador->tamanho; i++)
    {
        if (alocador->marcador[i] == 1)
        {
            printf(GREEN "#" RESET);
            ocupados++;
        }
        else
        {
            printf(GREEN "." RESET);
            livres++;
        }
    }
    printf(GREEN "]\n\n" RESET);
    printf(GREEN "Livres: %d\n" RESET, livres);
    printf(GREEN "Ocupados: %d\n" RESET, ocupados);
    printf(GREEN "Total: %d\n" RESET, livres + ocupados);
}

// definição das funções de alocação
int *first_fit(alocador_t *alocador, int tam_alocar)
{
    int i, j;
    for (i = 0; i < alocador->tamanho; ++i)
    {
        if (alocador->marcador[i] == 1)
        {
            alocador->tempoaloc++;
            continue;
        }
        for (j = i; j < alocador->tamanho; ++j)
        {
            if (alocador->marcador[j] == 1)
            {
                alocador->tempoaloc++;
                break;
            }
            if (j - i + 1 == tam_alocar)
                return (int *)(alocador->memoria + i);
        }
        i = j;
    }
    return NULL;
}
int *next_fit(alocador_t *alocador, int tam_alocar)
{
    int i, j;
    for (i = alocador->pos_ultima_alocacao; i < alocador->tamanho; ++i)
    {
        if (alocador->marcador[i] == 1)
        {
            alocador->tempoaloc++;
            continue;
        }
        for (j = i; j < alocador->tamanho; ++j)
        {
            if (alocador->marcador[j] == 1)
            {
                alocador->tempoaloc++;
                break;
            }
            if (j - i + 1 == tam_alocar)
            {
                alocador->pos_ultima_alocacao = j + 1;
                return (int *)(alocador->memoria + i);
            }
        }
        i = j;
    }
    for (i = 0; i < alocador->pos_ultima_alocacao; ++i)
    {
        if (alocador->marcador[i] == 1)
        {
            alocador->tempoaloc++;
            continue;
        }
        for (j = i; j < alocador->tamanho; ++j)
        {
            if (alocador->marcador[j] == 1)
            {
                alocador->tempoaloc++;
                break;
            }
            if (j - i + 1 == tam_alocar)
            {
                alocador->pos_ultima_alocacao = j + 1;
                return (int *)(alocador->memoria + i);
            }
        }
        i = j;
    }
    return NULL;
}
int *best_fit(alocador_t *alocador, int tam_alocar)
{
    int i, j, best = -1, tam_best = INT32_MAX;
    for (i = 0; i < alocador->tamanho; ++i)
    {
        if (alocador->marcador[i] == 1)
        {
            alocador->tempoaloc++;
            continue;
        }
        for (j = i; j < alocador->tamanho && alocador->marcador[j] == 0; ++j)
            ;
        if (j - i >= tam_alocar && j - i < tam_best)
        {
            best = i;
            tam_best = j - i;
        }
        else
        {
            alocador->tempoaloc++;
        }
        i = j;
    }
    if (best != -1)
        return (int *)(alocador->memoria + best);
    return NULL;
}

int *worst_fit(alocador_t *alocador, int tam_alocar)
{
    int i, j, worst = -1, tam_worst = 0;
    for (i = 0; i < alocador->tamanho; ++i)
    {
        if (alocador->marcador[i] == 1)
        {
            alocador->tempoaloc++;
            continue;
        }
        for (j = i; j < alocador->tamanho && alocador->marcador[j] == 0; ++j)
            ;
        if (j - i >= tam_alocar && j - i > tam_worst)
        {
            worst = i;
            tam_worst = j - i;
        }
        else
        {
            alocador->tempoaloc++;
        }
        i = j;
    }
    if (worst != -1)
        return (int *)(alocador->memoria + worst);
    return NULL;
}