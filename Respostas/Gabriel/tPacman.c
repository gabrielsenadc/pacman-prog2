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
  p->nLinhasTrilha = 0;
  p->nColunasTrilha = 0;
  return p;
}

tPacman *ClonaPacman(tPacman *pacman) {
  tPosicao *posicao = CriaPosicao(ObtemLinhaPosicao(pacman->posicaoAtual), ObtemColunaPosicao(pacman->posicaoAtual));
  tPacman *p = CriaPacman(posicao); 
  return p;
}

tMovimento **ClonaHistoricoDeMovimentosSignificativosPacman(tPacman *pacman) {
  tMovimento **m = (tMovimento **)malloc(pacman->nMovimentosSignificativos *
                                         sizeof(tMovimento *));
  for (int i = 0; i < pacman->nMovimentosSignificativos; i++) {
    m[i] = CriaMovimento(ObtemNumeroMovimento(pacman->historicoDeMovimentosSignificativos[i]),ObtemComandoMovimento(pacman->historicoDeMovimentosSignificativos[i]), ObtemAcaoMovimento(pacman->historicoDeMovimentosSignificativos[i]));
  }
  return m;
}

tPosicao *ObtemPosicaoPacman(tPacman *pacman) { return pacman->posicaoAtual; }

int EstaVivoPacman(tPacman *pacman) { return pacman->estaVivo; }

void MovePacman(tPacman *pacman, tMapa *mapa, COMANDO comando) {
  tPosicao *p = pacman->posicaoAtual; //cria posicao (nao dinamicamente) para ser analisada
  if (comando == 1) {
    pacman->nMovimentosCima++;
    p->linha--;
    if (EncontrouParedeMapa(mapa, p)) {
      p->linha++;
      pacman->nColisoesParedeCima++;
      InsereNovoMovimentoSignificativoPacman(pacman, comando, "colidiu com a parede");
    } else if (EncontrouComidaMapa(mapa, p)) {
      pacman->nFrutasComidasCima++;
      InsereNovoMovimentoSignificativoPacman(pacman, comando, "pegou comida");
      AtualizaItemMapa(mapa, p, ' '); //tira comida do mapa
    } else if(PossuiTunelMapa(mapa)){
      if(AcessouTunelMapa(mapa, p)){
        AtualizaTrilhaPacman(pacman); //se entrar no tunel, atualiza a trilha nessa posicao tambem
        EntraTunelMapa(mapa, p);
      } 
    }
    AtualizaPosicao(pacman->posicaoAtual, p); //atualiza a posicao do pacman com a posicao analisada
  }

  if (comando == 2) {
    p->linha++;
    pacman->nMovimentosBaixo++;
    if (EncontrouParedeMapa(mapa, p)) {
      p->linha--;
      pacman->nColisoesParedeBaixo++;
      InsereNovoMovimentoSignificativoPacman(pacman, comando, "colidiu com a parede");
    } else if (EncontrouComidaMapa(mapa, p)) {
      pacman->nFrutasComidasBaixo++;
      InsereNovoMovimentoSignificativoPacman(pacman, comando, "pegou comida");
      AtualizaItemMapa(mapa, p, ' ');
    } else if(PossuiTunelMapa(mapa)){
      if(AcessouTunelMapa(mapa, p)){
        AtualizaTrilhaPacman(pacman);
        EntraTunelMapa(mapa, p);
      } 
    }
    AtualizaPosicao(pacman->posicaoAtual, p);
  }

  if (comando == 0) {
    p->coluna--;
    pacman->nMovimentosEsquerda++;
    if (EncontrouParedeMapa(mapa, p)) {
      p->coluna++;
      pacman->nColisoesParedeEsquerda++;
      InsereNovoMovimentoSignificativoPacman(pacman, comando, "colidiu com a parede");
    } else if (EncontrouComidaMapa(mapa, p)) {
      pacman->nFrutasComidasEsquerda++;
      InsereNovoMovimentoSignificativoPacman(pacman, comando, "pegou comida");
      AtualizaItemMapa(mapa, p, ' ');
    } else if(PossuiTunelMapa(mapa)){
      if(AcessouTunelMapa(mapa, p)){
        AtualizaTrilhaPacman(pacman);
        EntraTunelMapa(mapa, p);
      } 
    }
    AtualizaPosicao(pacman->posicaoAtual, p);
  }

  if (comando == 3) {
    p->coluna++;
     pacman->nMovimentosDireita++;
    if (EncontrouParedeMapa(mapa, p)) {
      p->coluna--;
      pacman->nColisoesParedeDireita++;
      InsereNovoMovimentoSignificativoPacman(pacman, comando, "colidiu com a parede");
    } else if (EncontrouComidaMapa(mapa, p)) {
      pacman->nFrutasComidasDireita++;
      InsereNovoMovimentoSignificativoPacman(pacman, comando, "pegou comida");
      AtualizaItemMapa(mapa, p, ' ');
    } else if(PossuiTunelMapa(mapa)){
      if(AcessouTunelMapa(mapa, p)){
        AtualizaTrilhaPacman(pacman);
        EntraTunelMapa(mapa, p);
      } 
    }
    AtualizaPosicao(pacman->posicaoAtual, p);
  }
  AtualizaTrilhaPacman(pacman);
}

