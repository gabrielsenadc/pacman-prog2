#include <stdio.h>
#include "tMapa.h"
#include "tPosicao.h"
#include "tTunel.h"
#include "tPosicao.h"
#include "tMovimento.h"
#include "tFantasma.h"
#include "tPacman.h"

void PrintaMapa(tMapa* mapa){
  for(int i = 0; i < mapa->nLinhas; i++){
    for(int j = 0; j < mapa->nColunas; j++){
      if(j == mapa->nColunas--){
        printf("%c", mapa->grid[i][j]);
      }
      else{
        printf("%c ", mapa->grid[i][j]);
      }
    }
    printf("\n");
  }
}

int main(int argc, char *argv[]) {
  if(argc <=1){
    printf("ERRO: O diretorio de arquivos de configuracao nao foi informado");
    return 1;
  } //verificar se recebeu o diretorio
  char diretorio[1001];
  sprintf(diretorio, "%s", argv[1]); //transormar o diretorio em uma string
  tMapa* mapa = CriaMapa(diretorio);
  
  tPosicao *ppacman = ObtemPosicaoItemMapa(mapa, '>');
  tPacman *pacman = CriaPacman(ppacman);
  
  tPosicao *pB = ObtemPosicaoItemMapa(mapa, 'B');
  tFantasma *B = CriaFantasma(pB, 'B');

  tPosicao *pP = ObtemPosicaoItemMapa(mapa, 'P');
  tFantasma *P = CriaFantasma(pB, 'P');

  tPosicao *pI = ObtemPosicaoItemMapa(mapa, 'I');
  tFantasma *I = CriaFantasma(pB, 'I');

  tPosicao *pC = ObtemPosicaoItemMapa(mapa, 'C');
  tFantasma *C = CriaFantasma(pB, 'C');
  
  int venceu = 0;
  char acao = '\0';

  tPosicao* anterior = NULL;
  
  for(int g = 0; g < mapa->nMaximoMovimentos; g++){
    if(ObtemQuantidadeFrutasIniciaisMapa(mapa) == ObtemPontuacaoAtualPacman(pacman)){
      venceu = 1;
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

    MoveFantasma(P);
    MoveFantasma(B);
    MoveFantasma(I);
    MoveFantasma(C);

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
  }

  PrintaMapa(mapa);
}