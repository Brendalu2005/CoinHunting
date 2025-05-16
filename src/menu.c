#include "menu.h"
void AtualizarMenu(Rectangle botao, TelaAtual *telaAtual) {
    Vector2 mouse = GetMousePosition();

    if (CheckCollisionPointRec(mouse, botao) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        *telaAtual = JOGO;
    }
}

void DesenharMenu(Rectangle botao, Texture2D background) {
    ClearBackground(RAYWHITE);

    
    DrawTexturePro(
        background,
        (Rectangle){ 0, 0, background.width, background.height },
        (Rectangle){ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
        (Vector2){ 0, 0 },
        0.0f,
        WHITE
    );

    
    int fonteTitulo = 40;
    const char *titulo = "CoinHunting!";
    int larguraTitulo = MeasureText(titulo, fonteTitulo);
    DrawText(titulo, (GetScreenWidth() - larguraTitulo) / 2, 100, fonteTitulo, DARKGRAY);

   
    int largura = 200;
    int altura = 50;
    int x = GetScreenWidth() / 2 - largura / 2;
    int y = 200;

    DrawRectangle(x, y, largura, altura, LIGHTGRAY);
    DrawText("INICIAR", x + 60, y + 15, 20, BLACK);

    DrawRectangle(x, y + 70, largura, altura, LIGHTGRAY);
    DrawText("SOBRE", x + 65, y + 85, 20, BLACK);

    DrawRectangle(x, y + 140, largura, altura, LIGHTGRAY);
    DrawText("RANKING", x + 55, y + 155, 20, BLACK);
}
