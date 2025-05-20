#include "moeda.h"
#include "fixo.h"
#include "game.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

float tempoVidaMoeda = 13.0f;

Texture2D moedaPrataTex;
Texture2D moedaOuroTex;

Sound somMoeda;

void CarregarSonsMoedas(void) {
    somMoeda = LoadSound("audio/coinCatch.wav");
}

void DescarregarSonsMoedas(void) {
    UnloadSound(somMoeda);
}

void CarregarTexturasMoedas(void) {
    moedaPrataTex = LoadTexture("sprites/png/moeda_prata.png");
    moedaOuroTex  = LoadTexture("sprites/png/moeda_ouro.png");
}

void UnloadTexturasMoedas(void) {
    UnloadTexture(moedaPrataTex);
    UnloadTexture(moedaOuroTex);
}

void InicializarMoedas(Moeda moedas[]) {
    srand(time(NULL));
    for (int i = 0; i < MAX_MOEDAS; i++) {
        moedas[i].ativa     = false;
        moedas[i].posicao   = (Vector2){0, 0};
        moedas[i].tipo      = PRATA;
        moedas[i].tempoVida = 0.0f;
    }
}

void AtualizarTempoVidaMoeda(float tempoTotalJogo) {
    float novaVida = 13.0f - floorf(tempoTotalJogo / 30.0f);
    tempoVidaMoeda = fmaxf(2.0f, novaVida);
}

void AtualizarMoedas(Moeda moedas[], float *tempoRespawn, float tempoTotalJogo) {
    AtualizarTempoVidaMoeda(tempoTotalJogo);
    *tempoRespawn += GetFrameTime();

    if (*tempoRespawn >= 2.0f) {
        *tempoRespawn = 0;

        for (int i = 0; i < MAX_MOEDAS; i++) {
            if (!moedas[i].ativa) {
                if (rand() % 100 < 70) {
                    moedas[i].ativa     = true;
                    moedas[i].posicao   = (Vector2){rand() % (WINDOW_WIDTH - 32), rand() % (WINDOW_HEIGHT - 32)};
                    moedas[i].tipo      = (rand() % 100 < 80) ? PRATA : OURO;
                    moedas[i].tempoVida = 0.0f;
                    break;
                }
            }
        }
    }

    for (int i = 0; i < MAX_MOEDAS; i++) {
        if (moedas[i].ativa) {
            moedas[i].tempoVida += GetFrameTime();
            if (moedas[i].tempoVida >= tempoVidaMoeda) moedas[i].ativa = false;
        }
    }
}

void DesenharMoedas(Moeda moedas[]) {
    for (int i = 0; i < MAX_MOEDAS; i++) {
        if (!moedas[i].ativa) continue;

        Texture2D tex = (moedas[i].tipo == PRATA) ? moedaPrataTex : moedaOuroTex;
        float tempoRestante = tempoVidaMoeda - moedas[i].tempoVida;
        Color cor = WHITE;

        if (tempoRestante <= 2.0f) {
            float piscar = fmodf(GetTime(), 0.4f);
            if (piscar < 0.2f) cor.a = 100;
        }

        if (moedas[i].tipo == OURO)
            DrawTextureEx(tex, moedas[i].posicao, 0.0f, 1.2f, cor);
        else
            DrawTextureV(tex, moedas[i].posicao, cor);
    }
}

void colisaoMoedas(Moeda moedas[], Jogador *jogador, Sound somMoeda) {
    for (int i = 0; i < MAX_MOEDAS; i++) {
        if (moedas[i].ativa && CheckCollisionCircles(jogador->posicao, 16, moedas[i].posicao, 14)) {
            if (moedas[i].tipo == OURO) {
                jogador->moedasOuro++;
                PlaySound(somMoeda);
            } else {
                jogador->moedasPrata++;
                PlaySound(somMoeda);
            }
            moedas[i].ativa = false;
        }
    }
}
