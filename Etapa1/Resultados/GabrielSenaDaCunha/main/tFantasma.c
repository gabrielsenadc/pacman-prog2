#include "tFantasma.h"

tFantasma *CriaFantasma(tPosicao *posicao, char tipo) {
  if (posicao == NULL) { // caso o fantasma nao exista
    return NULL;
  }
  tFantasma *F = malloc(sizeof(tFantasma));
  F->posicao = posicao;
  F->tipo = tipo;
  F->anterior = CriaPosicao(0, 0);
  F->fruta = 0;
  if (tipo == 'P') { // comeca andando para cima
    F->cima = 1;
  }
  if (tipo == 'C') { // comeca andando para direita
    F->esquerda = 0;
  }
  if (tipo == 'I') { // comeca andando para baixo
    F->cima = 0;
  }
  if (tipo == 'B') { // comeca andando para esquerda
    F->esquerda = 1;
  }
  F->tunel = 0;
  return F;
}

void AndaVertical(tFantasma *F, tMapa *mapa) {
  tPosicao *p = ClonaPosicao(F->posicao); // clona posicao a ser analisada anda pra cima
  if (F->cima == 1) {                     
    p->linha--;
    if (EncontrouParedeMapa(mapa, p)) { // caso bata com a parede muda de sentido

      F->cima = 0;
      p->linha += 2;
    }
    if (EncontrouComidaMapa(mapa, p)) { // caso esteja em cima de uma fruta
      F->fruta = 1;
    }
    AtualizaPosicao(F->posicao, p); // atualizando a posicao do fantasma com a posicao analisada
  } else {  // anda pra baixo
    p->linha++;
    if (EncontrouParedeMapa(mapa, p)) {
      F->cima = 1;
      p->linha -= 2;
    }
    if (EncontrouComidaMapa(mapa, p)) {
      F->fruta = 1;
    }
    AtualizaPosicao(F->posicao, p);
  }
  DesalocaPosicao(p); // desaloca a posicao analisada
}

void AndaHorizontal(tFantasma *F, tMapa *mapa) {
  tPosicao *p = ClonaPosicao(F->posicao);
  if (F->esquerda == 1) { // anda para esquerda
    p->coluna--;
    if (EncontrouParedeMapa(mapa, p)) {
      F->esquerda = 0;
      p->coluna += 2;
    }
    if (EncontrouComidaMapa(mapa, p)) {
      F->fruta = 1;
    }
    AtualizaPosicao(F->posicao, p);
  } else { // anda para direita
    p->coluna++;
    if (EncontrouParedeMapa(mapa, p)) {
      F->esquerda = 1;
      p->coluna -= 2;
    }
    if (EncontrouComidaMapa(mapa, p)) {
      F->fruta = 1;
    }
    AtualizaPosicao(F->posicao, p);
  }
  DesalocaPosicao(p);
}

void MoveFantasma(tFantasma *F, tMapa *mapa) {
  if (F != NULL) {

    // atualiza a posicao anterior
    AtualizaPosicao(F->anterior, F->posicao);

    if (F->fruta) { // caso estivesse em cima de uma fruta, coloca ela no mapa
      AtualizaItemMapa(mapa, F->posicao, '*');
      F->fruta = 0;
    } else if (F->tunel) {
      AtualizaItemMapa(mapa, F->posicao, '@');
      F->tunel = 0;
    } else {
      AtualizaItemMapa(mapa, F->posicao, ' '); // caso nao, coloca um espaco em branco no mapa
    }
    if (F->tipo == 'P' || F->tipo == 'I') { // se for o fantasma I ou P, anda verticalmente
      AndaVertical(F, mapa);
    }
    if (F->tipo == 'B' || F->tipo == 'C') { // se for o fantasma C ou B, anda horizontalme
      AndaHorizontal(F, mapa);
    }
    if(ObtemItemMapa(mapa, ObtemPosicaoFantasma(F)) == '@')
      F->tunel = 1;
  }
}

void Morreu(tFantasma *F, tPacman *pacman, tPosicao *anterior) {
  if (F != NULL) {
    if (SaoIguaisPosicao(F->posicao, pacman->posicaoAtual)) { // caso os dois estejam na mesma posicao
      MataPacman(pacman);
    }
    if (anterior != NULL) {
      if (SaoIguaisPosicao(F->anterior, pacman->posicaoAtual) && SaoIguaisPosicao(F->posicao, anterior)) { // caso eles se cruzaram
        MataPacman(pacman);
      }
    }
  }
}

int TemComida(tMapa *mapa, tFantasma *F, tPacman *pacman) {
  if (F != NULL) {
    if (SaoIguaisPosicao(pacman->posicaoAtual, F->posicao)) {
      if (F->fruta == 1) {
        return 1;
      }
    }
  }
  return 0;
}

tPosicao* ObtemPosicaoFantasma(tFantasma *F){
  return F->posicao;
}

void DesalocaFantasma(tFantasma *F) {
  if (F != NULL) {
    DesalocaPosicao(F->posicao);
    DesalocaPosicao(F->anterior);
    free(F);
  }
}