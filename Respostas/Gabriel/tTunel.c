#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "tPosicao.h"
#include "tTunel.h"

tTunel* CriaTunel(int linhaAcesso1, int colunaAcesso1, int linhaAcesso2, int colunaAcesso2){
    tTunel* t = (tTunel*) malloc(sizeof(tTunel));

    if(t == NULL){
        printf("ERRO tTunel");
        exit(1);
    }

    t->acesso1 = CriaPosicao(linhaAcesso1, colunaAcesso1);
    t->acesso2 = CriaPosicao(linhaAcesso2, colunaAcesso2);
    return t;
}

bool EntrouTunel(tTunel* tunel, tPosicao* posicao){
    return (SaoIguaisPosicao(tunel->acesso1, posicao) || SaoIguaisPosicao(tunel->acesso2, posicao));
}

void LevaFinalTunel(tTunel* tunel, tPosicao* posicao){
    if(SaoIguaisPosicao(tunel->acesso1, posicao)){
        AtualizaPosicao(posicao, tunel->acesso2);
    }
    else{
        AtualizaPosicao(posicao, tunel->acesso2);
    }
}

void DesalocaTunel(tTunel* tunel){
    if(tunel != NULL){
        DesalocaPosicao(tunel->acesso1);
        DesalocaPosicao(tunel->acesso2);
        free(tunel);
    }
}