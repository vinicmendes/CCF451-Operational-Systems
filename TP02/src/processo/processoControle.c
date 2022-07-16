#include "processoControle.h"

#define MAX_PROCESSOS 50
#define TAMANHO_MEM 20
#define MAGENTA "\e[0;35m"
#define RED "\x1b[31m"
#define RESET "\x1b[0m"

void inicializaProcessoC(processoControle *gerenciador, int tipo, int tecmemoria)
{
    gerenciador->unidadeTempo = 0;
    inicializaCpu(&gerenciador->cpu);
    gerenciador->tabelaDeProcessos = (processoSimulado *)malloc(sizeof(processoSimulado) * MAX_PROCESSOS);
    inicializaListaEP(&gerenciador->estadoPronto);
    inicializaListaEB(&gerenciador->estadoBloqueado);
    inicializaListaEBM(&gerenciador->estadoBloqueadoM);
    gerenciador->estadoExecucao = -1;
    gerenciador->ultimaposicao = 0;
    gerenciador->ultimoindice = 0;
    gerenciador->tipoEscalonamento = tipo;
    gerenciador->tipoTecMemoria = tecmemoria;
}

void executaProcessoC(processoControle *gerenciador, Pipe *p)
{
    processoSimulado processo;
    instrucao *inst;
    int contador = 0;
    char instPipe[1025];
    int tam;
    memoria_t memoria;
    inicializa_memoria(&memoria, TAMANHO_MEM);
    marcador_t marcador;
    inicializa_marcador(&marcador, TAMANHO_MEM);
    alocador_t alocador;
    inicializa_alocador(&alocador, marcador, memoria, TAMANHO_MEM);
    printf("\nCriando Processo Gerenciador de Processos...\n");

    leArquivoInstrucao(&inst, "files/init.txt");
    inicializaProcessoSimulado(&processo, 0, -1, 0, 0, 0, alocador.memoria, 0, 0, inst);
    gerenciador->tabelaDeProcessos[gerenciador->ultimaposicao] = processo;
    inicializaCpu(&gerenciador->cpu);
    gerenciador->tabelaDeProcessos[gerenciador->ultimaposicao].estado = 1;
    insereProcesso(&gerenciador->cpu, gerenciador->tabelaDeProcessos[gerenciador->ultimaposicao]);
    gerenciador->estadoExecucao = 0;
    gerenciador->ultimaposicao++;

    while (1)
    {
        tam = lerPipe(p, instPipe, 1024);
        executarProcessoSimulado(gerenciador, instPipe, tam, &alocador, &memoria, &marcador);
    }
}
void escalona(processoControle *gerenciador)
{
    if (gerenciador->tipoEscalonamento == 1)
    {
        escalonarTempo(gerenciador);
    }
    else
    {
        escalonarProcessos(gerenciador);
    }
}

