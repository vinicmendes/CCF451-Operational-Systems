#ifndef PROCESSOCONTROLE_H
#define PROCESSOCONTROLE_H

#include "../estruturas/estadoPronto.h"
#include "../estruturas/estadoBloqueado.h"
#include "../estruturas/estadoBloqueadoM.h"
#include "../estruturas/cpu.h"
#include "../estruturas/leitor.h"
#include "../estruturas/pipe.h"
#include "../memoria/alocador.h"
#include "../memoria/marcador.h"
#include "../memoria/memoria.h"
#include "../memoria/virtual.h"
#include "processoSimulado.h"

typedef struct
{
    int unidadeTempo;
    cpu cpu;
    processoSimulado *tabelaDeProcessos;
    listaEP estadoPronto;
    listaEB estadoBloqueado;
    listaEBM estadoBloqueadoM;
    int ultimoindice;
    int tipoEscalonamento;
    int tipoTecMemoria;
    int memvirtual;
} processoControle;

void inicializaProcessoC(processoControle *gerenciador, int tipo, int tecmemoria, int memvirtual);
void executaProcessoC(processoControle *gerenciador, Pipe *p);
void executarProcessoSimulado(processoControle *gerenciador, char *instrucaoPipe, int tam, alocador_t *alocador,
                              memoria_t *memoria, marcador_t *marcador, alocador_t *alocadorv, memoria_t *memoriav, marcador_t *marcadorv, gerenciador_virtual_t *gerenciadorVirtual);
void comandoL(processoControle *gerenciador);
void comandoB(processoControle *gerenciador);
int trocaContexto(processoControle *gerenciador, alocador_t *alocador);
void escalonarTempo(processoControle *gerenciador, alocador_t *alocador);
void escalonarProcessos(processoControle *gerenciador, alocador_t *alocador);
void processoImpressao(processoControle *gerenciador, alocador_t *alocador, gerenciador_virtual_t *gerenciadorVirtual);
void insereProgramaNoProcessoSimulado(instrucao *instProcess, instrucao *ins);
void retiraProcessoTabelaProcessos(processoControle *gerenciador, int indice, alocador_t *alocador, gerenciador_virtual_t *gerenciadorVirtual);
int encontraIndiceTP(processoControle *gerenciador, int indice);

#endif