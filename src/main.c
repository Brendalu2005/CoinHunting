#include "fixo.h"
#include "game.h"
#include "ghost.h"
#include "menu.h"
#include "moeda.h"
#include "selecao.h"
#include <stdio.h>
#include <math.h>

int main(void) {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "CoinHunting");
    SetTargetFPS(60);

    TelaAtual tela = MENU;

    Rectangle botaoIniciar     = { WINDOW_WIDTH / 2 - 100, 200, 200, 50 };
    Rectangle botao1Jogador    = { WINDOW_WIDTH / 2 - 110, 200, 220, 50 };
    Rectangle botao2Jogadores  = { WINDOW_WIDTH / 2 - 110, 270, 220, 50 };

    const char *JSON = "sprites/json/movimentaçãoPlayer.json";
    Texture2D bgMenu = LoadTexture("sprites/png/BackgroundMenu.png");
    Texture2D bgJogo = LoadTexture("sprites/png/backgroundJogo.png");

    Jogador p1 = CriarJogador(JSON, "edu_walk",    (Vector2){100, 100});
    Jogador p2 = CriarJogador(JSON, "brenda_walk", (Vector2){600, 400});
    Jogador p3 = CriarJogador(JSON, "guto_walk",   (Vector2){100, 100});
    Ghost fantasma = CriarFantasma(JSON, "ghost_walk", (Vector2){300, 300});

    Moeda moedas[MAX_MOEDAS];
    float tempoRespawn = 0.0f;
    InicializarMoedas(moedas);
    CarregarTexturasMoedas();

    int opcaoMenu = 0;
    int opcaojogadores = -1;

    int tempoRestante = 180;
    float tempoAcumulado = 0.0f;

    bool jogoFinalizado = false;
    float tempoFimDeJogo = 0.0f;
    const float duracaoFimDeJogo = 3.0f;

    while (!WindowShouldClose()) {
        if (tela == JOGO && tempoRestante > 0 && !jogoFinalizado) {
            tempoAcumulado += GetFrameTime();
            if (tempoAcumulado >= 1.0f) {
                tempoRestante--;
                tempoAcumulado = 0.0f;
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch (tela) {
            case MENU:
                AtualizarMenu(botaoIniciar, &tela, &opcaoMenu);
                DesenharMenu(botaoIniciar, bgMenu, opcaoMenu);
                break;

                case SELECAO:
                AtualizarSelecaoJogadores(botao1Jogador, botao2Jogadores, &tela, &opcaojogadores);
                DesenharSelecaoJogadores(botao1Jogador, botao2Jogadores, bgMenu, opcaojogadores);
            
                if (tela == JOGO) {
                    if (opcaojogadores == 0 || opcaojogadores == 1) {
                        opcaojogadores += 1;
                    } else {
                        opcaojogadores = 1;
                    }
                }
                break;
            

            case SOBRE:
                AtualizarSobre(&tela);
                DesenharSobre(bgMenu);
                break;

            case JOGO:
                AtualizarMoedas(moedas, &tempoRespawn);
                DrawTexture(bgJogo, 0, 0, WHITE);
                DesenharMoedas(moedas); 
                DesenharFantasma(&fantasma);

                if (!jogoFinalizado) {
                    if (opcaojogadores == 1) {
                        AtualizarFantasma(&fantasma, p3.posicao, WINDOW_WIDTH, WINDOW_HEIGHT);
                        AtualizarJogador(&p3, KEY_W, KEY_S, KEY_A, KEY_D, WINDOW_WIDTH, WINDOW_HEIGHT);
                        colisaoMoedas(moedas, &p3);  
                        VerificarColisaoFantasma(&fantasma, &p3);
                    } else if (opcaojogadores == 2) {
                        AtualizarFantasma(&fantasma, p1.posicao, WINDOW_WIDTH, WINDOW_HEIGHT);
                        AtualizarJogador(&p1, KEY_W, KEY_S, KEY_A, KEY_D, WINDOW_WIDTH, WINDOW_HEIGHT);
                        AtualizarJogador(&p2, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, WINDOW_WIDTH, WINDOW_HEIGHT);
                        colisaoMoedas(moedas, &p1);  
                        colisaoMoedas(moedas, &p2);  
                        VerificarColisaoFantasma(&fantasma, &p1);
                        VerificarColisaoFantasma(&fantasma, &p2);
                    }

                    if (opcaojogadores == 1) {
                        DesenharJogador(&p3); 
                        int pontuacao = p3.moedasPrata * 1 + p3.moedasOuro * 2;
                        DrawText(TextFormat("P1 - Pontuação: %d", pontuacao), 10, 10, 20, GOLD);
                    } else if (opcaojogadores == 2) {
                        DesenharJogador(&p1);
                        DesenharJogador(&p2);
                        int pontuacao1 = p1.moedasPrata * 1 + p1.moedasOuro * 2;
                        int pontuacao2 = p2.moedasPrata * 1 + p2.moedasOuro * 2;
                        DrawText(TextFormat("P1 - Pontuação: %d", pontuacao1), 10, 10, 20, GOLD);
                        int larguraTexto = MeasureText(TextFormat("P2 - Pontuação: %d", pontuacao2), 20);
                        DrawText(TextFormat("P2 - Pontuação: %d", pontuacao2), GetScreenWidth() - larguraTexto - 10, 10, 20, SKYBLUE);
                    }

                    int minutos = tempoRestante / 60;
                    int segundos = tempoRestante % 60;
                    char textoTimer[20];
                    sprintf(textoTimer, "%02d:%02d", minutos, segundos);
                    int larguraTimer = MeasureText(textoTimer, 30);

                    Color corTimer = WHITE;

                    if (tempoRestante <= 30) {
                        float alpha = 0.5f + 0.5f * sinf(GetTime() * 6.0f);
                        corTimer = (Color){255, 0, 0, (unsigned char)(alpha * 255)};
                    }
                    else if (tempoRestante <= 60) {
                        corTimer = ORANGE;
                    }
                    else if (tempoRestante <= 100) {
                        corTimer = YELLOW;
                    }
                    
                    DrawText(textoTimer, GetScreenWidth() / 2 - larguraTimer / 2, 10, 30, corTimer);


                    if (tempoRestante <= 0) {
                        jogoFinalizado = true;
                        tempoFimDeJogo = 0.0f;
                    }
                } else {
                    const char* msg = "FIM DE JOGO";
                    int largura = MeasureText(msg, 50);
                    DrawText(msg, GetScreenWidth() / 2 - largura / 2, GetScreenHeight() / 2 - 25, 50, RED);

                    tempoFimDeJogo += GetFrameTime();
                    if (tempoFimDeJogo >= duracaoFimDeJogo) {
                        tela = MENU;
                        opcaojogadores = -1;
                        jogoFinalizado = false;

                        InicializarMoedas(moedas);
                        p1.moedasPrata = p1.moedasOuro = 0;
                        p2.moedasPrata = p2.moedasOuro = 0;
                        p3.moedasPrata = p3.moedasOuro = 0;

                        tempoRestante = 180;
                        tempoAcumulado = 0.0f;
                    }
                }

                break;
        }

        EndDrawing();
    }

    DestruirJogador(&p1);
    DestruirJogador(&p2);
    DestruirJogador(&p3);
    DestruirFantasma(&fantasma);
    UnloadTexturasMoedas(); 
    UnloadTexture(bgMenu);
    UnloadTexture(bgJogo);
    CloseWindow();

    return 0;
}
