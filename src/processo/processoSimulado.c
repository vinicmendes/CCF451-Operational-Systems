#include "processoSimulado.h"

void inicializaProcessoSimulado( processoSimulado *processo, int id, int idPrincipal,
 int *contadorPrograma, int prioridade, int *memoria, int tempoInicio, int tempoCPU, instrucao *instrucao){
    processo->id = id;
    processo->idPrincipal = idPrincipal;
    processo->contadorPrograma = contadorPrograma;
    processo->prioridade = prioridade;
    processo->memoria = memoria;
    processo->tempoInicio = tempoInicio;
    processo->tempoCPU = tempoCPU;
    processo->programa = instrucao;
}

void executaInstrucao( processoSimulado *processo){
    instrucao instrucao = processo->programa[*(processo->contadorPrograma)];
    switch (instrucao.id)
    {
    case 'N':
        instrucaoN(processo, instrucao);
        processo->contadorPrograma++;
    case 'D':
        instrucaoD(processo, instrucao);
        processo->contadorPrograma++;
    case 'V':
        instrucaoV(processo, instrucao);
        processo->contadorPrograma++;
    case 'A':
        instrucaoA(processo, instrucao);    
        processo->contadorPrograma++;
    case 'S':
        instrucaoS(processo, instrucao);
        processo->contadorPrograma++;
    case 'B':
        instrucaoB(processo);
        processo->contadorPrograma++;
    case 'T':
        instrucaoT(processo);
        processo->contadorPrograma++;
    case 'F':
        instrucaoF(processo, instrucao);
        processo->contadorPrograma++;
    case 'R':
        instrucaoR(processo, instrucao);
        processo->contadorPrograma++;
    default:
        break;
    }
}

void instrucaoN( processoSimulado *processo, instrucao instrucao){
    processo->memoria = (int *)malloc(sizeof(int) * instrucao.var1);
}

void instrucaoD( processoSimulado *processo, instrucao instrucao){
    processo->memoria[instrucao.var1] = 0;
}

void instrucaoV( processoSimulado *processo, instrucao instrucao){
    processo->memoria[instrucao.var1] = instrucao.var2;
}

void instrucaoA( processoSimulado *processo, instrucao instrucao){
    processo->memoria[instrucao.var1] += instrucao.var2;
}

void instrucaoS( processoSimulado *processo, instrucao instrucao){
    processo->memoria[instrucao.var1] -= instrucao.var2;
}

void instrucaoB( processoSimulado *processo){
    processo->estado = 2;
}

void instrucaoT( processoSimulado *processo){
    processo->estado = 3;
}

void instrucaoF( processoSimulado *processo, instrucao instrucao){
    
}

void instrucaoR( processoSimulado *processo, instrucao instrucao){
    free(processo->programa);
    //chamar função que le o arquivo do processo simulado
    processo->contadorPrograma = 0;
}

void calculaTempo( processoSimulado *processo){
    processo->tempoAtual++;
}