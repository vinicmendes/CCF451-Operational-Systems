#ifndef LEITOR_H
#define LEITOR_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "pipe.h"
#include "instrucao.h"

void lerTerminal(Pipe *p);
int leArquivoPipe(Pipe *p, char *nomeArquivo);
int leArquivoInstrucao(instrucao **inst, char *nomeArquivo);

#endif