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
    processoSimulado *tabelaDeProcessos;
    listaEP estadoPronto;
    listaEB estadoBloqueado;
    int estadoExecucao;
    int ultimaposicao; 
    int ultimoindice;
    int tipoEscalonamento;
}processoControle;

void inicializaProcessoC(processoControle *gerenciador);
void executaGerenciador(processoControle *gerenciador, Pipe *p);
void executarProcessoSimulado(processoControle *gerenciador, char *instrucaoPipe);
void comandoL(processoControle *gerenciador);
void comandoB(processoControle *gerenciador);
int trocaContexto(processoControle *gerenciador);
void escalonar(processoControle *gerenciador); //devemos escolher o método *****************
void escalonarProcessos(processoControle *gerenciador);
void processoImpressao(processoControle *gerenciador);
void insereProgramaNoProcessoSimulado(instrucao *instProcess, instrucao *ins);
void retiraProcessoTabelaProcessos(processoControle *gerenciador, int indice);

#endif