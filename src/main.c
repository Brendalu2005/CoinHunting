#include "fixo.h"
#include "game.h"
#include "ghost.h"
#include "menu.h"
#include "moeda.h"
#include "selecao.h"


int main(void)
{
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "CoinHunting");
    SetTargetFPS(60);

    TelaAtual tela = MENU;
    Rectangle botaoIniciar = { WINDOW_WIDTH/2 - 100, 200, 200, 50 };
    Rectangle botao1Jogador = { WINDOW_WIDTH/2 - 110, 200, 220, 50 };
    Rectangle botao2Jogadores = { WINDOW_WIDTH/2 - 110, 270, 220, 50 };


    const char *JSON = "sprites/json/movimentaçãoPlayer.json";
    Texture2D bgMenu  = LoadTexture("sprites/png/BackgroundMenu.png");
    Texture2D bgJogo  = LoadTexture("sprites/png/backgroundJogo.png");

    Jogador p1 = CriarJogador(JSON, "edu_walk",    (Vector2){100,100});
    Jogador p2 = CriarJogador(JSON, "brenda_walk", (Vector2){600,400});
    Jogador p3 = CriarJogador(JSON, "guto_walk", (Vector2){100,100});
    Ghost   fantasma = CriarFantasma(JSON, "ghost_walk", (Vector2){300,300});

    Moeda moedas[MAX_MOEDAS];
    float tempoRespawn = 0.0f;
    InicializarMoedas(moedas);
    CarregarTexturasMoedas();
    int opcaoMenu = 0;
    int opcaojogadores = -1;


    while (!WindowShouldClose()) {
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
            break;

        case SOBRE:
            AtualizarSobre(&tela);
            DesenharSobre(bgMenu);
            break;

        case JOGO:
            AtualizarMoedas(moedas, &tempoRespawn);
            if (opcaojogadores== 1) {
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

            DrawTexture(bgJogo, 0, 0, WHITE);
            DesenharMoedas(moedas); 
            DesenharFantasma(&fantasma);

            if (opcaojogadores == 1) {
                DesenharJogador(&p3);
                DrawText(TextFormat("P1 - Ouro: %d  Prata: %d", p3.moedasOuro, p3.moedasPrata), 10, 10, 20, GOLD);
            } else if (opcaojogadores == 2) {
                DesenharJogador(&p1);
                DesenharJogador(&p2);
                DrawText(TextFormat("P1 - Ouro: %d  Prata: %d", p1.moedasOuro, p1.moedasPrata), 10, 10, 20, GOLD);
                DrawText(TextFormat("P2 - Ouro: %d  Prata: %d", p2.moedasOuro, p2.moedasPrata), 10, 40, 20, SKYBLUE);
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