void executarProcessoSimulado(processoControle *gerenciador, char *instrucaoPipe, int tamPipe, alocador_t *alocador, memoria_t *memoria, marcador_t *marcador)
{
    int index, retorno;
    char resultado;
    processoSimulado processo;

    for (index = 0; index < tamPipe; index++)
    {
        if (instrucaoPipe[index] == 'U')
        {
            printf("\nComando U\n");
            // FIM DE UNIDADE DE TEMPO
            resultado = executaProcesso(&gerenciador->cpu, alocador, gerenciador->tipoTecMemoria);
            fprintf(stderr, "processocontrole.c - executa processo simulado - resultado de executa processo %c.\n", resultado);
            if (resultado == 'B')
            {
                // BLOQUEIA PROCESSO SIMULADO
                comandoB(gerenciador);
            }
            else if (resultado == 'M')
            {
                // BLOQUEIA PROCESSO SIMULADO POR MEMORIA
                processoSimulado p;
                int i, retorno;
                pararProcesso(&gerenciador->cpu, &p, 4);
                gerenciador->tabelaDeProcessos[gerenciador->estadoExecucao] = p;
                insereItememFilaEBM(&gerenciador->estadoBloqueadoM, gerenciador->estadoExecucao);
                gerenciador->estadoExecucao = -1;
                retorno = removeItemEP(&gerenciador->estadoPronto, &i);
                gerenciador->tabelaDeProcessos[i].estado = 1;
                if (retorno != -1)
                {
                    insereProcesso(&gerenciador->cpu, gerenciador->tabelaDeProcessos[i]);
                    gerenciador->estadoExecucao = i;
                }
            }
            else if (resultado == 'T')
            {
                int tentativa, j;
                elementoEBM *apAux;
                // TERMINA PROCESSO SIMULADO E TENTA USAR A MEMORIA PARA O ALGUM PROCESSO BLOQUEADO POR MEMORIA
                retiraProcessoTabelaProcessos(gerenciador, gerenciador->estadoExecucao, alocador);
                apAux = gerenciador->estadoBloqueadoM.apPrimeiro;
                while(apAux != gerenciador->estadoBloqueadoM.apUltimo)
                {
                    tentativa = removeItemEBM(&gerenciador->estadoBloqueadoM, &j);
                    instrucao instrucao = gerenciador->tabelaDeProcessos[j].programa[gerenciador->tabelaDeProcessos[j].contadorPrograma];
                    instrucaoN(&gerenciador->tabelaDeProcessos[j], instrucao, alocador, gerenciador->tipoTecMemoria);
                    if (gerenciador->tabelaDeProcessos[j].memoria == NULL)
                    {
                        insereItememFilaEBM(&gerenciador->estadoBloqueadoM, tentativa);
                        apAux=apAux->apProx;
                    }
                    else
                    {
                        insereItememFilaEP(&gerenciador->estadoPronto, tentativa, 0);
                    }
                }
            }
            else if (resultado == 'F')
            {
                // CRIA NOVO PROCESSO SIMULADO FILHO
                // estado ; 0 = pronto, 1 = em execução, 2 = bloquado, 3 = finalizado
                gerenciador->ultimoindice++;
                inicializaProcessoSimulado(&gerenciador->tabelaDeProcessos[gerenciador->ultimaposicao], gerenciador->ultimoindice,
                                           gerenciador->tabelaDeProcessos[gerenciador->estadoExecucao].id, gerenciador->cpu.procexec.contadorPrograma + 1,
                                           0, 0, gerenciador->cpu.procexec.memoria,
                                           gerenciador->cpu.unidTempo, 0, gerenciador->cpu.procexec.programa);
                gerenciador->cpu.procexec.contadorPrograma += 1 + gerenciador->cpu.procexec.programa[gerenciador->cpu.procexec.contadorPrograma].var1;

                fprintf(stderr, "Criando processo simulado de id %d\n", gerenciador->ultimoindice);
                if (gerenciador->tipoEscalonamento == 1)
                {
                    insereItememFilaEP(&gerenciador->estadoPronto, gerenciador->ultimaposicao, gerenciador->tabelaDeProcessos[gerenciador->ultimaposicao].prioridade);
                }
                else
                {
                    insereItemOrdenadoEP(&gerenciador->estadoPronto, gerenciador->ultimaposicao, gerenciador->tabelaDeProcessos[gerenciador->ultimaposicao].prioridade);
                }
                gerenciador->ultimaposicao++;
            }
            escalona(gerenciador);
        }
        else if (instrucaoPipe[index] == 'L')
        {
            printf("\nComando L\n");
            // Desbloqueia o primeiro processo simulado na fila bloqueada
            comandoL(gerenciador);
        }
        else if (instrucaoPipe[index] == 'I')
        {
            printf("\nComando I\n");
            // Imprime o estado atual do gerenciador
            processoImpressao(gerenciador, alocador);
        }
        else if (instrucaoPipe[index] == 'M')
        {
            printf("\nComando M\n");
            // Imprime o tempo médio do ciclo e finaliza o sistema
            printf(MAGENTA "\n Tempo medio de ciclo = %.2f\n" RESET, (float)(gerenciador->cpu.unidTempo / gerenciador->cpu.qtdprocessos));
            processoImpressao(gerenciador, alocador);
            destroi_alocador(alocador);
            destroi_marcador(marcador);
            destroi_memoria(memoria);
            exit(0);
        }
    }
}

void comandoL(processoControle *gerenciador)
{
    int indice, retorno;
    if (lEhVaziaEB(&gerenciador->estadoBloqueado))
        return;
    retorno = removeItemEB(&gerenciador->estadoBloqueado, &indice);
    if (retorno == -1)
        return;
    printf("Processo de indice %d liberado!\n", gerenciador->tabelaDeProcessos[indice].id);
    if (gerenciador->tipoEscalonamento == 1)
        insereItememFilaEP(&gerenciador->estadoPronto, indice, gerenciador->tabelaDeProcessos[indice].prioridade);
    else
        insereItemOrdenadoEP(&gerenciador->estadoPronto, indice, gerenciador->tabelaDeProcessos[indice].prioridade);
}

