#include "tPacman.h"
#include "tMapa.h"
#include "tMovimento.h"
#include "tPosicao.h"
#include <stdio.h>
#include <stdlib.h>

tPacman *CriaPacman(tPosicao *posicao) {
  tPacman *p = (tPacman *)malloc(sizeof(tPacman));
  if (p == NULL) {
    return NULL;
  }
  if (posicao == NULL) {
    return NULL;
  }
  p->posicaoAtual = posicao;

  p->nColisoesParedeBaixo = 0;
  p->nFrutasComidasBaixo = 0;
  p->nMovimentosBaixo = 0;

  p->nColisoesParedeCima = 0;
  p->nFrutasComidasCima = 0;
  p->nMovimentosCima = 0;

  p->nColisoesParedeDireita = 0;
  p->nFrutasComidasDireita = 0;
  p->nMovimentosDireita = 0;

  p->nColisoesParedeEsquerda = 0;
  p->nFrutasComidasEsquerda = 0;
  p->nMovimentosEsquerda = 0;

  p->estaVivo = 1;

  p->nMovimentosSignificativos = 0;
  p->historicoDeMovimentosSignificativos = NULL;

  p->trilha = NULL;
  return p;
}

tPacman *ClonaPacman(tPacman *pacman) {
  tPacman *p = (tPacman *)malloc(sizeof(tPacman));
  p->posicaoAtual = ClonaPosicao(pacman->posicaoAtual);
  return p;
}

tMovimento **ClonaHistoricoDeMovimentosSignificativosPacman(tPacman *pacman) {
  tMovimento **m = (tMovimento **)malloc(pacman->nMovimentosSignificativos *
                                         sizeof(tMovimento *));
  for (int i = 0; i < pacman->nMovimentosSignificativos; i++) {
    m[i] = pacman->historicoDeMovimentosSignificativos[i];
  }
  return m;
}

tPosicao *ObtemPosicaoPacman(tPacman *pacman) { return pacman->posicaoAtual; }

int EstaVivoPacman(tPacman *pacman) { return pacman->estaVivo; }

void MovePacman(tPacman *pacman, tMapa *mapa, COMANDO comando) {
  tPosicao *p = pacman->posicaoAtual;
  if (comando == 1) {
    p->linha--;
    if (EncontrouParedeMapa(mapa, p)) {
      p->linha++;
      pacman->nColisoesParedeCima++;
    } else if (EncontrouComidaMapa(mapa, p)) {
      pacman->nFrutasComidasCima++;
      AtualizaItemMapa(mapa, p, ' ');
    } else if(PossuiTunelMapa(mapa)){
      if(AcessouTunelMapa(mapa, p)){
        EntraTunelMapa(mapa, p);
      } 
    }
    AtualizaPosicao(pacman->posicaoAtual, p);
    pacman->nMovimentosCima++;
  }

  if (comando == 2) {
    p->linha++;
    if (EncontrouParedeMapa(mapa, p)) {
      p->linha--;
      pacman->nColisoesParedeBaixo++;
    } else if (EncontrouComidaMapa(mapa, p)) {
      pacman->nFrutasComidasBaixo++;
      AtualizaItemMapa(mapa, p, ' ');
    } else if(PossuiTunelMapa(mapa)){
      if(AcessouTunelMapa(mapa, p)){
        EntraTunelMapa(mapa, p);
      } 
    }
    AtualizaPosicao(pacman->posicaoAtual, p);
    pacman->nMovimentosBaixo++;
  }

  if (comando == 0) {
    p->coluna--;
    if (EncontrouParedeMapa(mapa, p)) {
      p->coluna++;
      pacman->nColisoesParedeEsquerda++;
    } else if (EncontrouComidaMapa(mapa, p)) {
      pacman->nFrutasComidasEsquerda++;
      AtualizaItemMapa(mapa, p, ' ');
    } else if(PossuiTunelMapa(mapa)){
      if(AcessouTunelMapa(mapa, p)){
        EntraTunelMapa(mapa, p);
      } 
    }
    AtualizaPosicao(pacman->posicaoAtual, p);
    pacman->nMovimentosEsquerda++;
  }

  if (comando == 3) {
    p->coluna++;
    if (EncontrouParedeMapa(mapa, p)) {
      p->coluna--;
      pacman->nColisoesParedeDireita++;
    } else if (EncontrouComidaMapa(mapa, p)) {
      pacman->nFrutasComidasDireita++;
      AtualizaItemMapa(mapa, p, ' ');
    } else if(PossuiTunelMapa(mapa)){
      if(AcessouTunelMapa(mapa, p)){
        EntraTunelMapa(mapa, p);
      } 
    }
    AtualizaPosicao(pacman->posicaoAtual, p);
    pacman->nMovimentosDireita++;
  }
  AtualizaTrilhaPacman(pacman);
}

