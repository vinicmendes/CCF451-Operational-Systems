#include "processoControle.h"

#define MAX_PROCESSOS 50
#define TAMANHO_MEM 24
#define TAMANHO_MEMV 2000
#define MAGENTA "\e[0;35m"
#define RED "\x1b[31m"
#define RESET "\x1b[0m"

void inicializaProcessoC(processoControle *gerenciador, int tipo, int tecmemoria, int memvirtual)
{
    gerenciador->unidadeTempo = 0;
    inicializaCpu(&gerenciador->cpu);
    gerenciador->tabelaDeProcessos = (processoSimulado *)malloc(sizeof(processoSimulado) * MAX_PROCESSOS);
    for (int i = 0; i < MAX_PROCESSOS; ++i)
    {
        gerenciador->tabelaDeProcessos[i].id = -1;
    }
    inicializaListaEP(&gerenciador->estadoPronto);
    inicializaListaEB(&gerenciador->estadoBloqueado);
    inicializaListaEBM(&gerenciador->estadoBloqueadoM);
    gerenciador->ultimoindice = 0;
    gerenciador->tipoEscalonamento = tipo;
    gerenciador->tipoTecMemoria = tecmemoria;
    gerenciador->memvirtual = memvirtual;
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
    inicializa_alocador(&alocador, marcador, memoria, TAMANHO_MEM, MAX_PROCESSOS);
    memoria_t memoriav;
    marcador_t marcadorv;
    alocador_t alocadorv;
    gerenciador_virtual_t gerenciadorVirtual;
    if (gerenciador->memvirtual == 1)
    {
        inicializa_memoria(&memoriav, TAMANHO_MEMV);
        inicializa_marcador(&marcadorv, TAMANHO_MEMV);
        inicializa_alocador(&alocadorv, marcadorv, memoriav, TAMANHO_MEMV, MAX_PROCESSOS);
        inicializa_gerenciador_virtual(&gerenciadorVirtual, alocador, alocadorv);
    }
    printf("\nCriando Processo Gerenciador de Processos...\n");

    leArquivoInstrucao(&inst, "files/init.txt");
    inicializaProcessoSimulado(&processo, 0, -1, 0, 0, 0, alocador.memoria, 0, 0, inst, 0);
    gerenciador->tabelaDeProcessos[0] = processo;
    inicializaCpu(&gerenciador->cpu);
    gerenciador->tabelaDeProcessos[0].estado = 1;
    insereProcesso(&gerenciador->cpu, gerenciador->tabelaDeProcessos[0]);

    while (1)
    {
        tam = lerPipe(p, instPipe, 1024);
        executarProcessoSimulado(gerenciador, instPipe, tam, &alocador, &memoria, &marcador, &alocadorv, &memoriav, &marcadorv, &gerenciadorVirtual);
    }
}
void escalona(processoControle *gerenciador, alocador_t *alocador)
{
    if (gerenciador->tipoEscalonamento == 1)
    {
        escalonarTempo(gerenciador, alocador);
    }
    else
    {
        escalonarProcessos(gerenciador, alocador);
    }
}

