#include "cpu.h"

//inicializando a cpu com o tempo começando de 0
void inicializaCpu(cpu *cpu)
{
    cpu->unidTempo = 0;
}

//inserindo o processo na cpu
void insereProcesso(cpu *cpu, processoSimulado p)
{
    cpu->procexec = p;
}

//executando o processo recebido
void executaProcesso(cpu *cpu)
{
    cpu->unidTempo++;
    executaInstrucao(&cpu->procexec);
    incrementaTempo(&cpu->procexec);
}

//parando o processo p
void pararProcesso(cpu *cpu, processoSimulado *p)
{
    cpu->procexec.tempoCPU += cpu->procexec.tempoAtual;
    cpu->procexec.tempoAtual = 0;
    cpu->procexec.estado = 0;
    *p = cpu->procexec;
    cpu->procexec.id = -1;
}

//atlerando o contador de programa do processo em execução
void alterarContadorPrograma(cpu *cpu){
    cpu->procexec.contadorPrograma += cpu->procexec.programa[*cpu->procexec.contadorPrograma].var1 + 1;
}

void mostrarProcessoCpu(cpu *cpu){
    if(cpu->procexec.id == -1){
        printf("\nCPU vazia!\n");
        printf("Tempo total: %d\n", cpu->unidTempo);
        return;
    }
    printf("\nRelatório do processo atualmente em execução\n");
    printf("Id Processo: %d\n", cpu->procexec.id);
    printf("Prioridade: %d\n", cpu->procexec.prioridade);
    printf("Id processo pai: %d\n", cpu->procexec.contadorPrograma);
    printf("Tempo de inicio: %d\n", cpu->procexec.tempoInicio);
    printf("Tempo em cpu: %d\n", cpu->procexec.tempoCPU + cpu->procexec.tempoAtual);
    printf("Tempo total: %d\n", cpu->unidTempo);
}