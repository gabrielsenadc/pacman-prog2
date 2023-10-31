#include <stdio.h>
#include <stdlib.h>
#include "tMapa.h"
#include "tPosicao.h"
#include "tTunel.h"
#include "tPosicao.h"
#include "tMovimento.h"
#include "tPacman.h"
#include "tFantasma.h"

typedef struct{
  int comida;
  int parede;
  int qtd;
  char tipo;
} twasd;

void PrintaMapa(tMapa* mapa){
  for(int i = 0; i < mapa->nLinhas; i++){
    for(int j = 0; j < mapa->nColunas; j++){
        printf("%c", mapa->grid[i][j]);
    }
    printf("\n");
  }
}

void PrintaEstatistica(tPacman* pacman){
  FILE* file;
  file = fopen("estatisticas.txt", "w");
  fprintf(file, "Numero de movimentos: %d\n", ObtemNumeroAtualMovimentosPacman(pacman));
  fprintf(file, "Numero de movimentos sem pontuar: %d\n", ObtemNumeroMovimentosSemPontuarPacman(pacman));
  fprintf(file, "Numero de colisoes com parede: %d\n", ObtemNumeroColisoesParedePacman(pacman));
  fprintf(file, "Numero de movimentos para baixo: %d\nNumero de movimentos para cima: %d\nNumero de movimentos para esquerda: %d\nNumero de movimentos para direita: %d\n", 
                ObtemNumeroMovimentosBaixoPacman(pacman), ObtemNumeroMovimentosCimaPacman(pacman), ObtemNumeroMovimentosEsquerdaPacman(pacman), ObtemNumeroMovimentosDireitaPacman(pacman));
  fclose(file);
}

void PrintaResumo(tMovimento** lista, int qtd){
  FILE* file;
  file = fopen("resumo.txt", "w");
  for(int i = 0; i < qtd; i++){
    if(lista[i]->comando == 0){
      fprintf(file, "Movimento %d (a) %s\n", lista[i]->numeroDoMovimento, lista[i]->acao);
    }
    if(lista[i]->comando == 1){
      fprintf(file, "Movimento %d (w) %s\n", lista[i]->numeroDoMovimento, lista[i]->acao);
    }
    if(lista[i]->comando == 2){
      fprintf(file, "Movimento %d (s) %s\n", lista[i]->numeroDoMovimento, lista[i]->acao);
    }
    if(lista[i]->comando == 3){
      fprintf(file, "Movimento %d (d) %s\n", lista[i]->numeroDoMovimento, lista[i]->acao);
    }

  }
  fclose(file);
}

int Maior(twasd* m1, twasd* m2) {
  if (m1->comida > m2->comida) {
    return 1;
  }
  if (m1->comida < m2->comida) {
    return 0;
  }
  if (m1->comida == m2->comida) {
    if (m1->parede < m2->parede) {
      return 1;
    }
    if (m1->parede > m2->parede) {
      return 0;
    }
    if (m1->parede == m2->parede) {
      if (m1->qtd > m2->qtd) {
        return 1;
      }
      if (m1->qtd < m2->qtd) {
        return 0;
      }
      if (m1->qtd == m2->qtd) {
        if (m1->tipo < m2->tipo) {
          return 1;
        }
        if (m1->tipo > m2->tipo) {
          return 0;
        }
      }
    }
  }
}


void TrocaSeAcharMaior(twasd * vet, int tam, twasd * paraTrocar){
      for(twasd *p = paraTrocar + 1; p < vet + tam; p++){
          if(Maior(p, paraTrocar)){
              twasd aux = *p;
              *p = *paraTrocar;
              *paraTrocar = aux;
          }
      }
  }

