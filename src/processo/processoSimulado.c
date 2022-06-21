#include "processoSimulado.h"

void inicializaProcessoSimulado(processoSimulado *processo, int id, int idPrincipal,
                                int contadorPrograma, int prioridade, int estado, int *memoria, int tempoInicio, int tempoCPU, instrucao *instrucao)
{
    processo->id = id;
    processo->idPrincipal = idPrincipal;
    processo->contadorPrograma = 0;
    processo->prioridade = prioridade;
    processo->estado = estado;
    processo->memoria = memoria;
    processo->tempoInicio = tempoInicio;
    processo->tempoCPU = tempoCPU;
    processo->programa = instrucao;
}

char executaInstrucao(processoSimulado *processo)
{
    instrucao instrucao = processo->programa[processo->contadorPrograma];
    printf("processoSimulado.c ---- executa instrucao --- contador de programa == %d id instrucao == %c.\n",processo->contadorPrograma,instrucao.id);
    switch (instrucao.id)
    {
    case 'N':
        printf("processosimulado.c --- executa instrucao ---- fez instrucao N contador de programa == %d\n",processo->contadorPrograma);
        instrucaoN(processo, instrucao);
        processo->contadorPrograma++;
        break;
    case 'D':
        printf("processosimulado.c --- executa instrucao ---- fez instrucao D\n");
        instrucaoD(processo, instrucao);
        processo->contadorPrograma++;
        break;
    case 'V':
        printf("processosimulado.c --- executa instrucao ---- fez instrucao V\n");
        instrucaoV(processo, instrucao);
        processo->contadorPrograma++;
        break;
    case 'A':
        printf("processosimulado.c --- executa instrucao ---- fez instrucao A\n");
        instrucaoA(processo, instrucao);
        processo->contadorPrograma++;
        break;
    case 'S':
        printf("processosimulado.c --- executa instrucao ---- fez instrucao S\n");
        instrucaoS(processo, instrucao);
        processo->contadorPrograma++;
        break;
    case 'B':
        printf("processosimulado.c --- executa instrucao ---- fez instrucao B\n");
        instrucaoB(processo);
        processo->contadorPrograma++;
        return 'B';
    case 'T':
        printf("processosimulado.c --- executa instrucao ---- fez instrucao T\n");
        instrucaoT(processo);
        processo->contadorPrograma++;
        return 'T';
    case 'F':
        printf("processosimulado.c --- executa instrucao ---- fez instrucao F\n");
        processo->contadorPrograma += 1 + instrucao.var1;
        return 'F';
    case 'R':
        printf("processosimulado.c --- executa instrucao ---- fez instrucao R\n");
        instrucaoR(processo, instrucao);
        processo->contadorPrograma++;
        break;
    default:
        break;
    }
    return ' ';
}

void instrucaoN(processoSimulado *processo, instrucao instrucao)
{
    processo->memoria = (int *)malloc(sizeof(int) * instrucao.var1);
}

void instrucaoD(processoSimulado *processo, instrucao instrucao)
{
    processo->memoria[instrucao.var1] = 0;
}

void instrucaoV(processoSimulado *processo, instrucao instrucao)
{
    processo->memoria[instrucao.var1] = instrucao.var2;
}

void instrucaoA(processoSimulado *processo, instrucao instrucao)
{
    processo->memoria[instrucao.var1] += instrucao.var2;
}

void instrucaoS(processoSimulado *processo, instrucao instrucao)
{
    processo->memoria[instrucao.var1] -= instrucao.var2;
}

void instrucaoB(processoSimulado *processo)
{
    processo->estado = 2;
}

void instrucaoT(processoSimulado *processo)
{
    processo->estado = 3;
}

void instrucaoR(processoSimulado *processo, instrucao instrucao)
{
    free(processo->programa);
    lerArquivoProcessoS(&processo->programa,instrucao.arqv);
    processo->contadorPrograma = 0;
}

void calculaTempo(processoSimulado *processo)
{
    processo->tempoAtual++;
}
void mostrarRelatorioProcesso(processoSimulado *processo)
{
    printf("----- Relatorio Processo -----\n");
    printf("PID: %d\n", processo->id);
    printf("Prioridade: %d\n", processo->prioridade);
    printf("ID Processo pai: %d\n", processo->idPrincipal);
    printf("Contador de programa: %d\n", processo->contadorPrograma);

    printf("Estado: ");
    switch (processo->estado)
    {
    case 0:
    {
        printf("Pronto\n");
        break;
    }

    case 1:
    {
        printf("Em execução\n");
        break;
    }

    case 2:
    {
        printf("Bloqueado\n");
        break;
    }

    case 3:
    {
        printf("Morto\n");
        break;
    }
    }
    printf("Tempo de inicio: %d\n", processo->tempoInicio);
    printf("Tempo em proc: %d\n", processo->tempoCPU + processo->tempoAtual);
    printf("-------------------------------\n\n");
}

void incrementaTempoCPU(processoSimulado *processo)
{
    processo->tempoCPU++;
}