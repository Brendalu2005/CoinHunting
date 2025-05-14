#include "raylib.h"

int main(void) {
    // Define a resolução da janela
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    // Inicializa a janela
    InitWindow(screenWidth, screenHeight, "CoinHunting");

    // Carrega a imagem de fundo
    Texture2D fundo = LoadTexture("sprites/png/backgroundJogo2.png");

    // Verifica se a imagem foi carregada corretamente
    if (fundo.id == 0) {
        TraceLog(LOG_ERROR, "Erro ao carregar imagem de fundo.");
        CloseWindow();
        return 1;
    }

    // Loop principal
    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK); // Cor de fundo para fallback

        // Redimensiona e desenha o fundo
        DrawTexturePro(
            fundo,
            (Rectangle){ 0, 0, (float)fundo.width, (float)fundo.height },
            (Rectangle){ 0, 0, screenWidth, screenHeight },
            (Vector2){ 0, 0 },
            0.0f,
            WHITE
        );

        // Aqui você pode desenhar outras coisas, como o personagem, HUD, etc.

        EndDrawing();
    }

    // Libera os recursos
    UnloadTexture(fundo);
    CloseWindow();

    return 0;
}
