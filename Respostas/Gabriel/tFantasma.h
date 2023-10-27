#ifndef TFANTASMA_H_
#define TFANTASMA_H_

#include "tPosicao.h"
#include "tMapa.h"
#include "tPacman.h"

typedef struct tFantasma{
  tPosicao* posicao;
  int lanterior;
  int canterior;
  int esquerda;
  int cima;
  int fruta;
  char tipo;
} tFantasma;

tFantasma* CriaFantasma(tPosicao* posicao, char tipo);

void AndaVertical(tFantasma *F, tMapa *mapa);

void AndaHorizontal(tFantasma *F, tMapa *mapa);

void MoveFantasma(tFantasma *F, tMapa *mapa);

void Morreu(tFantasma *F, tPacman *pacman, tPosicao *anterior);

void DesalocaFantamsa(tFantasma *F);


#endif