void CriaTrilhaPacman(tPacman *pacman, int nLinhas, int nColunas) {
  pacman->nLinhasTrilha = nLinhas;
  pacman->nColunasTrilha = nColunas;
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

void SalvaTrilhaPacman(tPacman *pacman) {
  FILE* trilha;
  trilha = fopen("trilha.txt", "w");
  for(int i= 0; i < pacman->nLinhasTrilha; i++){
    for(int j = 0; j < pacman->nColunasTrilha; j++){
      if(pacman->trilha[i][j] == -1){ //se o pacman nao passou nessa posicao
        if(j == pacman->nColunasTrilha - 1){
          fprintf(trilha, "#");
        }
        else{
          fprintf(trilha, "# ");
        }
      }
      else{
        fprintf(trilha, "%d ", pacman->trilha[i][j]);
      }
    }
    fprintf(trilha, "\n");
  }
  fclose(trilha);
}

void InsereNovoMovimentoSignificativoPacman(tPacman *pacman, COMANDO comando,
                                            const char *acao) {
  pacman->nMovimentosSignificativos++;
  pacman->historicoDeMovimentosSignificativos = realloc(pacman->historicoDeMovimentosSignificativos, pacman->nMovimentosSignificativos * sizeof(tMovimento *));
  pacman->historicoDeMovimentosSignificativos[pacman->nMovimentosSignificativos - 1] = NULL;
  pacman->historicoDeMovimentosSignificativos[pacman->nMovimentosSignificativos - 1] = CriaMovimento(ObtemNumeroAtualMovimentosPacman(pacman), comando, acao);
}

void MataPacman(tPacman* pacman){
  pacman->estaVivo = 0;
}

void DesalocaPacman(tPacman* pacman){
  if(pacman != NULL){
  DesalocaPosicao(pacman->posicaoAtual); 
  if(pacman->historicoDeMovimentosSignificativos != NULL){
    for(int i = 0; i < pacman->nMovimentosSignificativos; i++){
      if(pacman->historicoDeMovimentosSignificativos[i] != NULL){
        DesalocaMovimento(pacman->historicoDeMovimentosSignificativos[i]);
      }
    }
    free(pacman->historicoDeMovimentosSignificativos);
  }
  if(pacman->trilha != NULL){
    for(int j = 0; j < pacman->nLinhasTrilha; j++){
      if(pacman->trilha[j] != NULL){
        free(pacman->trilha[j]);
      }
    }
    free(pacman->trilha);
  }
  free(pacman);
  }
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