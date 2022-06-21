#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include <stdlib.h>
#include "../processo/processoSimulado.h"
#include "tempo.h"

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
char executaProcesso(cpu *cpu);
void pararProcesso(cpu *cpu, processoSimulado *p,int estado);
void comandoU(cpu *cpu);
void alterarContadorPrograma(cpu *cpu);
void mostrarProcessoCpu(cpu *cpu);

#endif