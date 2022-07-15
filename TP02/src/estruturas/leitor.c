#include "leitor.h"

void lerTerminal(Pipe *p)
{
    printf("\n PIPE \n");
    char c = ' ';
    printf("\nDigite os comandos, um a um (Ex: U ou  L ou I ou M): ");
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
        //sleep(1);
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
        printf("Erro ao abrir arquivo %s\n",nomeArquivo);
        exit(1);
    }
    while (fscanf(file," %c",&c) != EOF)
    {
        (*inst)[ordemInstrucao].id = c;
        switch (c)
        {
        case 'N':
            fscanf(file, " %d", &(*inst)[ordemInstrucao].var1);
            break;
        case 'D':
            fscanf(file, " %d", &(*inst)[ordemInstrucao].var1);
            break;
        case 'V':
            fscanf(file, " %d %d", &(*inst)[ordemInstrucao].var1, &(*inst)[ordemInstrucao].var2);
            break;
        case 'A':
            fscanf(file, " %d %d", &(*inst)[ordemInstrucao].var1, &(*inst)[ordemInstrucao].var2);
            break;
        case 'S':
            fscanf(file, " %d %d", &(*inst)[ordemInstrucao].var1, &(*inst)[ordemInstrucao].var2);
            break;
        case 'F':
            fscanf(file, " %d", &(*inst)[ordemInstrucao].var1);
            break;
        case 'R':
            fscanf(file, " %s", (*inst)[ordemInstrucao].arqv);
            break;
        default:
            break;
        }
        ordemInstrucao++;
    }
    fclose(file);
    return 1;
}
