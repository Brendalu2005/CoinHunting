#include "raylib.h"

int main(void) {
    
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    
    InitWindow(screenWidth, screenHeight, "CoinHunting");

   
    Texture2D fundo = LoadTexture("sprites/png/backgroundJogo.png");

    
    if (fundo.id == 0) {
        TraceLog(LOG_ERROR, "Erro ao carregar imagem de fundo.");
        CloseWindow();
        return 1;
    }

    
    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK); 

       
        DrawTexturePro(
            fundo,
            (Rectangle){ 0, 0, (float)fundo.width, (float)fundo.height },
            (Rectangle){ 0, 0, screenWidth, screenHeight },
            (Vector2){ 0, 0 },
            0.0f,
            WHITE
        );

        
        EndDrawing();
    }

    
    UnloadTexture(fundo);
    CloseWindow();

    return 0;
}
