// telaDeFundo.c
#include "raylib.h"

void MostrarTelaDeFundo() {
    // Inicialize a janela (se ainda não tiver feito em outro lugar)
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "CoinHunting");

    // Carrega a imagem de fundo
    Texture2D fundo = LoadTexture("sprites/png/backgroundJogo.png");

    // Loop principal do jogo
    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK); // ou qualquer outra cor que preferir

        DrawTexture(fundo, 0, 0, WHITE); // Desenha o fundo

        EndDrawing();
    }

    // Libera recursos
    UnloadTexture(fundo);
    CloseWindow();
}
