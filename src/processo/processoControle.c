#include "processoControle.h"
#define MAX_PROCESSOS 50

void inicializaProcessoC(processoControle *gerenciador)
{
    gerenciador->unidadeTempo = 0;
    inicializaCpu(&gerenciador->cpu);
    gerenciador->tabelaDeProcessos = (processoSimulado *)malloc(sizeof(processoSimulado) * MAX_PROCESSOS);
    inicializaListaEP(&gerenciador->estadoPronto);
    inicializaListaEB(&gerenciador->estadoBloqueado); // temos que criar a lista encadeada ou fila pra definir fluxo do processo
    gerenciador->estadoExecucao = -1;
    gerenciador->ultimaposicao = 0;
    gerenciador->ultimoindice = 0;
}

void executaGerenciador(processoControle *gerenciador, Pipe *p)
{
    processoSimulado processo;
    instrucao *inst;
    int *buffer;
    char *instPipe;
    int tipo = 0;

    printf("\nCriando Processo Simulado...\n");

    lerArquivoDeInstrucoesPS(&inst, "../files/file_f");

    InicializaProcessoSimulado(&processo, 0, -1, 0, buffer, 0, 2, 0, 0, inst);
    gerenciador->tabelaDeProcessos[gerenciador->ultimaposicao] = processo;
    inicializaCpu(&gerenciador->cpu);
    gerenciador->tabelaDeProcessos[gerenciador->ultimaposicao].estado = 2;
    insereProcesso(&gerenciador->cpu, gerenciador->tabelaDeProcessos[gerenciador->ultimaposicao]);
    gerenciador->estadoExecucao = 0;
    ++gerenciador->ultimaposicao;

    while (tipo != 1 && tipo != 2)
    {
        printf("\nQual o tipo de escalonamento você deseja?\n");
        printf("1. Escalonamento por *****\n");
        printf("2. Escalonamento por Filas ordenadas por prioridades\n");
        scanf("%d", &tipo);
    }
    gerenciador->tipoEscalonamento = tipo;

    lerPipe(p, &instPipe);

    executarProcessoSimulado(gerenciador, instPipe);
}

void executarProcessoSimulado(processoControle *gerenciador, char *instrucaoPipe)
{
    int index, retorno, tamPipe;
    tamPipe = strlen(instrucaoPipe);
    char resultado;
    processoSimulado processo;

    for (index = 0; index < strlen(instrucaoPipe); index++)
    {
        if (instrucaoPipe[index] == 'U')
        {
            // FIM DE UNIDADE DE TEMPO
            resultado = executaProcesso(&gerenciador->cpu);

            if (resultado == 'B')
            {
                // BLOQUEIA PROCESSO SIMULADO
                comandoB(gerenciador);
                continue;
            }
            else if (resultado == 'T')
            {
                // TERMINA PROCESSO SIMULADO
                retiraProcessoTabelaProcessos(gerenciador, gerenciador->estadoExecucao);
                retorno = trocaContexto(gerenciador);
                if (retorno == -1)
                {
                    gerenciador->cpu.processo.id = -1;
                }
            }
            else if (resultado = 'F')
            {
                // CRIA NOVO PROCESSO SIMULADO FILHO
                // estado ; 0 = pronto, 1 = em execução, 2 = bloquado, 3 = finalizado
                ++gerenciador->ultimoindice;
                inicializaProcessoSimulado(&gerenciador->tabelaDeProcessos[gerenciador->ultimaposicao], gerenciador->ultimoindice,
                                           gerenciador->tabelaDeProcessos[gerenciador->estadoExecucao].id, gerenciador->cpu.processo.contadorPrograma + 1,
                                           gerenciador->cpu.processo.prioridade, 0, gerenciador->cpu.processo.memoria,
                                           gerenciador->cpu.unidTempo, 0, gerenciador->cpu.processo.programa);
                alterarContadorPrograma(&gerenciador->cpu);

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
            if (resultado != 'B')
            {
                if (gerenciador->tipoEscalonamento == 1)
                {
                    escalonar(gerenciador);
                }
                else
                {
                    escalonarProcessos(gerenciador);
                }
            }
        }
        else if (instrucaoPipe[index] == 'L'){
            //Desbloqueia o primeiro processo simulado na fila bloqueada
            comandoL(gerenciador);
            if(gerenciador->tipoEscalonamento == 1){
                escalonar(gerenciador);
            }
            else{
                escalonarProcessos(gerenciador);
            }
        }
        else if(instrucaoPipe[index] == 'I'){
            //Imprime o estado atual do gerenciador
            processoImpressao(gerenciador);
        }
        else if(instrucaoPipe[index] == 'M'){
            //Imprime o tempo médio do ciclo e finaliza o sistema
            processoImpressao(gerenciador);
            return;
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
    if (gerenciador->tipoEscalonamento == 1)
        insereItememFilaEP(&gerenciador->estadoPronto, indice, gerenciador->tabelaDeProcessos[indice].prioridade);
    else
        insereItemOrdenadoEP(&gerenciador->estadoPronto, indice, gerenciador->tabelaDeProcessos[indice].prioridade);
}

void comandoB(processoControle *gerenciador)
{
    processoSimulado p;
    int i, retorno;
    pararProcesso(&gerenciador->cpu, &p);
    if (p.prioridade != 0)
        --p.prioridade;
    gerenciador->tabelaDeProcessos[gerenciador->estadoExecucao] = p;
    insereItememFilaEB(&gerenciador->estadoBloqueado, gerenciador->estadoExecucao, gerenciador->tabelaDeProcessos[gerenciador->estadoExecucao].prioridade);
    gerenciador->estadoExecucao = -1;
    retorno = removeItemEP(&gerenciador->estadoPronto, &i);
    gerenciador->tabelaDeProcessos[i].estado = 2;
    if (retorno != 1)
    {
        insereProcesso(&gerenciador->cpu, gerenciador->tabelaDeProcessos[i]);
        gerenciador->estadoExecucao = i;
    }
}
int trocaContexto(processoControle *gerenciador)
{
    processoSimulado p;
    int i;
    if (lEhVaziaEP(&gerenciador->estadoPronto))
        return 1;
    pararProcesso(&gerenciador->cpu, &p);
    if (p.id != -1)
    {
        gerenciador->tabelaDeProcessos[gerenciador->estadoExecucao] = p;
        if (gerenciador->tipoEscalonamento == 1)
            insereItememFilaEP(&gerenciador->estadoPronto, gerenciador->estadoExecucao, gerenciador->tabelaDeProcessos[gerenciador->estadoExecucao].prioridade);
        else
            insereItemOrdenadoEP(&gerenciador->estadoPronto, gerenciador->estadoExecucao, gerenciador->tabelaDeProcessos[gerenciador->estadoExecucao].prioridade);
    }
    removeItemEP(&gerenciador->estadoPronto, &i);
    gerenciador->tabelaDeProcessos[i].estado = 2;
    insereProcesso(&gerenciador->cpu, gerenciador->tabelaDeProcessos[i]);
    gerenciador->estadoExecucao = i;
    
    return 1;
}
