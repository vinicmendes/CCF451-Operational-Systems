#ifndef LEITOR_H
#define LEITOR_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pipe.h"
#include "instrucao.h"

void leArquivoPipe(Pipe *p, char *nomeArquivo);
void leArquivoInstrucao(instrucao **inst, char *nomeArquivo);
void lerArquivoProcessoS(instrucao **inst, char *nomeArquivo);
#endif