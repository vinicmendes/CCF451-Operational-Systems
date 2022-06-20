#include "leitor.h"

void lerTerminal(Pipe *p)
{
    printf("\n PIPE \n");
    char c = ' ';
    printf("Digite os comandos, um a um (Ex: U ou  L ou I ou M): ");
    while (c != 'M'){
        scanf(" %c",&c);
        c = toupper(c);
        escrevePipe(p,c);
    }
}

int leArquivoPipe(Pipe *p, char *nomeArquivo)
{
    FILE *file;
    char c;
    char arqv[20];

    file = fopen(nomeArquivo, "r");
    if (file == NULL)
    {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    while(fscanf(file, " %c", &c) != EOF){
        fprintf(stderr, "leitor.c - lerArquivoPipe - leu: %c\n", c);
        escrevePipe(p, c);
        sleep(1);
    }
    fclose(file);
    return 1;
}

int leArquivoInstrucao(instrucao **inst, char *nomeArquivo)
{
    FILE *file;
    char c;
    char arqv[20];
    int ordemInstrucao = 0;
    (*inst) = (instrucao *)malloc((100) * sizeof(instrucao));
    file = fopen(nomeArquivo, "r");
    if (file == NULL)
    {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }
    while ((c = fgetc(file)) != EOF)
    {
        c = fgetc(file);
        (*inst)[ordemInstrucao].id = c;
        switch (c)
        {
        case 'N':
            fscanf(file, "%d", &(*inst)[ordemInstrucao].var1);
        case 'D':
            fscanf(file, "%d", &(*inst)[ordemInstrucao].var1);
        case 'V':
            fscanf(file, "%d %d", &(*inst)[ordemInstrucao].var1, &(*inst)[ordemInstrucao].var2);
        case 'A':
            fscanf(file, "%d %d", &(*inst)[ordemInstrucao].var1, &(*inst)[ordemInstrucao].var2);
        case 'S':
            fscanf(file, "%d %d", &(*inst)[ordemInstrucao].var1, &(*inst)[ordemInstrucao].var2);
        case 'F':
            fscanf(file, "%d", &(*inst)[ordemInstrucao].var1);
        case 'R':
            fscanf(file, "%s", (*inst)[ordemInstrucao].arqv);
        default:
            break;
        }
        ++ordemInstrucao;
        c = fgetc(file);
    }
    fclose(file);
    return 1;
}

void lerArquivoProcessoS(instrucao **inst, char *nomeArquivo)
{
    FILE *arq;
    char c;
    int ordemInstrucao = 0;
    (*inst) = (instrucao *)malloc(1000 * sizeof(instrucao));
    arq = fopen(nomeArquivo, "r");
    if (arq == NULL)
    {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }
    while ((c = fgetc(arq)) != EOF)
    {
        c = fgetc(arq);
        (*inst)[ordemInstrucao].id = c;
        switch (c)
        {
        case 'N':
            fscanf(arq, "%d", &(*inst)[ordemInstrucao].var1);
        case 'D':
            fscanf(arq, "%d", &(*inst)[ordemInstrucao].var1);
        case 'V':
            fscanf(arq, "%d %d", &(*inst)[ordemInstrucao].var1, &(*inst)[ordemInstrucao].var2);
        case 'A':
            fscanf(arq, "%d %d", &(*inst)[ordemInstrucao].var1, &(*inst)[ordemInstrucao].var2);
        case 'S':
            fscanf(arq, "%d %d", &(*inst)[ordemInstrucao].var1, &(*inst)[ordemInstrucao].var2);
        case 'F':
            fscanf(arq, "%d", &(*inst)[ordemInstrucao].var1);
        case 'R':
            fscanf(arq, "%s", (*inst)[ordemInstrucao].arqv);
        default:
            break;
        }
        ++ordemInstrucao;
        c = fgetc(arq);
    }
    fclose(arq);
}