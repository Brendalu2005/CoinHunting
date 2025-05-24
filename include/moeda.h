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
    float duracaoVida;
} Moeda;

extern Texture2D moedaPrataTex;
extern Texture2D moedaOuroTex;
extern Sound somMoeda;

void InicializarMoedas(Moeda moedas[]);
void AtualizarMoedas(Moeda moedas[], float *tempoRespawn, float tempoTotalJogo, Rectangle areaJogo);
void DesenharMoedas(Moeda moedas[]);
void CarregarTexturasMoedas(void);
void UnloadTexturasMoedas(void);
void CarregarSonsMoedas(void);
void DescarregarSonsMoedas(void);
void colisaoMoedas(Moeda moedas[], Jogador *jogador, Sound somMoeda);

#endif