void CriaTrilhaPacman(tPacman *pacman, int nLinhas, int nColunas) {
  if (pacman->trilha == NULL) {
    pacman->trilha = malloc(nLinhas * sizeof(int *));
    for (int i = 0; i < nLinhas; i++) {
      pacman->trilha[i] = malloc(nColunas * sizeof(int));
      for (int j = 0; j < nColunas; j++) {
        pacman->trilha[i][j] = -1;
      }
    }
  }
}

void AtualizaTrilhaPacman(tPacman *pacman) {
  pacman->trilha[ObtemLinhaPosicao(ObtemPosicaoPacman(pacman))]
                [ObtemColunaPosicao(ObtemPosicaoPacman(pacman))] =
      ObtemNumeroAtualMovimentosPacman(pacman);
}

void SalvaTrilhaPacman(tPacman *pacman) {}

void InsereNovoMovimentoSignificativoPacman(tPacman *pacman, COMANDO comando,
                                            const char *acao) {
  pacman->nMovimentosSignificativos++;
  pacman->historicoDeMovimentosSignificativos =
      realloc(pacman->historicoDeMovimentosSignificativos,
              pacman->nMovimentosSignificativos * sizeof(tMovimento *));
  pacman->historicoDeMovimentosSignificativos
      [pacman->nMovimentosSignificativos++ - 1] =
      CriaMovimento(ObtemNumeroAtualMovimentosPacman(pacman), comando, acao);
}

void MataPacman(tPacman* pacman){
  pacman->estaVivo = 0;
}

void DesalocaPacman(tPacman* pacman){
  DesalocaPosicao(pacman->posicaoAtual);
  for(int i = 0; i < pacman->nMovimentosSignificativos; i++){
    DesalocaMovimento(pacman->historicoDeMovimentosSignificativos[i]);
  }
  free(pacman->historicoDeMovimentosSignificativos);
  for(int j = 0; j < pacman->nLinhasTrilha; j++){
    free(pacman->trilha[j]);
  }
  free(pacman->trilha);
  free(pacman);
}

int ObtemNumeroAtualMovimentosPacman(tPacman* pacman){
  return (pacman->nMovimentosCima + pacman->nMovimentosBaixo + pacman->nMovimentosDireita + pacman->nMovimentosEsquerda);
}

int ObtemNumeroMovimentosSemPontuarPacman(tPacman* pacman){
  return (ObtemNumeroAtualMovimentosPacman(pacman) - ObtemPontuacaoAtualPacman(pacman));
}

int ObtemNumeroColisoesParedePacman(tPacman* pacman){
  return (pacman->nColisoesParedeBaixo + pacman->nColisoesParedeCima + pacman->nColisoesParedeDireita + pacman->nColisoesParedeEsquerda);
}

int ObtemNumeroMovimentosBaixoPacman(tPacman* pacman){
  return pacman->nMovimentosBaixo;
}

int ObtemNumeroFrutasComidasBaixoPacman(tPacman* pacman){
  return pacman->nFrutasComidasBaixo;
}

int ObtemNumeroColisoesParedeBaixoPacman(tPacman* pacman){
  return pacman->nColisoesParedeBaixo;
}

int ObtemNumeroMovimentosCimaPacman(tPacman* pacman){
  return pacman->nMovimentosCima;
}

int ObtemNumeroFrutasComidasCimaPacman(tPacman* pacman){
  return pacman->nFrutasComidasCima;
}

int ObtemNumeroColisoesParedeCimaPacman(tPacman* pacman){
  return pacman->nColisoesParedeCima;
}

int ObtemNumeroMovimentosEsquerdaPacman(tPacman* pacman){
  return pacman->nMovimentosEsquerda;
}

int ObtemNumeroFrutasComidasEsquerdaPacman(tPacman* pacman){
  return pacman->nFrutasComidasEsquerda;
}

int ObtemNumeroColisoesParedeEsquerdaPacman(tPacman* pacman){
  return pacman->nColisoesParedeEsquerda;
}

int ObtemNumeroMovimentosDireitaPacman(tPacman* pacman){
  return pacman->nMovimentosDireita;
}

int ObtemNumeroFrutasComidasDireitaPacman(tPacman* pacman){
  return pacman->nFrutasComidasDireita;
}

int ObtemNumeroColisoesParedeDireitaPacman(tPacman* pacman){
  return pacman->nColisoesParedeDireita;
}

int ObtemNumeroMovimentosSignificativosPacman(tPacman* pacman){
  return pacman->nMovimentosSignificativos;
}

int ObtemPontuacaoAtualPacman(tPacman* pacman){
  return (pacman->nFrutasComidasBaixo + pacman->nFrutasComidasCima + pacman->nFrutasComidasDireita + pacman->nFrutasComidasEsquerda);
}