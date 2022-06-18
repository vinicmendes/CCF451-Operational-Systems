#include "processoControle.h"
#define MAX_PROCESSOS 50

void inicializaProcessoC(processoControle *gerenciador){
    gerenciador->unidadeTempo = 0;
    inicializaCpu(&gerenciador->cpu);
    gerenciador->tabelaDeProcessos = (processoSimulado *)malloc(sizeof(processoSimulado) * MAX_PROCESSOS);
    // FLVazia(&gProc->estadoPronto);
    // FLVazia(&gProc->estadoBloqueado);    //temos que criar a lista encadeada ou fila pra definir fluxo do processo
    gerenciador->estadoExecucao = -1;
    gerenciador->ultimaposicao = 0;
    gerenciador->ultimoindice = 0;  
}    

void executaGerenciador(processoControle *gerenciador, Pipe *p){
    processoSimulado processo;
    instrucao *inst;
    int *buffer;
    char *instPipe;
    int tipo=0;

    printf("\nCriando Processo Simulado...\n");

    lerArquivoDeInstrucoesPS(&inst);

    InicializaProcessoSimulado(&processo, 0, -1, 0, buffer, 0, 2, 0, 0, inst);
    gerenciador->tabelaDeProcessos[gerenciador->ultimaposicao] = processo;
    inicializaCPU(&gerenciador->cpu);
    gerenciador->tabelaDeProcessos[gerenciador->ultimaposicao].estado = 2;
    insereProcessoCPU(&gerenciador->cpu, gerenciador->tabelaDeProcessos[gerenciador->ultimaposicao]);
    gerenciador->estadoExecucao = 0;
    gerenciador->ultimaposicao++;

    while (tipo!=1 && tipo!=2)
    {
        printf("\nQual o tipo de escalonamento você deseja?\n");
        printf("1. Escalonamento por Chaveamento Circular\n");
        printf("2. Escalonamento por Filas com classes de prioridades\n");
        scanf("%d", &tipo);
    }
    gerenciador->tipoEscalonamento = tipo;
    
    lerPipe(p, &instPipe);
  
    executarProcessoSimulado(gerenciador, instPipe);
}