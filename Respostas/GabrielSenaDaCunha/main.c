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
} twasd; //coloca as informacoes de cada sentido(w, a, s, d) em uma struct para analisar o ranking

typedef struct{
  int contagem;
  tPosicao* p;
} tBomba;
void PrintaMapa(tMapa* mapa){
  for(int i = 0; i < ObtemNumeroLinhasMapa(mapa); i++){
    for(int j = 0; j < ObtemNumeroColunasMapa(mapa); j++){
        tPosicao * posicao = CriaPosicao(i, j);
        printf("%c", ObtemItemMapa(mapa, posicao));
        DesalocaPosicao(posicao);
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
    if(ObtemComandoMovimento(lista[i]) == 0){
      fprintf(file, "Movimento %d (a) %s\n", ObtemNumeroMovimento(lista[i]), ObtemAcaoMovimento(lista[i]));
    }
    if(ObtemComandoMovimento(lista[i]) == 1){
      fprintf(file, "Movimento %d (w) %s\n", ObtemNumeroMovimento(lista[i]), ObtemAcaoMovimento(lista[i]));
    }
    if(ObtemComandoMovimento(lista[i]) == 2){
      fprintf(file, "Movimento %d (s) %s\n", ObtemNumeroMovimento(lista[i]), ObtemAcaoMovimento(lista[i]));
    }
    if(ObtemComandoMovimento(lista[i]) == 3){
      fprintf(file, "Movimento %d (d) %s\n", ObtemNumeroMovimento(lista[i]), ObtemAcaoMovimento(lista[i]));
    }

  }
  fclose(file);
}

int Maior(twasd* m1, twasd* m2) { // compara qual dos sentidos(w, a, s, d) estao em uma posicao maior no ranking
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


void TrocaSeAcharMaior(twasd * vet, int tam, twasd * paraTrocar){  //ordena os sentidos(w, a, s, d)
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

  // atribui as informacoes dos sentidos(w, a, s, d) na struct para ser analisado posteriormente
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

void Explode(tMapa *mapa, int l, int c, tFantasma *P, tFantasma *I, tFantasma *B, tFantasma *C, tPacman *pacman){
  tPosicao* p = CriaPosicao(l, c);
  ExplodeFantasma(p, P);
  ExplodeFantasma(p, I);
  ExplodeFantasma(p, B);
  ExplodeFantasma(p, C);
  if(SaoIguaisPosicao(p, pacman->posicaoAtual))
    MataPacman(pacman);
  AtualizaItemMapa(mapa, p, ' ');
  DesalocaPosicao(p);
}

int main(int argc, char *argv[]) {
  if(argc <=1){
    printf("ERRO: O diretorio de arquivos de configuracao nao foi informado");
    return 1;
  } //verificar se recebeu o diretorio

  tMapa* mapa = CriaMapa(argv[1]);
  if(mapa == NULL){ //nao criar o mapa 
    return 1;
  }

  // inicializacao
  FILE* inicializacao;
  inicializacao = fopen("inicializacao.txt", "w"); 
  int i = 0, j = 0;
  for(i = 0; i < ObtemNumeroLinhasMapa(mapa); i++){
    for(j = 0; j < ObtemNumeroColunasMapa(mapa); j++){
      tPosicao * posicao = CriaPosicao(i, j);
      fprintf(inicializacao, "%c", ObtemItemMapa(mapa, posicao));
      DesalocaPosicao(posicao);
    }
    fprintf(inicializacao, "\n");
  }
  tPosicao *ppacman = ObtemPosicaoItemMapa(mapa, '>');
  tPacman* pacman = CriaPacman(ppacman);
  fprintf(inicializacao, "Pac-Man comecara o jogo na linha %d e coluna %d", ObtemLinhaPosicao(ObtemPosicaoPacman(pacman)) + 1, ObtemColunaPosicao(ObtemPosicaoPacman(pacman)) + 1);
  fclose(inicializacao);

  // cria fantasmas
  tPosicao *pB = ObtemPosicaoItemMapa(mapa, 'B');
  tFantasma *B = CriaFantasma(pB, 'B');

  tPosicao *pP = ObtemPosicaoItemMapa(mapa, 'P');
  tFantasma *P = CriaFantasma(pP, 'P');

  tPosicao *pI = ObtemPosicaoItemMapa(mapa, 'I');
  tFantasma *I = CriaFantasma(pI, 'I');

  tPosicao *pC = ObtemPosicaoItemMapa(mapa, 'C');
  tFantasma *C = CriaFantasma(pC, 'C');

  int qtdBombas = -1;
  tBomba **bombas = NULL;

  CriaTrilhaPacman(pacman, ObtemNumeroLinhasMapa(mapa),ObtemNumeroColunasMapa(mapa));

  tPosicao* anterior = NULL;
  char acao = '\0';
  int venceu = 0;

  AtualizaTrilhaPacman(pacman);
  for(int g = 0; g < ObtemNumeroMaximoMovimentosMapa(mapa); g++){
    if(ObtemQuantidadeFrutasIniciaisMapa(mapa) == ObtemPontuacaoAtualPacman(pacman)){
      printf("Voce venceu!\nPontuacao final: %d", ObtemPontuacaoAtualPacman(pacman));
      venceu = 1;
      break;
    }
    if(!EstaVivoPacman(pacman)){
      break;
    }
    if(g != 0){ //criar posicao anterior do pacman para analisar se morreu
      anterior = ClonaPosicao(ObtemPosicaoPacman(pacman));
    }
    
    printf("Estado do jogo apos o movimento '%c':\n", acao);
    PrintaMapa(mapa);
    printf("Pontuacao: %d\n\n", ObtemPontuacaoAtualPacman(pacman));

    if(PossuiTunelMapa(mapa)){ // caso o mapa possua tunel
      if(EntrouTunel(ObtemTunelMapa(mapa), ObtemPosicaoPacman(pacman))){ //se o pacman estava em cima de um tunel, volta ele pro mapa
        AtualizaItemMapa(mapa, ObtemPosicaoPacman(pacman), '@'); 
      }
      else {
        AtualizaItemMapa(mapa, ObtemPosicaoPacman(pacman), ' '); // caso nao, coloca um espaco vazio
      }
    }
    else{ //se nao tiver tunel, coloca um espaco vazio onde o pacman estava
      AtualizaItemMapa(mapa, ObtemPosicaoPacman(pacman), ' '); 
    }

    scanf("%c%*c", &acao);
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
    if(ObtemItemMapa(mapa, pacman->posicaoAtual) == '&'){
      qtdBombas++;
      bombas = realloc(bombas, (qtdBombas + 1) * sizeof(tBomba*));
      bombas[qtdBombas] = malloc(sizeof(tBomba));
      bombas[qtdBombas]->contagem = 3;
      bombas[qtdBombas]->p = CriaPosicao(ObtemLinhaPosicao(pacman->posicaoAtual), ObtemColunaPosicao(pacman->posicaoAtual));
    }

    if(TemComida(mapa, P, pacman) || TemComida(mapa, B, pacman) || TemComida(mapa, C, pacman) || TemComida(mapa, I, pacman)){ //analisa caso tenha comida na posicao que o pacman se moveu e exista um fantasma em cima dela
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

    for(int a = 0; a <= qtdBombas; a++){
    if(bombas[a]->p != NULL){
      if(bombas[a]->contagem == 3)
        AtualizaItemMapa(mapa, bombas[a]->p, '3');
      else if(bombas[a]->contagem == 2)
        AtualizaItemMapa(mapa, bombas[a]->p, '2');
      else if(bombas[a]->contagem == 1)
        AtualizaItemMapa(mapa, bombas[a]->p, '1');
      else if(bombas[a]->contagem == 0){
        Explode(mapa, ObtemLinhaPosicao(bombas[a]->p) - 1, ObtemColunaPosicao(bombas[a]->p) - 1, P, I, B, C, pacman);
        Explode(mapa, ObtemLinhaPosicao(bombas[a]->p) - 1, ObtemColunaPosicao(bombas[a]->p), P, I, B, C, pacman);
        Explode(mapa, ObtemLinhaPosicao(bombas[a]->p) - 1, ObtemColunaPosicao(bombas[a]->p) + 1, P, I, B, C, pacman);
        Explode(mapa, ObtemLinhaPosicao(bombas[a]->p), ObtemColunaPosicao(bombas[a]->p) - 1, P, I, B, C, pacman);
        Explode(mapa, ObtemLinhaPosicao(bombas[a]->p), ObtemColunaPosicao(bombas[a]->p), P, I, B, C, pacman);
        Explode(mapa, ObtemLinhaPosicao(bombas[a]->p), ObtemColunaPosicao(bombas[a]->p) + 1, P, I, B, C, pacman);
        Explode(mapa, ObtemLinhaPosicao(bombas[a]->p) + 1, ObtemColunaPosicao(bombas[a]->p) - 1, P, I, B, C, pacman);
        Explode(mapa, ObtemLinhaPosicao(bombas[a]->p) + 1, ObtemColunaPosicao(bombas[a]->p), P, I, B, C, pacman);
        Explode(mapa, ObtemLinhaPosicao(bombas[a]->p) + 1, ObtemColunaPosicao(bombas[a]->p) + 1, P, I, B, C, pacman);
        DesalocaPosicao(bombas[a]->p);
        bombas[a]->p = NULL;
      }
      bombas[a]->contagem--;
    }
    }

    if(P != NULL)
      if(P->vivo)
        AtualizaItemMapa(mapa, ObtemPosicaoFantasma(P), 'P');
    
    if(B != NULL)
      if(B->vivo)
        AtualizaItemMapa(mapa, ObtemPosicaoFantasma(B), 'B');
    
    if(I != NULL)
      if(I->vivo)
        AtualizaItemMapa(mapa, ObtemPosicaoFantasma(I), 'I');
    
    if(C != NULL)
      if(C->vivo)
        AtualizaItemMapa(mapa, ObtemPosicaoFantasma(C), 'C');
    
    if(EstaVivoPacman(pacman)){
      AtualizaItemMapa(mapa, ObtemPosicaoPacman(pacman), '>');
    }

  }

  printf("Estado do jogo apos o movimento '%c':\n", acao);
  PrintaMapa(mapa);
  printf("Pontuacao: %d\n\n", ObtemPontuacaoAtualPacman(pacman));

  if(venceu == 0){ //caso o pacman tenha perdido
    printf("Game over!\nPontuacao final: %d", ObtemPontuacaoAtualPacman(pacman));
  }
  if(!EstaVivoPacman(pacman)){ //caso o pacman tenha morrido
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
  PrintaResumo(clone, ObtemNumeroMovimentosSignificativosPacman(pacman));
  PrintaRanking(pacman);

  if(clone != NULL){
    for(int i = 0; i < ObtemNumeroMovimentosSignificativosPacman(pacman); i++){
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
  for(int b = 0; b <= qtdBombas; b++){
    free(bombas[b]);
  }
  free(bombas);
  return 0;
}
