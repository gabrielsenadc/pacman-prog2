#ifndef TFANTASMA_H_
#define TFANTASMA_H_

#include "tMapa.h"
#include "tPacman.h"
#include "tPosicao.h"

typedef struct tFantasma {
  // psoicao atual fantasma
  tPosicao *posicao;

  // posicao anterior fantasma
  tPosicao *anterior;

  // sentido que o fantasma se mexe
  int esquerda;
  int cima;

  // se o fantasma esta em cima de uma fruta
  int fruta;

  // qual eh o fantasma
  char tipo;

  int vivo;
} tFantasma;

// aloca dinamicamente o fantasma e retorna NULL se ele nao existir
//  \param posicao posicao do fantasma
//  \param tipo char com a letra do fantasma
tFantasma *CriaFantasma(tPosicao *posicao, char tipo);

// anda verticalmente os fantasmas I e P
//  \param F fantasma
//  \param mapa mapa
void AndaVertical(tFantasma *F, tMapa *mapa);

// anda verticalmente os fantasmas B e C
//  \param F fantasma
//  \param mapa mapa
void AndaHorizontal(tFantasma *F, tMapa *mapa);

// move o fantasma caso ele exista(for diferente de NULL) chamando as funcoes
// Anda dependendo do seu tipo
//  \param F fantasma
//  \param mapa mapa
void MoveFantasma(tFantasma *F, tMapa *mapa);

// analisa se o pacman morreu em relacao a um fantasma se ele existir
//  \param F fantasma
//  \param pacman pacman
//  \param anterior posicao anterior do pacman
void Morreu(tFantasma *F, tPacman *pacman, tPosicao *anterior);

// analisa se tem comida na posicao do pacman, caso o pacman va para uma posicao com um fantasma em cima de uma comida
//  \param mapa mapa
//  \param F fantamsa
//  \param pacman pacman
int TemComida(tMapa *mapa, tFantasma *F, tPacman *pacman);

// retorna a posicao do fantasma, caso ele exista
//  \param F fantasma
tPosicao* ObtemPosicaoFantasma(tFantasma *F);

// desaloca fantasma caso ele seja diferente de NULL
//  \param F fantasma
void DesalocaFantasma(tFantasma *F);

void ExplodeFantasma(tPosicao *p, tFantasma *F);

#endif