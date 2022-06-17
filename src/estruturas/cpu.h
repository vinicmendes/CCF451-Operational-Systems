#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include <stdlib.h>
#include "processoSimulado.h"

//estrutura cpu contendo o processo em execução e o contador de unidade de tempo
typedef struct Cpu
{
    processoSimulado procexec;
    int unidTempo;
} cpu;

void inicializaCpu(cpu *cpu);
void insereProcesso(cpu *cpu, processoSimulado p);
void executaProcesso(cpu *cpu);
void pararProcesso(cpu *cpu, processoSimulado *p);
void comandoU(cpu *cpu);
void mostrarProcesso(cpu *cpu);
void alterarContadorPrograma(cpu *cpu);
void mostrarProcessoCpu(cpu *cpu);

#endif