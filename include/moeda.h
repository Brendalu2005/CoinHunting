#ifndef COIN_H
#define COIN_H

#include "raylib.h"

#define MAX_MOEDAS 20

typedef enum { PRATA, OURO } TipoMoeda;

typedef struct {
    Vector2 posicao;
    bool ativa;
    TipoMoeda tipo;
    float tempoVida;  // novo: tempo em segundos que a moeda está ativa
} Moeda;

extern Texture2D moedaPrataTex;
extern Texture2D moedaOuroTex;

void InicializarMoedas(Moeda moedas[]);
void AtualizarMoedas(Moeda moedas[], float *tempoRespawn);
void DesenharMoedas(Moeda moedas[]);
void CarregarTexturasMoedas(void);
void UnloadTexturasMoedas(void);

#endif
