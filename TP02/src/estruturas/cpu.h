#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include <stdlib.h>
#include "tempo.h"
#include "../processo/processoSimulado.h"
#include "../memoria/alocador.h"
#include "../memoria/marcador.h"
#include "../memoria/memoria.h"

// estrutura cpu contendo o processo em execução e o contador de unidade de tempo
typedef struct Cpu
{
    processoSimulado procexec;
    int unidTempo;
    int tempoProcessoAtual;
    int qtdprocessos;
} cpu;

void inicializaCpu(cpu *cpu);
void insereProcesso(cpu *cpu, processoSimulado p);
char executaProcesso(cpu *cpu, alocador_t* alocador,int tecnica);
void pararProcesso(cpu *cpu, processoSimulado *p,int estado);
void comandoU(cpu *cpu);
void alterarContadorPrograma(cpu *cpu);
void mostrarProcessoCpu(cpu *cpu);

#endif