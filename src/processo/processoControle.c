#include "processoControle.h"
#define MAX_PROCESSOS 50

void inicializaProcessoC(processoControle *gerenciador,int tipo)
{
    gerenciador->unidadeTempo = 0;
    inicializaCpu(&gerenciador->cpu);
    gerenciador->tabelaDeProcessos = (processoSimulado *)malloc(sizeof(processoSimulado) * MAX_PROCESSOS);
    inicializaListaEP(&gerenciador->estadoPronto);
    inicializaListaEB(&gerenciador->estadoBloqueado); // temos que criar a lista encadeada ou fila pra definir fluxo do processo
    gerenciador->estadoExecucao = -1;
    gerenciador->ultimaposicao = 0;
    gerenciador->ultimoindice = 0;
    gerenciador->tipoEscalonamento = tipo;
}

void executaProcessoC(processoControle *gerenciador, Pipe *p)
{
    processoSimulado processo;
    instrucao *inst;
    int *buffer,contador=0;
    char instPipe[1025];
    
    printf("\nCriando Processo Simulado...\n");

    leArquivoInstrucao(&inst, "files/init.txt");
    inicializaProcessoSimulado(&processo, 0, -1, 0 , 0, 0,buffer, 0, 0, inst);
    gerenciador->tabelaDeProcessos[gerenciador->ultimaposicao] = processo;
    inicializaCpu(&gerenciador->cpu);
    gerenciador->tabelaDeProcessos[gerenciador->ultimaposicao].estado = 2;
    insereProcesso(&gerenciador->cpu, gerenciador->tabelaDeProcessos[gerenciador->ultimaposicao]);
    gerenciador->estadoExecucao = 0;
    ++gerenciador->ultimaposicao;

    while(1) {
        lerPipe(p, instPipe, 1024);
        executarProcessoSimulado(gerenciador, instPipe);
        printf("processoControle.c --- processo Controle -- mais uma execuçao\n");
    }

}

void executarProcessoSimulado(processoControle *gerenciador, char *instrucaoPipe)
{ // DA PRA ALTERAR PARA SWITCH CASE
    int index, retorno, tamPipe;
    tamPipe = strlen(instrucaoPipe);
    char resultado;
    processoSimulado processo;

    for (index = 0; index < strlen(instrucaoPipe); index++)
    {
        if (instrucaoPipe[index] == 'U')
        {
            printf("\nComando U\n");
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
                    gerenciador->cpu.procexec.id = -1;
                }
            }
            else if (resultado = 'F')
            {
                // CRIA NOVO PROCESSO SIMULADO FILHO
                // estado ; 0 = pronto, 1 = em execução, 2 = bloquado, 3 = finalizado
                ++gerenciador->ultimoindice;
                inicializaProcessoSimulado(&gerenciador->tabelaDeProcessos[gerenciador->ultimaposicao], gerenciador->ultimoindice,
                                           gerenciador->tabelaDeProcessos[gerenciador->estadoExecucao].id, gerenciador->cpu.procexec.contadorPrograma + 1,
                                           gerenciador->cpu.procexec.prioridade, 0, gerenciador->cpu.procexec.memoria,
                                           gerenciador->cpu.unidTempo, 0, gerenciador->cpu.procexec.programa);
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
                    escalonarTempo(gerenciador);
                }
                else
                {
                    escalonarProcessos(gerenciador);
                }
            }
        }
        else if (instrucaoPipe[index] == 'L')
        {
            printf("\nComando L\n");
            // Desbloqueia o primeiro processo simulado na fila bloqueada
            comandoL(gerenciador);
            if (gerenciador->tipoEscalonamento == 1)
            {
                escalonarTempo(gerenciador);
            }
            else
            {
                escalonarProcessos(gerenciador);
            }
        }
        else if (instrucaoPipe[index] == 'I')
        {
            printf("\nComando I\n");
            // Imprime o estado atual do gerenciador
            processoImpressao(gerenciador);
        }
        else if (instrucaoPipe[index] == 'M')
        {
            printf("\nComando M\n");
            // Imprime o tempo médio do ciclo e finaliza o sistema
            processoImpressao(gerenciador);
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
    gerenciador->tabelaDeProcessos[i].estado = 1;
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
    gerenciador->tabelaDeProcessos[i].estado = 1;
    insereProcesso(&gerenciador->cpu, gerenciador->tabelaDeProcessos[i]);
    gerenciador->estadoExecucao = i;

    return 1;
}
void escalonarTempo(processoControle *gerenciador){
    if(gerenciador->cpu.procexec.tempoCPU >= 10 || gerenciador->cpu.procexec.id == -1)
        trocaContexto(gerenciador);
}
void escalonarProcessos(processoControle *gerenciador)
{ // DA PRA ALTERAR PARA SWITCH CASE
    if (gerenciador->cpu.procexec.prioridade == 0)
    {
        gerenciador->cpu.procexec.prioridade = 1;
        trocaContexto(gerenciador);
    }
    else if (gerenciador->cpu.procexec.prioridade == 1 && gerenciador->cpu.procexec.tempoAtual > 1)
    {
        gerenciador->cpu.procexec.prioridade = 2;
        trocaContexto(gerenciador);
    }
    else if (gerenciador->cpu.procexec.prioridade == 2 && gerenciador->cpu.procexec.tempoAtual > 3)
    {
        gerenciador->cpu.procexec.prioridade = 3;
        trocaContexto(gerenciador);
    }
    else if (gerenciador->cpu.procexec.prioridade == 3 && gerenciador->cpu.procexec.tempoAtual > 7)
    {
        trocaContexto(gerenciador);
    }
    else if (gerenciador->cpu.procexec.id == -1)
    {
        trocaContexto(gerenciador);
    }
}

void processoImpressao(processoControle *gerenciador)
{
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

void retiraProcessoTabelaProcessos(processoControle *gerenciador, int indice)
{
    processoSimulado processo;
    atualizaItensEB(&gerenciador->estadoBloqueado, indice);
    atualizaItensEP(&gerenciador->estadoPronto, indice);

    while (indice < MAX_PROCESSOS - 1)
    {
        gerenciador->tabelaDeProcessos[indice] = gerenciador->tabelaDeProcessos[indice + 1];
        indice++;
    }

    gerenciador->cpu.procexec.id = -1;
    gerenciador->tabelaDeProcessos[MAX_PROCESSOS - 1] = processo;
    gerenciador->ultimaposicao--;
}