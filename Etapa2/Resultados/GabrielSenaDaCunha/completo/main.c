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

void ExplodeFantasma(tPosicao *p, tFantasma *F){
  if(F != NULL){
    if(EstaVivoFantasma(F)){
      if(SaoIguaisPosicao(p, ObtemPosicaoFantasma(F))){
        MataFantasma(F);
      }
    }
  }
}

void Explode(tMapa *mapa, int l, int c, tFantasma *P, tFantasma *I, tFantasma *B, tFantasma *C, tFantasma *H, tFantasma *V, tPacman *pacman){
  if(l != 0 && l != ObtemNumeroLinhasMapa(mapa) - 1 && c != 0 && c != ObtemNumeroColunasMapa(mapa)){
  tPosicao *p = CriaPosicao(l, c);
  ExplodeFantasma(p, P);
  ExplodeFantasma(p, I);
  ExplodeFantasma(p, B);
  ExplodeFantasma(p, C);
  ExplodeFantasma(p, H);
  ExplodeFantasma(p, V);
  if(SaoIguaisPosicao(ObtemPosicaoPacman(pacman), p)){
    MataPacman(pacman);
  }
  AtualizaItemMapa(mapa, p, ' ');
  DesalocaPosicao(p);
  }
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

  tPosicao *mais = ObtemPosicaoItemMapa(mapa, '+');

  // cria fantasmas
  tPosicao *pB = ObtemPosicaoItemMapa(mapa, 'B');
  tFantasma *B = CriaFantasma(pB, 'B');

  tPosicao *pP = ObtemPosicaoItemMapa(mapa, 'P');
  tFantasma *P = CriaFantasma(pP, 'P');

  tPosicao *pI = ObtemPosicaoItemMapa(mapa, 'I');
  tFantasma *I = CriaFantasma(pI, 'I');

  tPosicao *pC = ObtemPosicaoItemMapa(mapa, 'C');
  tFantasma *C = CriaFantasma(pC, 'C');

  tPosicao *pH = ObtemPosicaoItemMapa(mapa, 'H');
  tFantasma *H = CriaFantasma(pH, 'H');

  tPosicao *pV = ObtemPosicaoItemMapa(mapa, 'V');
  tFantasma *V = CriaFantasma(pV, 'V');

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
    if(mais != NULL){
      if(SaoIguaisPosicao(mais, ObtemPosicaoPacman(pacman))){
        ComecaContagemMais(pacman);
        free(mais);
        mais = NULL;
      }
    }
   
    if(g != 0){ //criar posicao anterior do pacman para analisar se morreu
      anterior = ClonaPosicao(ObtemPosicaoPacman(pacman));
    }
    
    if(PossuiTunelMapa(mapa)){ // caso o mapa possua tunel
      if(EntrouTunel(ObtemTunelMapa(mapa), ObtemPosicaoPacman(pacman))){ //se o pacman estava em cima de um tunel, volta ele pro mapa
        AtualizaItemMapa(mapa, ObtemPosicaoPacman(pacman), '@'); 
      }
      else {
        if(PacmanParede(pacman)){
          AtualizaItemMapa(mapa, ObtemPosicaoPacman(pacman), '#'); 
        }
        else {
          AtualizaItemMapa(mapa, ObtemPosicaoPacman(pacman), ' '); // caso nao, coloca um espaco vazio
        }
        
      }
    }
    else if(PacmanParede(pacman)){
      AtualizaItemMapa(mapa, ObtemPosicaoPacman(pacman), '#'); 
    }
    else {
      AtualizaItemMapa(mapa, ObtemPosicaoPacman(pacman), ' '); // caso nao, coloca um espaco vazio
    }

    DiminuiContagemX(pacman);

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

    if(TemComida(mapa, P, pacman) || TemComida(mapa, B, pacman) || TemComida(mapa, C, pacman) || TemComida(mapa, I, pacman) || TemComida(mapa, H, pacman) || TemComida(mapa, V, pacman)){ //analisa caso tenha comida na posicao que o pacman se moveu e exista um fantasma em cima dela
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
    if(H != NULL)
      if(EstaVivoFantasma(H))
        if(ParedeFantasma(H))
          AtualizaItemMapa(mapa, ObtemPosicaoFantasma(H), '#');

    if(V != NULL)
      if(EstaVivoFantasma(V))
        if(ParedeFantasma(V))
          AtualizaItemMapa(mapa, ObtemPosicaoFantasma(V), '#');

    MoveFantasma(P, mapa);
    MoveFantasma(B, mapa);
    MoveFantasma(I, mapa);
    MoveFantasma(C, mapa);
    MoveFantasma(V, mapa);
    MoveFantasma(H, mapa);

    Morreu(P, pacman, anterior);
    Morreu(B, pacman, anterior);
    Morreu(I, pacman, anterior);
    Morreu(C, pacman, anterior);
    Morreu(V, pacman, anterior);
    Morreu(H, pacman, anterior);

    free(anterior);
    anterior = NULL;

    if(ContagemBomba(pacman) >= 0){
      if(ContagemBomba(pacman) == 3)
        AtualizaItemMapa(mapa, BombaPacman(pacman), '3');
      if(ContagemBomba(pacman) == 2)
        AtualizaItemMapa(mapa, BombaPacman(pacman), '2');
      if(ContagemBomba(pacman) == 1)
        AtualizaItemMapa(mapa, BombaPacman(pacman), '1');
      if(ContagemBomba(pacman) == 0){
        Explode(mapa, ObtemLinhaPosicao(BombaPacman(pacman)) - 1 , ObtemColunaPosicao(BombaPacman(pacman)) - 1, P, I, B, C, H, V, pacman);
        Explode(mapa, ObtemLinhaPosicao(BombaPacman(pacman)) - 1, ObtemColunaPosicao(BombaPacman(pacman)), P, I, B, C, H, V, pacman);
        Explode(mapa, ObtemLinhaPosicao(BombaPacman(pacman)) - 1, ObtemColunaPosicao(BombaPacman(pacman)) + 1, P, I, B, C, H, V, pacman);
        Explode(mapa, ObtemLinhaPosicao(BombaPacman(pacman)), ObtemColunaPosicao(BombaPacman(pacman)) - 1, P, I, B, C, H, V, pacman);
        Explode(mapa, ObtemLinhaPosicao(BombaPacman(pacman)), ObtemColunaPosicao(BombaPacman(pacman)), P, I, B, C, H, V, pacman);
        Explode(mapa, ObtemLinhaPosicao(BombaPacman(pacman)), ObtemColunaPosicao(BombaPacman(pacman)) + 1, P, I, B, C, H, V, pacman);
        Explode(mapa, ObtemLinhaPosicao(BombaPacman(pacman)) + 1, ObtemColunaPosicao(BombaPacman(pacman)) - 1, P, I, B, C, H, V, pacman);
        Explode(mapa, ObtemLinhaPosicao(BombaPacman(pacman)) + 1, ObtemColunaPosicao(BombaPacman(pacman)), P, I, B, C, H, V, pacman);
        Explode(mapa, ObtemLinhaPosicao(BombaPacman(pacman)) + 1, ObtemColunaPosicao(BombaPacman(pacman)) + 1, P, I, B, C, H, V, pacman);
        DesalocaPosicao(BombaPacman(pacman));
      }
      DiminuiContagemBomba(pacman);
    }
    if(P != NULL)
      if(EstaVivoFantasma(P))
        AtualizaItemMapa(mapa, ObtemPosicaoFantasma(P), 'P');
    
    if(B != NULL)
      if(EstaVivoFantasma(B))
        AtualizaItemMapa(mapa, ObtemPosicaoFantasma(B), 'B');
    
    if(I != NULL)
      if(EstaVivoFantasma(I))
        AtualizaItemMapa(mapa, ObtemPosicaoFantasma(I), 'I');
    
    if(C != NULL)
      if(EstaVivoFantasma(C))
        AtualizaItemMapa(mapa, ObtemPosicaoFantasma(C), 'C');
    
    if(H != NULL)
      if(EstaVivoFantasma(H))
        AtualizaItemMapa(mapa, ObtemPosicaoFantasma(H), 'H');
    
    if(V != NULL)
      if(EstaVivoFantasma(V))
        AtualizaItemMapa(mapa, ObtemPosicaoFantasma(V), 'V');
    
    if(EstaVivoPacman(pacman)){
      AtualizaItemMapa(mapa, ObtemPosicaoPacman(pacman), '>');
    }

     if(!FazContagemMais(pacman, mapa, acao)){
        AtualizaItemMapa(mapa, ObtemPosicaoPacman(pacman), '#'); 
        printf("Estado do jogo apos o movimento '%c':\n", acao);
        PrintaMapa(mapa);
        printf("Pontuacao: %d\n\n", ObtemPontuacaoAtualPacman(pacman));
        break;
     }

    printf("Estado do jogo apos o movimento '%c':\n", acao);
    PrintaMapa(mapa);
    printf("Pontuacao: %d\n\n", ObtemPontuacaoAtualPacman(pacman));
  }

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
  DesalocaFantasma(H);
  DesalocaFantasma(V);
  DesalocaMapa(mapa);
  DesalocaPacman(pacman);
  if(mais != NULL){
    free(mais);
  }
  return 0;
}
