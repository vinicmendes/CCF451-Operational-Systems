#ifndef PROCESSOSIMULADO_H
#define PROCESSOSIMULADO_H

#include "../estruturas/instrucao.h"
#include "../estruturas/leitor.h"
#include "../memoria/alocador.h"
#include "../memoria/marcador.h"
#include "../memoria/memoria.h"


typedef struct ProcessoSimulado
{
    int id;                // id do processo
    int idPrincipal;       // processo que criou o processo atual
    int contadorPrograma; // contador do programa
    int prioridade;        // prioridade do processo atual
    int estado;            // estado ; 0 = pronto, 1 = em execução, 2 = bloqueado, 3 = finalizado, 4 = bloqueado por memoria
    int *memoria;          // memória do processo para as variáveis
    int tempoInicio;       // tempo de início do processo
    int tempoCPU;          // tempo de CPU do processo
    int tempoAtual;        // tempo atual do processo
    instrucao *programa;   // programa
} processoSimulado;

void inicializaProcessoSimulado(processoSimulado *processo, int id, int idPrincipal,
                                int contadorPrograma, int prioridade, int estado, int *memoria, int tempoInicio, int tempoCPU, instrucao *instrucao); // inicialização do processo
char executaInstrucao(processoSimulado *processo, alocador_t* alocador,int tecnica);                                                                                                     // executa uma instrução
void instrucaoN(processoSimulado *processo, instrucao instrucao, alocador_t* alocador,int tecnica);                                                                                      // instrução N
void instrucaoD(processoSimulado *processo, instrucao instrucao);                                                                                      // instrução D
void instrucaoV(processoSimulado *processo, instrucao instrucao);                                                                                      // instrução V
void instrucaoA(processoSimulado *processo, instrucao instrucao);                                                                                      // instrução A
void instrucaoS(processoSimulado *processo, instrucao instrucao);                                                                                      // instrução S
void instrucaoB(processoSimulado *processo);                                                                                                           // instrução B
void instrucaoT(processoSimulado *processo);                                                                                                           // instrução T
void instrucaoR(processoSimulado *processo, instrucao instrucao);                                                                                      // instrução R
void calculaTempo(processoSimulado *processo);                                                                                                         // calcula tempo de CPU atual
void mostrarRelatorioProcesso(processoSimulado *processo);                                                                                             // exibe status atual do processo
void incrementaTempoCPU(processoSimulado *processo);

#endif