void executarProcessoSimulado(processoControle *gerenciador, char *instrucaoPipe, int tamPipe, alocador_t *alocador, memoria_t *memoria, marcador_t *marcador, alocador_t *alocadorv, memoria_t *memoriav, marcador_t *marcadorv, gerenciador_virtual_t *gerenciadorVirtual)
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
            resultado = executaProcesso(&gerenciador->cpu, alocador, gerenciador->tipoTecMemoria, gerenciador->memvirtual, gerenciadorVirtual);
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
                int i, retorno, indice;
                pararProcesso(&gerenciador->cpu, &p, 4);
                indice = encontraIndiceTP(gerenciador, p.id);
                gerenciador->tabelaDeProcessos[indice] = p;
                insereItememFilaEBM(&gerenciador->estadoBloqueadoM, indice);
                retorno = removeItemEP(&gerenciador->estadoPronto, &i);
                i = encontraIndiceTP(gerenciador, i);
                gerenciador->tabelaDeProcessos[i].estado = 1;
                if (retorno != -1)
                {
                    insereProcesso(&gerenciador->cpu, gerenciador->tabelaDeProcessos[i]);
                }
            }
            else if (resultado == 'T')
            {
                int tentativa, j, indice;
                // TERMINA PROCESSO SIMULADO E TENTA USAR A MEMORIA PARA O ALGUM PROCESSO BLOQUEADO POR MEMORIA
                indice = encontraIndiceTP(gerenciador, gerenciador->cpu.procexec.id);
                retiraProcessoTabelaProcessos(gerenciador, indice, alocador, gerenciadorVirtual);
                if (gerenciador->memvirtual == 0)
                {
                    elementoEBM *apAux;
                    apAux = gerenciador->estadoBloqueadoM.apPrimeiro;
                    fprintf(stderr, "ProcessoControle.c ---  tam estado bloqueadom : %d\n", gerenciador->estadoBloqueadoM.tam);
                    tentativa = removeItemEBM(&gerenciador->estadoBloqueadoM, &j);
                    j = encontraIndiceTP(gerenciador, j);
                    if (tentativa == 1)
                    {
                        gerenciador->tabelaDeProcessos[j].estado = 0;
                        if (gerenciador->tipoEscalonamento == 1)
                        {
                            insereItememFilaEP(&gerenciador->estadoPronto, gerenciador->tabelaDeProcessos[j].id, gerenciador->tabelaDeProcessos[j].prioridade);
                        }
                        else
                        {
                            insereItemOrdenadoEP(&gerenciador->estadoPronto, gerenciador->tabelaDeProcessos[j].id, gerenciador->tabelaDeProcessos[j].prioridade);
                        }
                    }
                }
            }
            else if (resultado == 'F')
            {
                // CRIA NOVO PROCESSO SIMULADO FILHO
                // estado ; 0 = pronto, 1 = em execução, 2 = bloqueado, 3 = finalizado
                int pos = 0;
                gerenciador->ultimoindice++;
                for (pos; pos < MAX_PROCESSOS && gerenciador->tabelaDeProcessos[pos].id != -1; pos++)
                    ;
                inicializaProcessoSimulado(&gerenciador->tabelaDeProcessos[pos], gerenciador->ultimoindice,
                                           gerenciador->cpu.procexec.id, gerenciador->cpu.procexec.contadorPrograma + 1,
                                           0, 0, gerenciador->cpu.procexec.memoria,
                                           gerenciador->cpu.unidTempo, 0, gerenciador->cpu.procexec.programa, gerenciador->cpu.procexec.tammem);
                if (gerenciador->memvirtual == 1)
                {
                    aloca_memoria_virtual(gerenciadorVirtual, gerenciador->tabelaDeProcessos[pos].id,gerenciador->cpu.procexec.tammem);
                    for(int i=0;i<gerenciador->cpu.procexec.tammem;++i){
                        int origem = *acessa_memoria_virtual(gerenciadorVirtual,gerenciador->cpu.procexec.id,i);
                        int *destino = acessa_memoria_virtual(gerenciadorVirtual,gerenciador->tabelaDeProcessos[pos].id,i);
                        *destino = origem;
                    }
                }
                gerenciador->cpu.procexec.contadorPrograma += 1 + gerenciador->cpu.procexec.programa[gerenciador->cpu.procexec.contadorPrograma].var1;

                fprintf(stderr, "Criando processo simulado de id %d\n", gerenciador->ultimoindice);
                if (gerenciador->tipoEscalonamento == 1)
                {
                    insereItememFilaEP(&gerenciador->estadoPronto, gerenciador->tabelaDeProcessos[pos].id, gerenciador->tabelaDeProcessos[pos].prioridade);
                }
                else
                {
                    insereItemOrdenadoEP(&gerenciador->estadoPronto, gerenciador->tabelaDeProcessos[pos].id, gerenciador->tabelaDeProcessos[pos].prioridade);
                }
            }
            escalona(gerenciador, alocador);
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
            processoImpressao(gerenciador, alocador, gerenciadorVirtual);
        }
        else if (instrucaoPipe[index] == 'M')
        {
            printf("\nComando M\n");
            // Imprime o tempo médio do ciclo e finaliza o sistema
            printf(MAGENTA "\n Tempo medio de ciclo = %.2f\n" RESET, (float)(gerenciador->cpu.unidTempo / gerenciador->cpu.qtdprocessos));
            processoImpressao(gerenciador, alocador, gerenciadorVirtual);
            destroi_alocador(alocador);
            destroi_marcador(marcador);
            destroi_memoria(memoria);
            destroi_alocador(alocadorv);
            destroi_marcador(marcadorv);
            destroi_memoria(memoriav);
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
    indice = encontraIndiceTP(gerenciador, indice);
    printf("Processo de indice %d liberado!\n", gerenciador->tabelaDeProcessos[indice].id);
    if (gerenciador->tipoEscalonamento == 1)
        insereItememFilaEP(&gerenciador->estadoPronto, gerenciador->tabelaDeProcessos[indice].id, gerenciador->tabelaDeProcessos[indice].prioridade);
    else
        insereItemOrdenadoEP(&gerenciador->estadoPronto, gerenciador->tabelaDeProcessos[indice].id, gerenciador->tabelaDeProcessos[indice].prioridade);
    gerenciador->tabelaDeProcessos[indice].estado = 0;
}

void comandoB(processoControle *gerenciador)
{
    processoSimulado p;
    int i, retorno, indice;
    pararProcesso(&gerenciador->cpu, &p, 2);
    if (p.prioridade != 0)
        p.prioridade--;
    indice = encontraIndiceTP(gerenciador, p.id);
    gerenciador->tabelaDeProcessos[indice] = p;
    printf("Processo de id %d bloqueado! \n", p.id);
    insereItememFilaEB(&gerenciador->estadoBloqueado, p.id, p.prioridade);
    retorno = removeItemEP(&gerenciador->estadoPronto, &i);
    i = encontraIndiceTP(gerenciador, i);
    gerenciador->tabelaDeProcessos[i].estado = 1;
    if (retorno != -1)
    {
        insereProcesso(&gerenciador->cpu, gerenciador->tabelaDeProcessos[i]);
    }
}

