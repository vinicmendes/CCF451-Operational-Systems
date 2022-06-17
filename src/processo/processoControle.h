#ifndef PROCESSOCONTROLE_H
#define PROCESSOCONTROLE_H

#include "../estruturas/estadoPronto.h"
#include "../estruturas/estadoBloqueado.h"
#include "../estruturas/cpu.h"
#include "processoSimulado.h"
#include "../estruturas/pipe.h"

typedef struct {
    int unidadeTempo;
    cpu cpu;
    ProcessoSimulado *tabelaDeProcessos;
    listaEP estadoPronto;
    listaEB estadoBloqueado;
    int estadoExecucao;
    int ultimoposicao; // marca a posição seguinte à última posição preenchida do vetor tabelaDeProcessos
    int ultimoindice; // ultimo indice utilizado
    int tipoEscalonamento;
}processoControle;

void inicializaProcessoC(processoControle *gProc);

void executaGerenciador(processoControle *gProc, Pipe *p);

void executarProcessoSimulado(processoControle *gProc, char *instPipe);

void comandoL(processoControle *gProc);

void comandoB(processoControle *gProc);

int trocaContexto(processoControle *gProc);

void escalonar(processoControle *gProc); //devemos escolher o método *****************

void escalonarProcessos(processoControle *gProc);

void processoImpressao(processoControle *gProc);

void insereProgramaNoProcessoSimulado(Instrucao *instProcess, Instrucao *ins);

void retiraProcessoTabelaProcessos(processoControle *gProc, int indice);

#endif