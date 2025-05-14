#include "menu.h"

void AtualizarMenu(Rectangle botao, TelaAtual *telaAtual) {
    Vector2 mouse = GetMousePosition();

    if (CheckCollisionPointRec(mouse, botao) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        *telaAtual = JOGO;
    }
}

void DesenharMenu(Rectangle botao, Texture2D background) {
    ClearBackground(RAYWHITE);

    
    Rectangle source = { 0, 0, background.width, background.height };
    Rectangle dest = { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() };
    Vector2 origin = { 0, 0 };
    DrawTexturePro(background, source, dest, origin, 0.0f, WHITE);

    DrawText("JOGO: CAÇA ÀS MOEDAS", 400, 150, 30, DARKGRAY);
    DrawRectangleRec(botao, LIGHTGRAY);
    DrawText("INICIAR", botao.x + 60, botao.y + 15, 20, BLACK);
}
