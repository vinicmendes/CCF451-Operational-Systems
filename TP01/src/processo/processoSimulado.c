#include "processoSimulado.h"

#define MAGENTA "\e[0;33m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\e[0;33m"
#define BLUE "\e[0;36m"
#define RESET "\x1b[0m"

void inicializaProcessoSimulado(processoSimulado *processo, int id, int idPrincipal,
                                int contadorPrograma, int prioridade, int estado, int *memoria, int tempoInicio, int tempoCPU, instrucao *instrucao)
{
    processo->id = id;
    processo->idPrincipal = idPrincipal;
    processo->contadorPrograma = contadorPrograma;
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
    fprintf(stderr, "processoSimulado.c - executa instrucao - contador de programa == %d id instrucao == %c. Processo que realizou a instrucao == %d\n", processo->contadorPrograma, instrucao.id, processo->id);
    switch (instrucao.id)
    {
    case 'N':
        instrucaoN(processo, instrucao);
        processo->contadorPrograma++;
        break;
    case 'D':
        instrucaoD(processo, instrucao);
        processo->contadorPrograma++;
        break;
    case 'V':
        instrucaoV(processo, instrucao);
        processo->contadorPrograma++;
        break;
    case 'A':
        instrucaoA(processo, instrucao);
        processo->contadorPrograma++;
        break;
    case 'S':
        instrucaoS(processo, instrucao);
        processo->contadorPrograma++;
        break;
    case 'B':
        instrucaoB(processo);
        processo->contadorPrograma++;
        return 'B';
    case 'T':
        instrucaoT(processo);
        processo->contadorPrograma++;
        return 'T';
    case 'F':
        // Não alterar contador de programa aqui
        return 'F';
    case 'R':
        instrucaoR(processo, instrucao);
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
    char caminho[100] = "files/";
    free(processo->programa);
    strcat(caminho, instrucao.arqv);
    fprintf(stderr, "processosimulado.c - instrucao R - nome arq filho == %s\n", caminho);
    leArquivoInstrucao(&processo->programa, caminho);
    processo->contadorPrograma = 0;
}

void calculaTempo(processoSimulado *processo)
{
    processo->tempoAtual++;
}
void mostrarRelatorioProcesso(processoSimulado *processo)
{
    printf(BLUE "\n----- Relatorio de processo -----\n" RESET);
    printf("PID: %d\n", processo->id);
    printf("Prioridade: %d\n", processo->prioridade);
    printf("ID Processo pai: %d\n", processo->idPrincipal);
    printf("Contador de programa: %d\n", processo->contadorPrograma);

    printf("Estado: ");
    switch (processo->estado)
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
    printf("Tempo de inicio: %d\n", processo->tempoInicio);
    printf("Tempo em processamento: %d\n", processo->tempoCPU + processo->tempoAtual);
    printf("-----------------------------------------------------------------\n\n");
}

void incrementaTempoCPU(processoSimulado *processo)
{
    processo->tempoCPU++;
}