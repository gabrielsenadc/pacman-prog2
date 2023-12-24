#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tPosicao.h"

tPosicao* CriaPosicao(int linha, int coluna){
    tPosicao* p = (tPosicao*) malloc(sizeof(tPosicao));

    if(p == NULL){
        printf("ERRO tPosicao");
        exit(1);
    }

    p->linha = linha;
    p->coluna = coluna;
    return p;
}

tPosicao* ClonaPosicao(tPosicao* posicao){
    tPosicao* p = CriaPosicao(posicao->linha, posicao->coluna);
    return p;
}

int ObtemLinhaPosicao(tPosicao* posicao){
    return posicao->linha;
}

int ObtemColunaPosicao(tPosicao* posicao){
    return posicao->coluna;
}

void AtualizaPosicao(tPosicao* posicaoAtual, tPosicao* posicaoNova){
    posicaoAtual->linha = posicaoNova->linha;
    posicaoAtual->coluna = posicaoNova->coluna;
}

bool SaoIguaisPosicao(tPosicao* posicao1, tPosicao* posicao2){
    return (posicao1->linha == posicao2->linha && posicao1->coluna == posicao2->coluna);
}

void DesalocaPosicao(tPosicao* posicao){
    free(posicao);
}