#include "moeda.h"
#include "fixo.h"
#include "game.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdio.h>

float tempoVidaMoeda = 13.0f;

#define MOEDA_LARGURA 27
#define MOEDA_ALTURA 26

Texture2D moedaPrataTex;
Texture2D moedaOuroTex;


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
        moedas[i].ativa        = false;
        moedas[i].posicao      = (Vector2){0, 0};
        moedas[i].tipo         = PRATA;
        moedas[i].tempoVida    = 0.0f;
        moedas[i].duracaoVida  = 0.0f;
    }
}


void AtualizarTempoVidaMoeda(float tempoTotalJogo) {
    float novaVida = 13.0f - floorf(tempoTotalJogo / 30.0f);
    tempoVidaMoeda = fmaxf(5.0f, novaVida);
}

void AtualizarMoedas(Moeda moedas[], float *tempoRespawn, float tempoTotalJogo,  Rectangle areaJogo) {
    AtualizarTempoVidaMoeda(tempoTotalJogo);
    *tempoRespawn += GetFrameTime();

    if (*tempoRespawn >= 2.0f) {
        *tempoRespawn = 0;

        for (int i = 0; i < MAX_MOEDAS; i++) {
            if (!moedas[i].ativa) {
                if (rand() % 100 < 70) {
                    moedas[i].ativa     = true;
                    moedas[i].posicao = (Vector2){
                        rand() % (int)(areaJogo.width - 32) + (int)areaJogo.x,
                        rand() % (int)(areaJogo.height - 32) + (int)areaJogo.y
                    };                    
                    if (rand() % 100 < 80) {
                        moedas[i].tipo = PRATA;
                    } else {
                        moedas[i].tipo = OURO;
                    }
                    
                    moedas[i].tempoVida   = 0.0f;
                    moedas[i].duracaoVida = tempoVidaMoeda;
                    break;
                }
            }
        }
    }

    for (int i = 0; i < MAX_MOEDAS; i++) {
        if (moedas[i].ativa) {
            moedas[i].tempoVida += GetFrameTime();
            if (moedas[i].tempoVida >= moedas[i].duracaoVida){
                moedas[i].ativa = false;
            } 
        }
    }
}

void DesenharMoedas(Moeda moedas[]) {
    for (int i = 0; i < MAX_MOEDAS; i++) {
        if (!moedas[i].ativa) continue;

        Texture2D tex;
        if (moedas[i].tipo == PRATA) {
            tex = moedaPrataTex;
        } else {
            tex = moedaOuroTex;
        }

        Color cor = WHITE;

        float tempoRestante = moedas[i].duracaoVida - moedas[i].tempoVida;

        if (tempoRestante <= 3.0f) {
            float piscar = fmodf(GetTime(), 0.4f);
            if (piscar < 0.3f) cor.a = 100;
        }

        if (moedas[i].tipo == OURO){
            DrawTextureEx(tex, moedas[i].posicao, 0.0f, 1.2f, cor);
        }else{
            DrawTextureV(tex, moedas[i].posicao, cor);
        }
    }
}

#define JOGADOR_HITBOX_LARGURA 16
#define JOGADOR_HITBOX_ALTURA  20

Rectangle getHitboxJogador(Jogador *jogador) {
    return (Rectangle){
        jogador->posicao.x,
        jogador->posicao.y,
        jogador->direita[0].width,
        jogador->baixo[0].height
    };
}

void colisaoMoedas(Moeda moedas[], Jogador *jogador, Sound somMoeda) {
    Rectangle hitboxJogador = getHitboxJogador(jogador);

    for (int i = 0; i < MAX_MOEDAS; i++) {
        if (!moedas[i].ativa) continue;

        Rectangle rectMoeda = {
            moedas[i].posicao.x - MOEDA_LARGURA / 2.0f,
            moedas[i].posicao.y - MOEDA_ALTURA / 2.0f,
            MOEDA_LARGURA,
            MOEDA_ALTURA
        };

        if (CheckCollisionRecs(hitboxJogador, rectMoeda)) {

            if (moedas[i].tipo == OURO) {
                jogador->moedasOuro++;
            } else {
                jogador->moedasPrata++;
            }

            PlaySound(somMoeda);
            moedas[i].ativa = false;

            jogador->tempoTextoMoeda = GetTime() + 0.2f;
        }
    }
}



