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
    (*inst) = (instrucao*)malloc((100)*sizeof(instrucao));
    file = fopen(nomeArquivo, "r");
    
}

void lerArquivoProcessoS(instrucao *inst, char *nomeArquivo){
    FILE *arq;
    char c;
    inst = (instrucao*) malloc(1000* sizeof(instrucao));
    
}