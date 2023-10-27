#include "tFantasma.h"

tFantasma* CriaFantasma(tPosicao* posicao, char tipo){
  if(posicao == NULL){
    return NULL;
  }
  tFantasma* F = malloc(sizeof(tFantasma));
  F->posicao = posicao;
  F->tipo = tipo;
  F->lanterior = 0;
  F->canterior = 0;
  F->fruta = 0;
  if(tipo == 'P'){
    F->cima = 1;
  }
  if(tipo == 'C'){
    F->esquerda = 0;
  }
  if(tipo == 'I'){
    F->cima = 0;
  }
  if(tipo == 'B'){
    F->esquerda = 1;
  }
  return F;
}

void AndaVertical(tFantasma *F, tMapa *mapa){
  tPosicao *p = ClonaPosicao(F->posicao);
  if(F->cima == 1){
    p->linha--;
    if(EncontrouParedeMapa(mapa, p)){
      F->cima = 0;
      p->linha += 2;
    }
    if(EncontrouComidaMapa(mapa, p)){
      F->fruta = 1;
    }
    AtualizaPosicao(F->posicao, p);
  }
  else {
      p->linha++;
      if(EncontrouParedeMapa(mapa, p)){
        F->cima = 1;
        p->linha -= 2;
      }
      if(EncontrouComidaMapa(mapa, p)){
        F->fruta = 1;
      }
      AtualizaPosicao(F->posicao, p);
  }
  DesalocaPosicao(p);
}

void AndaHorizontal(tFantasma *F, tMapa *mapa){
  tPosicao *p = ClonaPosicao(F->posicao);
  if(F->esquerda == 1){
    p->coluna--;
    if(EncontrouParedeMapa(mapa, p)){
      F->esquerda = 0;
      p->coluna += 2;
    }
    if(EncontrouComidaMapa(mapa, p)){
      F->fruta = 1;
    }
    AtualizaPosicao(F->posicao, p);
  }
  else {
      p->coluna++;
      if(EncontrouParedeMapa(mapa, p)){
        F->esquerda = 1;
        p->coluna -= 2;
      }
      if(EncontrouComidaMapa(mapa, p)){
        F->fruta = 1;
      }
      AtualizaPosicao(F->posicao, p);
  }
  DesalocaPosicao(p);
}

void MoveFantasma(tFantasma *F, tMapa *mapa){
  if(F != NULL){
    F->lanterior = F->posicao->linha;
    F->canterior = F->posicao->coluna;
    if(F->fruta){
      AtualizaItemMapa(mapa, F->posicao, '*');
      F->fruta = 0;
    }
    if(F->tipo == 'P' || F->tipo == 'I'){
      AndaVertical(F, mapa);
    }
    if(F->tipo == 'B' || F->tipo == 'C'){
      AndaHorizontal(F, mapa);
    }
  } 
}

void Morreu(tFantasma *F, tPacman *pacman, tPosicao *anterior){
  if(F!= NULL){
    if(SaoIguaisPosicao(F->posicao, pacman->posicaoAtual)) {
       MataPacman(pacman);
     }
    if(anterior != NULL){
      if(anterior->linha == F->posicao->linha && pacman->posicaoAtual->linha == F->lanterior && anterior->coluna == F->posicao->coluna && pacman->posicaoAtual->coluna == F->canterior){
        MataPacman(pacman);
      }
    }
  }
}

void DesalocaFantamsa(tFantasma *F){
  if(F != NULL){
    DesalocaPosicao(F->posicao);
    free(F);
  }
}