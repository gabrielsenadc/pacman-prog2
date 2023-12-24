#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tMovimento.h"

tMovimento* CriaMovimento(int numeroDoMovimento, COMANDO comando, const char* acao){
    tMovimento* m = (tMovimento*) malloc(sizeof(tMovimento));

    if(m == NULL){
        printf("ERRO tMovimento");
        exit(1);
    }

    m->numeroDoMovimento = numeroDoMovimento;
    m->comando = comando;
    strcpy(m->acao, acao);
    return m;
}

int ObtemNumeroMovimento(tMovimento* movimento){
    return movimento->numeroDoMovimento;
}

COMANDO ObtemComandoMovimento(tMovimento* movimento){
    return movimento->comando;
}

char* ObtemAcaoMovimento(tMovimento* movimento){
    return movimento->acao;
}

void DesalocaMovimento(tMovimento* movimento){
    free(movimento);
}