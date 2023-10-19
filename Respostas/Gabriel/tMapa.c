#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "tPosicao.h"
#include "tTunel.h"
#include "tMapa.h"

tMapa* CriaMapa(const char* caminhoConfig){
    FILE *pFile;
    char mapa[1030];
    sprintf(mapa, "%s/mapa.txt", caminhoConfig);
    pFile = fopen(mapa, "r");
    tMapa* m = (tMapa*) malloc(sizeof(tMapa));
    fscanf(pFile, "%d\n", &m->nMaximoMovimentos);

    int linha = 1, coluna = 0, fruta = 0, tunel = 0, pl1, pl2, pc1, pc2;
    char c;
    
    m->grid = malloc(sizeof(char*));
    m->grid[0] = NULL;
    while(1){
        fscanf(pFile, "%c", &c);
        if(c == '\n'){
            break;
        }
        coluna++;
        m->grid[0] = realloc(m->grid[0], coluna * sizeof(char));
        m->grid[0][coluna - 1] = c;
    }
    while(fscanf(pFile, "%c", &c) == 1){
        if(c == 'F'){
            break;
        }
        linha++;
        m->grid = realloc(m->grid, linha * sizeof(char*));
        m->grid[linha - 1] = malloc(coluna * sizeof(char));
        m->grid[linha - 1][0] = c;
        for(int j = 1; j < coluna; j++){
            fscanf(pFile, "%c", &c);
            if(c == '*'){
            fruta++;
            }
            if(c == '@'){
                if(tunel == 0){
                    pl1 = linha - 1;
                    pc1 = j;
                    tunel++;
                }
                else{
                    pl2 = linha - 1;
                    pc2 = j;
                }
            }
            m->grid[linha - 1][j] = c;
        }
        fscanf(pFile, "%*c");
    }
    m->tunel = NULL;
    if(tunel > 0){
        m->tunel = CriaTunel(pl1, pc1, pl2, pc2);
    }
    m->nLinhas = linha;
    m->nColunas = coluna;
    m->nFrutasAtual = fruta;
    fclose(pFile);
    return m;
}

tPosicao* ObtemPosicaoItemMapa(tMapa* mapa, char item){
    tPosicao* p = NULL;
    for(int i = 0; i < mapa->nLinhas; i++){
        for(int j = 0; j < mapa->nColunas; j++){
            if(mapa->grid[i][j] == item){
                p = CriaPosicao(i, j);
            }
        }
    }
    return p;
}

tTunel* ObtemTunelMapa(tMapa* mapa){
    return mapa->tunel;
}

char ObtemItemMapa(tMapa* mapa, tPosicao* posicao){
    return mapa->grid[posicao->linha][posicao->coluna];
}

int ObtemNumeroLinhasMapa(tMapa* mapa){
    return mapa->nLinhas;
}

int ObtemNumeroColunasMapa(tMapa* mapa){
    return mapa->nColunas;
}

int ObtemQuantidadeFrutasIniciaisMapa(tMapa* mapa){
    return mapa->nFrutasAtual;
}

int ObtemNumeroMaximoMovimentosMapa(tMapa* mapa){
    return mapa->nMaximoMovimentos;
}

bool EncontrouComidaMapa(tMapa* mapa, tPosicao* posicao){
    return (ObtemItemMapa(mapa, posicao) == '*');
}

bool EncontrouParedeMapa(tMapa* mapa, tPosicao* posicao){
    return (ObtemItemMapa(mapa, posicao) == '#');
}

bool AtualizaItemMapa(tMapa* mapa, tPosicao* posicao, char item){
    mapa->grid[posicao->linha][posicao->coluna] = item;
    return 1;
}

bool PossuiTunelMapa(tMapa* mapa){
    return (mapa->tunel != NULL);
}

bool AcessouTunelMapa(tMapa* mapa, tPosicao* posicao){
    return (ObtemItemMapa(mapa, posicao) == '@');
}

void EntraTunelMapa(tMapa* mapa, tPosicao* posicao){
    LevaFinalTunel(mapa->tunel, posicao);
}

void DesalocaMapa(tMapa* mapa){
    for(int i = 0; i < mapa->nLinhas; i++){
        free(mapa->grid[i]);
    }
    free(mapa->grid);
    DesalocaTunel(mapa->tunel);
    free(mapa);
}
