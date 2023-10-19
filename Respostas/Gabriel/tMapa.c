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
    fscanf(mapa, "%d\n", &m->nMaximoMovimentos);

    int linha = 1, coluna = 0, fruta = 0, tunel = 0;
    
    m->grid = NULL;
    char c;
    m->grid = realloc(m->grid, linha);
    m->grid[0] = NULL;
    while(1){
        fscanf(mapa, "%c", &c);
        if(c == '\n'){
            break;
        }
        if(c == '*'){
            fruta++;
        }
        if(c == '@'){
            if(tunel == 0){
                tPosicao* p1 = CriaPosicao(0, coluna - 1);
                tunel++;
            }
            else{
                tPosicao* p2 = CriaPosicao(0, coluna - 1);
                tunel++;
            }
        }
        coluna++;
        m->grid[0] = realloc(m->grid[0], coluna);
        m->grid[0][coluna - 1] = c;
    }
    while(fscanf(mapa, "%c", &c) == 1){
        linha++;
        m->grid = realloc(m->grid, linha);
        m->grid[linha - 1] == NULL;
        m->grid[linha - 1] = realloc(m->grid[linha - 1], coluna);
        m->grid[linha - 1][0] = c;
        for(int j = 1; j < coluna; j++){
            fscanf(mapa, "%c", &c);
            if(c == '*'){
            fruta++;
            }
            if(c == '@'){
                if(tunel == 0){
                    tPosicao* p1 = CriaPosicao(0, coluna - 1);
                    tunel++;
                }
                else{
                    tPosicao* p2 = CriaPosicao(0, coluna - 1);
                }
            }
            m->grid[linha - 1][j] = c;
        }
        fscanf(mapa, "\n");
    }

    
}