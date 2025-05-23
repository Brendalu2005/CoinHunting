#ifndef JOGADOR_H
#define JOGADOR_H
#define MAX_TEXTS 50

#include "raylib.h"
#include "fixo.h"
#include <stdbool.h>

typedef enum { UP, DOWN, LEFT, RIGHT } Direction;

typedef struct TextoPerda {
    bool active;
    Vector2 posicao;
    float Congelado;
    char texto[32];
} TextoPerda;


typedef struct {
    Vector2  posicao;
    float    velocidade;
    Direction direcaoAtual;
    float    tempoFrame;
    float    temporizador;
    int      indiceFrame;
    int     moedasPrata;
    int     moedasOuro;
    float tempoUltimaColisaoGhost2;
    float tempoTextoMoeda;
    Texture2D cima   [MAX_FRAMES_CIMA_BAIXO];
    Texture2D baixo  [MAX_FRAMES_CIMA_BAIXO];
    Texture2D esquerda[MAX_FRAMES_LADO];
    Texture2D direita [MAX_FRAMES_LADO];
} Jogador;


Jogador CriarJogador(const char *jsonPath, const char *nomeNoJson, Vector2 posicaoInicial);

void AtualizarJogador(Jogador *j, int upKey, int downKey,int leftKey, int rightKey,  Rectangle areaJogo);
void AtualizarAnimacao(Jogador *j, int andando);
void DesenharJogador (Jogador *j);
void DestruirJogador (Jogador *j);

#endif
