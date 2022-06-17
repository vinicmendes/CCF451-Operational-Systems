#include "leitor.h"


void leArquivoPipe(Pipe *p, char *nomeArquivo){
    FILE *file;
    char c;
    char arqv[20];
    inicializaPipe(p);


    file = fopen(nomeArquivo, "r");
    if(file == NULL){
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    while((c=fgetc(file))!= EOF){
        c = fgetc(file);
        if(c != '\n'){
            strcat(arqv, &c);
        }
        else{
            fscanf(file, "%c", &c);
            escrevePipe(p, c);
        }
    }
    fclose(file);
}

void leArquivoInstrucao(instrucao **inst, char *nomeArquivo){
    FILE *file;
    char c;
    char arqv[20];
    int ordemInstrucao = 0;
    (*inst) = (instrucao*)malloc((100)*sizeof(instrucao));
    file = fopen(nomeArquivo, "r");
    if(file == NULL){
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }
    while((c=fgetc(file))!= EOF){
        c = fgetc(file);
        switch (c)
        {
        case 'N':
            fscanf(file, "%d", (*inst)[ordemInstrucao].var1);
        case 'D':
            fscanf(file, "%d", (*inst)[ordemInstrucao].var1);
        case 'V':
            fscanf(file, "%d %d", (*inst)[ordemInstrucao].var1, (*inst)[ordemInstrucao].var2);
        case 'A':
            fscanf(file, "%d %d", (*inst)[ordemInstrucao].var1, (*inst)[ordemInstrucao].var2);
        case 'S':
            fscanf(file, "%d %d", (*inst)[ordemInstrucao].var1, (*inst)[ordemInstrucao].var2);
        case 'B':
            //TO_DO
        case 'T':
            //TO_DO
        case 'F':
            fscanf(file, "%d", (*inst)[ordemInstrucao].var1);
        case 'R':
            fscanf(file, "%s", (*inst)[ordemInstrucao].arqv);
        default:
            break;
        }   
    }
    fclose(file);
}

void lerArquivoProcessoS(instrucao **inst, char *nomeArquivo){
    FILE *arq;
    char c;
    int ordemInstrucao = 0;
    inst = (instrucao*) malloc(1000* sizeof(instrucao));
    arq = fopen(nomeArquivo, "r");
    if(arq == NULL){
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }
    while((c=fgetc(arq))!= EOF){
        c = fgetc(arq);
        switch (c)
        {
        case 'N':
            fscanf(arq, "%d", (*inst)[ordemInstrucao].var1);
        case 'D':
            fscanf(arq, "%d", (*inst)[ordemInstrucao].var1);
        case 'V':
            fscanf(arq, "%d %d", (*inst)[ordemInstrucao].var1, (*inst)[ordemInstrucao].var2);
        case 'A':
            fscanf(arq, "%d %d", (*inst)[ordemInstrucao].var1, (*inst)[ordemInstrucao].var2);
        case 'S':
            fscanf(arq, "%d %d", (*inst)[ordemInstrucao].var1, (*inst)[ordemInstrucao].var2);
        case 'B':
            //TO_DO
        case 'T':
            //TO_DO
        case 'F':
            fscanf(arq, "%d", (*inst)[ordemInstrucao].var1);
        case 'R':
            fscanf(arq, "%s", (*inst)[ordemInstrucao].arqv);
        default:
            break;
        }   
    }
    fclose(arq);
}