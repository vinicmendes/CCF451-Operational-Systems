#include "cpu.h"

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\e[0;33m"
#define BLUE "\e[0;34m"
#define RESET "\x1b[0m"

// inicializando a cpu com o tempo começando de 0
void inicializaCpu(cpu *cpu)
{
    cpu->unidTempo = 0;
    cpu->procexec.id = -1;
    cpu->qtdprocessos = 0;
}

// inserindo o processo na cpu
void insereProcesso(cpu *cpu, processoSimulado p)
{
    cpu->procexec = p;
    cpu->qtdprocessos++;
    cpu->tempoProcessoAtual = 0;
}

// executando o processo recebido
char executaProcesso(cpu *cpu)
{
    char retorno;
    cpu->unidTempo++;
    cpu->tempoProcessoAtual++;
    if (cpu->procexec.id == -1)
        return ' ';

    retorno = executaInstrucao(&cpu->procexec);
    incrementaTempoCPU(&cpu->procexec);
    return retorno;
}

// parando o processo p
void pararProcesso(cpu *cpu, processoSimulado *p, int estado)
{
    cpu->procexec.tempoCPU += cpu->procexec.tempoAtual;
    cpu->procexec.tempoAtual = 0;
    cpu->procexec.estado = estado;
    *p = cpu->procexec;
    cpu->procexec.id = -1;
}

// alterando o contador de programa do processo em execução
void alterarContadorPrograma(cpu *cpu)
{
    cpu->procexec.contadorPrograma += cpu->procexec.programa[cpu->procexec.contadorPrograma].var1 + 1;
}

void mostrarProcessoCpu(cpu *cpu)
{
    if (cpu->procexec.id == -1)
    {
        printf(RED"\n-----CPU vazia!-----\n"RESET);
        printf("Tempo total: %d\n", cpu->unidTempo);
        return;
    }
    printf(BLUE"\n-----Relatório do processo atualmente em execução-----\n"RESET);
    printf("PID: %d\n", cpu->procexec.id);
    printf("Prioridade: %d\n", cpu->procexec.prioridade);
    printf("Id processo pai: %d\n", cpu->procexec.idPrincipal);
    printf("Contador de programa: %d\n", cpu->procexec.contadorPrograma);
    printf("Estado: ");
    switch (cpu->procexec.estado)
    {
    case 0:
    {
        printf(YELLOW "Pronto\n" RESET);
        break;
    }

    case 1:
    {
        printf(GREEN "Em execução\n" RESET);
        break;
    }

    case 2:
    {
        printf(RED "Bloqueado\n" RESET);
        break;
    }

    case 3:
    {
        printf("Morto\n");
        break;
    }
    }
    printf("Tempo de inicio: %d\n", cpu->procexec.tempoInicio);
    printf("Tempo de cpu: %d\n", cpu->unidTempo + cpu->procexec.tempoAtual);
    printf("Tempo total da CPU: %d\n", cpu->unidTempo);
}