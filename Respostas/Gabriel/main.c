#include <stdio.h>
#include <stdlib.h>
#include "tMapa.h"
#include "tPosicao.h"
#include "tTunel.h"
#include "tPosicao.h"
#include "tMovimento.h"
#include "tPacman.h"
#include "tFantasma.h"


void PrintaMapa(tMapa* mapa){
  for(int i = 0; i < mapa->nLinhas; i++){
    for(int j = 0; j < mapa->nColunas; j++){
        printf("%c ", mapa->grid[i][j]);
    }
    printf("\n");
  }
}

int main(int argc, char *argv[]) {
  if(argc <=1){
    printf("ERRO: O diretorio de arquivos de configuracao nao foi informado");
    return 1;
  } //verificar se recebeu o diretorio

  tMapa* mapa = CriaMapa(argv[1]);
  if(mapa == NULL){
    return 1;
  }

  FILE* inicializacao;
  inicializacao = fopen("inicializacao.txt", "w"); 
  int i = 0, j = 0;
  for(i = 0; i < ObtemNumeroLinhasMapa(mapa); i++){
    for(j = 0; j < ObtemNumeroColunasMapa(mapa); j++){
      fprintf(inicializacao, "%c", mapa->grid[i][j]);;
      
    }
    fprintf(inicializacao, "\n");
  }
  tPosicao *ppacman = ObtemPosicaoItemMapa(mapa, '>');
  tPacman* pacman = CriaPacman(ppacman);
  fprintf(inicializacao, "Pac-Man comecara o jogo na linha %d e coluna %d", ObtemLinhaPosicao(pacman->posicaoAtual) + 1, ObtemColunaPosicao(pacman->posicaoAtual) + 1);
  fclose(inicializacao);

  
  tPosicao *pB = ObtemPosicaoItemMapa(mapa, 'B');
  tFantasma *B = CriaFantasma(pB, 'B');

  tPosicao *pP = ObtemPosicaoItemMapa(mapa, 'P');
  tFantasma *P = CriaFantasma(pP, 'P');

  tPosicao *pI = ObtemPosicaoItemMapa(mapa, 'I');
  tFantasma *I = CriaFantasma(pI, 'I');

  tPosicao *pC = ObtemPosicaoItemMapa(mapa, 'C');
  tFantasma *C = CriaFantasma(pC, 'C');

  CriaTrilhaPacman(pacman, mapa->nLinhas, mapa->nColunas);

  tPosicao* anterior = NULL;
  char acao = '\0';

  for(int g = 0; g < mapa->nMaximoMovimentos; g++){
    if(ObtemQuantidadeFrutasIniciaisMapa(mapa) == ObtemPontuacaoAtualPacman(pacman)){
      break;
    }
    if(!EstaVivoPacman(pacman)){
      break;
    }
    if(g != 0){
      anterior = ClonaPosicao(pacman->posicaoAtual);
    }
    scanf("%c", &acao);
    if(acao == 'a'){
       MovePacman(pacman, mapa, 0);
    }
    if(acao == 'w'){
       MovePacman(pacman, mapa, 1);
    }
    if(acao == 's'){
       MovePacman(pacman, mapa, 2);
    }
    if(acao == 'd'){
       MovePacman(pacman, mapa, 3);
    }
    AtualizaItemMapa(mapa, pacman->posicaoAtual, ' ');

    MoveFantasma(P, mapa);
    MoveFantasma(B, mapa);
    MoveFantasma(I, mapa);
    MoveFantasma(C, mapa);

    Morreu(P, pacman, anterior);
    Morreu(B, pacman, anterior);
    Morreu(I, pacman, anterior);
    Morreu(C, pacman, anterior);
    free(anterior);
    anterior = NULL;

    if(P != NULL){
      AtualizaItemMapa(mapa, P->posicao, 'P');
    }
    if(B != NULL){
      AtualizaItemMapa(mapa, B->posicao, 'B');
    }
    if(I != NULL){
      AtualizaItemMapa(mapa, I->posicao, 'I');
    }
    if(C != NULL){
      AtualizaItemMapa(mapa, C->posicao, 'C');
    }
    if(EstaVivoPacman(pacman)){
      AtualizaItemMapa(mapa, pacman->posicaoAtual, '>');
    }
    printf("Estado do jogo apos o movimento '%c':\n", acao);
    PrintaMapa(mapa);
    printf("Pontuacao: %d\n\n", ObtemPontuacaoAtualPacman(pacman));
  }



  DesalocaFantasma(P);
  DesalocaFantasma(B);
  DesalocaFantasma(I);
  DesalocaFantasma(C);
  DesalocaMapa(mapa);
  DesalocaPacman(pacman);
  return 0;
}
