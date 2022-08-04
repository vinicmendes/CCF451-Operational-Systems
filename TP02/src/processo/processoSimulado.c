#include "processoSimulado.h"

#define MAGENTA "\e[0;33m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\e[0;33m"
#define BLUE "\e[0;36m"
#define RESET "\x1b[0m"

void inicializaProcessoSimulado(processoSimulado *processo, int id, int idPrincipal,
                                int contadorPrograma, int prioridade, int estado, int *memoria, int tempoInicio, int tempoCPU, instrucao *instrucao, int tammem)
{
    processo->id = id;
    processo->idPrincipal = idPrincipal;
    processo->contadorPrograma = contadorPrograma;
    processo->prioridade = prioridade;
    processo->estado = estado;
    processo->memoria = memoria;
    processo->tammem = tammem;
    processo->tempoInicio = tempoInicio;
    processo->tempoCPU = tempoCPU;
    processo->programa = instrucao;
    processo->tempoAtual = 0;
}

char executaInstrucao(processoSimulado *processo, alocador_t *alocador, int tecnica, int memvirtual, gerenciador_virtual_t *gerenciadorVirtual)
{
    instrucao instrucao = processo->programa[processo->contadorPrograma];
    fprintf(stderr, "processoSimulado.c - executa instrucao - contador de programa == %d id instrucao == %c. Processo que realizou a instrucao == %d\n", processo->contadorPrograma, instrucao.id, processo->id);
    switch (instrucao.id)
    {
    case 'N':
        instrucaoN(processo, instrucao, alocador, tecnica, memvirtual, gerenciadorVirtual);
        if (processo->memoria == NULL && memvirtual == 0)
        {
            printf(BLUE "Alocação negada!\n" RESET);
            fprintf(stderr, "Erro ao alocar memoria para o processo %d -- Processo adicionado na lista de bloqueados por memoria", processo->id);
            if (processo->prioridade > 0)
                processo->prioridade--;
            alocador->qtalocsnegadas++;
            return 'M';
        }
        alocador->qtalocs++;
        processo->contadorPrograma++;
        break;
    case 'D':
        instrucaoD(processo, instrucao, memvirtual, gerenciadorVirtual);
        processo->contadorPrograma++;
        break;
    case 'V':
        instrucaoV(processo, instrucao, memvirtual, gerenciadorVirtual);
        processo->contadorPrograma++;
        break;
    case 'A':
        instrucaoA(processo, instrucao, memvirtual, gerenciadorVirtual);
        processo->contadorPrograma++;
        break;
    case 'S':
        instrucaoS(processo, instrucao, memvirtual, gerenciadorVirtual);
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
        instrucaoR(processo, instrucao, gerenciadorVirtual);
        break;
    default:
        break;
    }
    return ' ';
}

void instrucaoN(processoSimulado *processo, instrucao instrucao, alocador_t *alocador, int tecnica, int memvirtual, gerenciador_virtual_t *gerenciadorVirtual)
{
    if (memvirtual == 1)
    {
        aloca_memoria_virtual(gerenciadorVirtual, processo->id, instrucao.var1);
    }
    else
    {
        if (tecnica == 1)
        {
            processo->memoria = aloca_memoria_simulada(alocador, instrucao.var1, first_fit);
        }
        else if (tecnica == 2)
        {
            processo->memoria = aloca_memoria_simulada(alocador, instrucao.var1, next_fit);
        }
        else if (tecnica == 3)
        {
            processo->memoria = aloca_memoria_simulada(alocador, instrucao.var1, best_fit);
        }
        else if (tecnica == 4)
        {
            processo->memoria = aloca_memoria_simulada(alocador, instrucao.var1, worst_fit);
        }
    }

    printf(RED "Alocando processo: %d\n" RESET, processo->id);
    processo->tammem = instrucao.var1;
}

void instrucaoD(processoSimulado *processo, instrucao instrucao, int memvirtual, gerenciador_virtual_t *gerenciadorVirtual)
{
    if (memvirtual != 0)
    {
        int *posicao = acessa_memoria_virtual(gerenciadorVirtual, processo->id, instrucao.var1);
        *posicao = 0;
    }
    else
        processo->memoria[instrucao.var1] = 0;
}

void instrucaoV(processoSimulado *processo, instrucao instrucao, int memvirtual, gerenciador_virtual_t *gerenciadorVirtual)
{
    if (memvirtual != 0)
    {
        int *posicao = acessa_memoria_virtual(gerenciadorVirtual, processo->id, instrucao.var1);
        *posicao = instrucao.var2;
    }
    else
        processo->memoria[instrucao.var1] = instrucao.var2;
}

void instrucaoA(processoSimulado *processo, instrucao instrucao, int memvirtual, gerenciador_virtual_t *gerenciadorVirtual)
{
    if (memvirtual != 0)
    {
        int *posicao = acessa_memoria_virtual(gerenciadorVirtual, processo->id, instrucao.var1);
        *posicao += instrucao.var2;
    }
    else
        processo->memoria[instrucao.var1] += instrucao.var2;
}

void instrucaoS(processoSimulado *processo, instrucao instrucao, int memvirtual, gerenciador_virtual_t *gerenciadorVirtual)
{
    if (memvirtual != 0)
    {
        int *posicao = acessa_memoria_virtual(gerenciadorVirtual, processo->id, instrucao.var1);
        *posicao -= instrucao.var2;
    }
    else
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

void instrucaoR(processoSimulado *processo, instrucao instrucao, gerenciador_virtual_t *gerenciadorVirtual)
{
    if (gerenciadorVirtual)
    {
        desaloca_memoria_virtual(gerenciadorVirtual, processo->id);
        processo->tammem = 0;
    }
    char caminho[100] = "files/";
    strcat(caminho, instrucao.arqv);
    fprintf(stderr, "processosimulado.c - instrucao R - nome arq filho == %s\n", caminho);
    leArquivoInstrucao(&processo->programa, caminho);
    processo->contadorPrograma = 0;
}

void calculaTempo(processoSimulado *processo)
{
    processo->tempoAtual++;
}
void mostrarRelatorioProcesso(processoSimulado *processo, gerenciador_virtual_t *gerenciadorVitual, int memvirtual)
{
    int tammem = 0;
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
    case 4:
    {
        printf("Bloqueado por memoria\n");
        break;
    }
    }
    printf("Tempo de inicio: %d\n", processo->tempoInicio);
    printf("Tempo em processamento: %d\n", processo->tempoCPU + processo->tempoAtual);
    printf("-----------------------------------------------------------------\n\n");
    printf("Memoria do Processo: %d\n", processo->id);
    if (tammem == 0)
    {
        if (processo->memoria != NULL && processo->tammem > 0)
        {
            for (int i = 0; i < processo->tammem; i++)
            {
                printf("Variavel %d\n", i);
                printf("%d\n", processo->memoria[i]);
            }
        }
        else
        {
            printf("Memoria Vazia\n");
        }
    }
    else
        mostrarVariaveisProcesso(gerenciadorVitual, processo->id, processo->tammem);
}

void incrementaTempoCPU(processoSimulado *processo)
{
    processo->tempoCPU++;
}