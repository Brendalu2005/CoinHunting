#include "fixo.h"
#include "game.h"
#include "ghost.h"
#include "menu.h"

int main(void)
{
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "CoinHunting");
    SetTargetFPS(60);

    TelaAtual tela = MENU;
    Rectangle botaoIniciar = { WINDOW_WIDTH/2 -100, 200, 200, 50 };

    const char *JSON = "sprites/json/movimentaçãoPlayer.json";
    Texture2D bgMenu  = LoadTexture("sprites/png/BackgroundMenu.png");
    Texture2D bgJogo  = LoadTexture("sprites/png/backgroundJogo.png");

    Jogador p1 = CriarJogador(JSON, "edu_walk",    (Vector2){100,100});
    Jogador p2 = CriarJogador(JSON, "brenda_walk", (Vector2){600,400});
    Ghost   fantasma = CriarFantasma(JSON, "ghost_walk", (Vector2){300,300});


    while (!WindowShouldClose()) {
        if (tela == MENU) {
            AtualizarMenu(botaoIniciar, &tela);
        } else if (tela == JOGO) {
            AtualizarFantasma(&fantasma, p1.posicao, WINDOW_WIDTH, WINDOW_HEIGHT);
            AtualizarJogador(&p1, KEY_W, KEY_S, KEY_A, KEY_D,       WINDOW_WIDTH, WINDOW_HEIGHT);
            AtualizarJogador(&p2, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, WINDOW_WIDTH, WINDOW_HEIGHT);
        }

        BeginDrawing();
        if (tela == MENU) {
            DesenharMenu(botaoIniciar, bgMenu);
        } else if (tela == JOGO) {
            int m = 80;
            DrawTexturePro(bgJogo,
               (Rectangle){m,m,bgJogo.width-2*m,bgJogo.height-2*m},
               (Rectangle){0,0,WINDOW_WIDTH,WINDOW_HEIGHT},
               (Vector2){0,0}, 0, WHITE);

            DesenharJogador(&p1);
            DesenharJogador(&p2);
            DesenharFantasma(&fantasma);
        }
        EndDrawing();
    }

    DestruirJogador(&p1);
    DestruirJogador(&p2);
    DestruirFantasma(&fantasma);
    UnloadTexture(bgMenu);
    UnloadTexture(bgJogo);
    CloseWindow();
    return 0;
}
