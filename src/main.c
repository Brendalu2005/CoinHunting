#include "fixo.h"
#include "game.h"
#include "ghost.h"
#include "menu.h"
#include "moeda.h"

int main(void)
{
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "CoinHunting");
    SetTargetFPS(60);

    TelaAtual tela = MENU;
    Rectangle botaoIniciar = { WINDOW_WIDTH/2 - 100, 200, 200, 50 };

    const char *JSON = "sprites/json/movimentaçãoPlayer.json";
    Texture2D bgMenu  = LoadTexture("sprites/png/BackgroundMenu.png");
    Texture2D bgJogo  = LoadTexture("sprites/png/backgroundJogo.png");

    Jogador p1 = CriarJogador(JSON, "edu_walk",    (Vector2){100,100});
    Jogador p2 = CriarJogador(JSON, "brenda_walk", (Vector2){600,400});
    Ghost   fantasma = CriarFantasma(JSON, "ghost_walk", (Vector2){300,300});

    Moeda moedas[MAX_MOEDAS];
    float tempoRespawn = 0.0f;
    InicializarMoedas(moedas);
    CarregarTexturasMoedas();
    int opcao = 0;


    while (!WindowShouldClose()) {
        if (tela == MENU) {
            AtualizarMenu(botaoIniciar, &tela, &opcao);
        } else if (tela == JOGO) {
            AtualizarFantasma(&fantasma, p1.posicao, WINDOW_WIDTH, WINDOW_HEIGHT);
            AtualizarJogador(&p1, KEY_W, KEY_S, KEY_A, KEY_D, WINDOW_WIDTH, WINDOW_HEIGHT);
            AtualizarJogador(&p2, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, WINDOW_WIDTH, WINDOW_HEIGHT);
            AtualizarMoedas(moedas, &tempoRespawn);
            
            colisaoMoedas(moedas, &p1);  
            colisaoMoedas(moedas, &p2);  
            VerificarColisaoFantasma(&fantasma, &p1);
            VerificarColisaoFantasma(&fantasma, &p2);   
            
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        if (tela == MENU) {
            DesenharMenu(botaoIniciar, bgMenu, opcao);
        } else if (tela == JOGO) {
            int m = 80;
            DrawTexturePro(bgJogo,
               (Rectangle){m, m, bgJogo.width - 2 * m, bgJogo.height - 2 * m},
               (Rectangle){0, 0, WINDOW_WIDTH, WINDOW_HEIGHT},
               (Vector2){0, 0}, 0, WHITE);

            DesenharJogador(&p1);
            DesenharJogador(&p2);
            DesenharFantasma(&fantasma);
            DesenharMoedas(moedas); 
            DrawText(TextFormat("P1 - Ouro: %d  Prata: %d", p1.moedasOuro, p1.moedasPrata), 10, 10, 20, GOLD);
            DrawText(TextFormat("P2 - Ouro: %d  Prata: %d", p2.moedasOuro, p2.moedasPrata), 10, 40, 20, SKYBLUE);

        }
        EndDrawing();
    }

    DestruirJogador(&p1);
    DestruirJogador(&p2);
    DestruirFantasma(&fantasma);
    UnloadTexturasMoedas(); 
    UnloadTexture(bgMenu);
    UnloadTexture(bgJogo);
    CloseWindow();
    return 0;
}