void PrintaRanking(tPacman* pacman) {
  twasd vet[4];

  vet[0].comida = ObtemNumeroFrutasComidasBaixoPacman(pacman);
  vet[0].tipo = 's';
  vet[0].parede = ObtemNumeroColisoesParedeBaixoPacman(pacman);
  vet[0].qtd = ObtemNumeroMovimentosBaixoPacman(pacman);

  vet[1].comida = ObtemNumeroFrutasComidasCimaPacman(pacman);
  vet[1].tipo = 'w';
  vet[1].parede = ObtemNumeroColisoesParedeCimaPacman(pacman);
  vet[1].qtd = ObtemNumeroMovimentosCimaPacman(pacman);

  vet[2].comida = ObtemNumeroFrutasComidasDireitaPacman(pacman);
  vet[2].tipo = 'd';
  vet[2].parede = ObtemNumeroColisoesParedeDireitaPacman(pacman);
  vet[2].qtd = ObtemNumeroMovimentosDireitaPacman(pacman);

  vet[3].comida = ObtemNumeroFrutasComidasEsquerdaPacman(pacman);
  vet[3].tipo = 'a';
  vet[3].parede = ObtemNumeroColisoesParedeEsquerdaPacman(pacman);
  vet[3].qtd = ObtemNumeroMovimentosEsquerdaPacman(pacman);

  FILE *pFile;
  pFile = fopen("ranking.txt", "w");
  for(int i = 0; i < 4; i++){
      TrocaSeAcharMaior(vet, 4, &vet[i]);
  }
  for(int i = 0; i < 4; i++){
    fprintf(pFile, "%c,%d,%d,%d\n", vet[i].tipo, vet[i].comida, vet[i].parede,vet[i].qtd);
  }
  fclose(pFile);
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
  int venceu = 0;

  AtualizaTrilhaPacman(pacman);
  for(int g = 0; g < mapa->nMaximoMovimentos; g++){
    if(ObtemQuantidadeFrutasIniciaisMapa(mapa) == ObtemPontuacaoAtualPacman(pacman)){
      printf("Voce venceu!\nPontuacao final: %d", ObtemPontuacaoAtualPacman(pacman));
      venceu = 1;
      break;
    }
    if(!EstaVivoPacman(pacman)){
      break;
    }
    if(g != 0){
      anterior = ClonaPosicao(pacman->posicaoAtual);
    }
    AtualizaItemMapa(mapa, pacman->posicaoAtual, ' ');
    scanf("%c\n", &acao);
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
    
    if(TemComida(mapa, P, pacman) || TemComida(mapa, B, pacman) || TemComida(mapa, C, pacman) || TemComida(mapa, I, pacman)){
      if(acao == 'a'){
        (pacman->nFrutasComidasEsquerda)++;
        InsereNovoMovimentoSignificativoPacman(pacman, 0, "pegou comida");
      }
      if(acao == 'd'){
        (pacman->nFrutasComidasDireita)++;
        InsereNovoMovimentoSignificativoPacman(pacman, 3, "pegou comida");
      }
      if(acao == 's'){
        (pacman->nFrutasComidasBaixo)++;
        InsereNovoMovimentoSignificativoPacman(pacman, 2, "pegou comida");
      }
      if(acao == 'w'){
        (pacman->nFrutasComidasCima)++;
        InsereNovoMovimentoSignificativoPacman(pacman, 1, "pegou comida");
      }

    }

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
    if(mapa->tunel != NULL){
      AtualizaItemMapa(mapa, mapa->tunel->acesso1, '@');
      AtualizaItemMapa(mapa, mapa->tunel->acesso2, '@');
    }
    if(EstaVivoPacman(pacman)){
      AtualizaItemMapa(mapa, pacman->posicaoAtual, '>');
    }
    printf("Estado do jogo apos o movimento '%c':\n", acao);
    PrintaMapa(mapa);
    printf("Pontuacao: %d\n\n", ObtemPontuacaoAtualPacman(pacman));
  }

  if(venceu == 0){
    printf("Game over!\nPontuacao final: %d", ObtemPontuacaoAtualPacman(pacman));
  }
  if(!EstaVivoPacman(pacman)){
    if(acao == 'a'){
      InsereNovoMovimentoSignificativoPacman(pacman, 0, "fim de jogo por encostar em um fantasma");
    }
    if(acao == 'w'){
      InsereNovoMovimentoSignificativoPacman(pacman, 1, "fim de jogo por encostar em um fantasma");
    }
    if(acao == 's'){
      InsereNovoMovimentoSignificativoPacman(pacman, 2, "fim de jogo por encostar em um fantasma");
    }
    if(acao == 'd'){
      InsereNovoMovimentoSignificativoPacman(pacman, 3, "fim de jogo por encostar em um fantasma");
    }
  }

  tMovimento** clone = NULL;
  clone = ClonaHistoricoDeMovimentosSignificativosPacman(pacman);

  SalvaTrilhaPacman(pacman);
  PrintaEstatistica(pacman);
  PrintaResumo(pacman->historicoDeMovimentosSignificativos, pacman->nMovimentosSignificativos);
  PrintaRanking(pacman);

  if(clone != NULL){
    for(int i = 0; i < pacman->nMovimentosSignificativos; i++){
      if(clone[i] != NULL){
        DesalocaMovimento(clone[i]);
      }
    }
    free(clone);
  }
  DesalocaFantasma(P);
  DesalocaFantasma(B);
  DesalocaFantasma(I);
  DesalocaFantasma(C);
  DesalocaMapa(mapa);
  DesalocaPacman(pacman);
  return 0;
}