void comandoB(processoControle *gerenciador)
{
    processoSimulado p;
    int i, retorno;
    pararProcesso(&gerenciador->cpu, &p, 2);
    if (p.prioridade != 0)
        p.prioridade--;
    gerenciador->tabelaDeProcessos[gerenciador->estadoExecucao] = p;
    printf("Processo de id %d bloqueado! \n", gerenciador->tabelaDeProcessos[gerenciador->estadoExecucao].id);
    insereItememFilaEB(&gerenciador->estadoBloqueado, gerenciador->estadoExecucao, gerenciador->tabelaDeProcessos[gerenciador->estadoExecucao].prioridade);
    gerenciador->estadoExecucao = -1;
    retorno = removeItemEP(&gerenciador->estadoPronto, &i);
    gerenciador->tabelaDeProcessos[i].estado = 1;
    if (retorno != -1)
    {
        insereProcesso(&gerenciador->cpu, gerenciador->tabelaDeProcessos[i]);
        gerenciador->estadoExecucao = i;
    }
}

int trocaContexto(processoControle *gerenciador)
{
    printf(RED "Trocando contexto\n" RESET);
    processoSimulado p;
    int i;
    pararProcesso(&gerenciador->cpu, &p, 0);
    if (p.id != -1)
    {
        gerenciador->tabelaDeProcessos[gerenciador->estadoExecucao] = p;
        if (gerenciador->tipoEscalonamento == 1)
            insereItememFilaEP(&gerenciador->estadoPronto, gerenciador->estadoExecucao, gerenciador->tabelaDeProcessos[gerenciador->estadoExecucao].prioridade);
        else
            insereItemOrdenadoEP(&gerenciador->estadoPronto, gerenciador->estadoExecucao, gerenciador->tabelaDeProcessos[gerenciador->estadoExecucao].prioridade);
    }
    if (lEhVaziaEP(&gerenciador->estadoPronto))
    {
        gerenciador->cpu.procexec.id = -1;
        return -1;
    }
    removeItemEP(&gerenciador->estadoPronto, &i);
    gerenciador->tabelaDeProcessos[i].estado = 1;
    insereProcesso(&gerenciador->cpu, gerenciador->tabelaDeProcessos[i]);
    gerenciador->estadoExecucao = i;

    return 1;
}
void escalonarTempo(processoControle *gerenciador)
{
    if (gerenciador->cpu.tempoProcessoAtual >= 5 || gerenciador->cpu.procexec.id == -1)
        trocaContexto(gerenciador);
}
void escalonarProcessos(processoControle *gerenciador)
{
    if (gerenciador->cpu.procexec.prioridade == 0)
    {
        gerenciador->cpu.procexec.prioridade = 1;
        trocaContexto(gerenciador);
    }
    else if (gerenciador->cpu.procexec.prioridade == 1 && gerenciador->cpu.tempoProcessoAtual > 1)
    {
        gerenciador->cpu.procexec.prioridade = 2;
        trocaContexto(gerenciador);
    }
    else if (gerenciador->cpu.procexec.prioridade == 2 && gerenciador->cpu.tempoProcessoAtual > 3)
    {
        gerenciador->cpu.procexec.prioridade = 3;
        trocaContexto(gerenciador);
    }
    else if (gerenciador->cpu.procexec.prioridade == 3 && gerenciador->cpu.tempoProcessoAtual > 7)
    {
        trocaContexto(gerenciador);
    }
    else if (gerenciador->cpu.procexec.id == -1)
    {
        trocaContexto(gerenciador);
    }
}

void processoImpressao(processoControle *gerenciador, alocador_t *alocador)
{
    exibe_memoria(alocador);
    for (int i = 0; i < gerenciador->ultimaposicao; i++)
    {
        if (i == gerenciador->estadoExecucao)
            mostrarProcessoCpu(&gerenciador->cpu);
        else
            mostrarRelatorioProcesso(&gerenciador->tabelaDeProcessos[i]); // mostrar relatório processo
    }
    if (gerenciador->cpu.procexec.id == -1)
        mostrarProcessoCpu(&gerenciador->cpu);
}

void retiraProcessoTabelaProcessos(processoControle *gerenciador, int indice, alocador_t *alocador)
{
    processoSimulado processo;
    atualizaItensEB(&gerenciador->estadoBloqueado, indice);
    atualizaItensEP(&gerenciador->estadoPronto, indice);

    while (indice < MAX_PROCESSOS - 1)
    {
        gerenciador->tabelaDeProcessos[indice] = gerenciador->tabelaDeProcessos[indice + 1];
        indice++;
    }
    printf("Processo %d finalizado ... \n", gerenciador->cpu.procexec.id);
    gerenciador->cpu.procexec.id = -1;
    gerenciador->tabelaDeProcessos[MAX_PROCESSOS - 1] = processo;
    gerenciador->ultimaposicao--;
    desaloca_memoria_simulada(alocador, gerenciador->cpu.procexec.memoria);
}