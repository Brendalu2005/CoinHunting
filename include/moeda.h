#ifndef COIN_H
#define COIN_H

#include "raylib.h"
#include "game.h"
#define MAX_MOEDAS 20

typedef enum { PRATA, OURO } TipoMoeda;

typedef struct {
    Vector2 posicao;
    bool ativa;
    TipoMoeda tipo;
    float tempoVida; 
} Moeda;

extern Texture2D moedaPrataTex;
extern Texture2D moedaOuroTex;

void InicializarMoedas(Moeda moedas[]);
void AtualizarMoedas(Moeda moedas[], float *tempoRespawn);
void DesenharMoedas(Moeda moedas[]);
void CarregarTexturasMoedas(void);
void UnloadTexturasMoedas(void);
void colisaoMoedas(Moeda moedas[], Jogador *jogador);

#endif