int trocaContexto(processoControle *gerenciador, alocador_t *alocador)
{
    printf(RED "Trocando contexto\n" RESET);
    processoSimulado p;
    int i, indice;
    pararProcesso(&gerenciador->cpu, &p, 0);
    if (p.id != -1)
    {
        indice = encontraIndiceTP(gerenciador, p.id);
        gerenciador->tabelaDeProcessos[indice] = p;
        if (gerenciador->tipoEscalonamento == 1)
            insereItememFilaEP(&gerenciador->estadoPronto, p.id, p.prioridade);
        else
            insereItemOrdenadoEP(&gerenciador->estadoPronto, p.id, p.prioridade);
    }
    if (lEhVaziaEP(&gerenciador->estadoPronto))
    {
        gerenciador->cpu.procexec.id = -1;
        return -1;
    }
    removeItemEP(&gerenciador->estadoPronto, &i);
    i = encontraIndiceTP(gerenciador, i);
    fprintf(stderr, "ProcessoControle.c - trocacontexto - teste -- indice removido = %d\n", gerenciador->tabelaDeProcessos[i].id);
    gerenciador->tabelaDeProcessos[i].estado = 1;
    insereProcesso(&gerenciador->cpu, gerenciador->tabelaDeProcessos[i]);

    return 1;
}
void escalonarTempo(processoControle *gerenciador, alocador_t *alocador)
{
    if (gerenciador->cpu.tempoProcessoAtual >= 5 || gerenciador->cpu.procexec.id == -1)
        trocaContexto(gerenciador, alocador);
}
void escalonarProcessos(processoControle *gerenciador, alocador_t *alocador)
{
    if (gerenciador->cpu.procexec.prioridade == 0)
    {
        gerenciador->cpu.procexec.prioridade = 1;
        trocaContexto(gerenciador, alocador);
    }
    else if (gerenciador->cpu.procexec.prioridade == 1 && gerenciador->cpu.tempoProcessoAtual > 1)
    {
        gerenciador->cpu.procexec.prioridade = 2;
        trocaContexto(gerenciador, alocador);
    }
    else if (gerenciador->cpu.procexec.prioridade == 2 && gerenciador->cpu.tempoProcessoAtual > 3)
    {
        gerenciador->cpu.procexec.prioridade = 3;
        trocaContexto(gerenciador, alocador);
    }
    else if (gerenciador->cpu.procexec.prioridade == 3 && gerenciador->cpu.tempoProcessoAtual > 7)
    {
        trocaContexto(gerenciador, alocador);
    }
    else if (gerenciador->cpu.procexec.id == -1)
    {
        trocaContexto(gerenciador, alocador);
    }
}

void processoImpressao(processoControle *gerenciador, alocador_t *alocador, gerenciador_virtual_t *gerenciadorVirtual)
{
    if (gerenciador->memvirtual == 0)
    {
        exibe_memoria(alocador);
    }
    else
    {
        mostra_memoriaVirtual(gerenciadorVirtual);
    }
    for (int i = 0; i < MAX_PROCESSOS; i++)
    {
        if (gerenciador->tabelaDeProcessos[i].id == -1)
            continue;
        if (gerenciador->tabelaDeProcessos[i].id == gerenciador->cpu.procexec.id)
            mostrarProcessoCpu(&gerenciador->cpu, gerenciadorVirtual,gerenciador->memvirtual);
        else
            mostrarRelatorioProcesso(&gerenciador->tabelaDeProcessos[i], gerenciadorVirtual,gerenciador->memvirtual); // mostrar relatório processo
    }
    if (gerenciador->cpu.procexec.id == -1)
        mostrarProcessoCpu(&gerenciador->cpu, gerenciadorVirtual,gerenciador->memvirtual);
}

void retiraProcessoTabelaProcessos(processoControle *gerenciador, int indice, alocador_t *alocador, gerenciador_virtual_t *gerenciadorVirtual)
{
    if (gerenciador->memvirtual)
    {
        desaloca_memoria_virtual(gerenciadorVirtual, gerenciador->cpu.procexec.id);
    }
    else
        desaloca_memoria_simulada(alocador, gerenciador->cpu.procexec.memoria);
    alocador->qtfragmentos++;
    printf("Processo %d finalizado ... \n", gerenciador->cpu.procexec.id);
    gerenciador->cpu.procexec.id = -1;
    gerenciador->tabelaDeProcessos[indice].id = -1;
}
int encontraIndiceTP(processoControle *gerenciador, int indice)
{
    for (int i = 0; i < MAX_PROCESSOS; ++i)
    {
        if (gerenciador->tabelaDeProcessos[i].id == indice)
            return i;
    }
}