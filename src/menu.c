#include "menu.h"

#define TOTAL_OPCOES_MENU 3 
void AtualizarMenu(Rectangle botao, TelaAtual *telaAtual, int *opcao) {
    if (IsKeyPressed(KEY_DOWN)) {
        *opcao = (*opcao + 1) % TOTAL_OPCOES_MENU;
    }
    if (IsKeyPressed(KEY_UP)) {
        *opcao = (*opcao - 1 + TOTAL_OPCOES_MENU) % TOTAL_OPCOES_MENU;
    }

    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        switch (*opcao) {
            case 0:
                *telaAtual =  SELECAO;
                break;
            case 1:
                *telaAtual =  SOBRE;
                break;
            case 2:
                 *telaAtual = RANKING;
                break;
        }
    }

}

#define TAM_OPCAO 20
#define TOTAL_OPCOES_MENU 3

char opcoes[TOTAL_OPCOES_MENU][TAM_OPCAO] = {
    "INICIAR",
    "SOBRE",
    "RANKING"
};

void DesenharMenu(Rectangle botao, Texture2D background, int opcao) {
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
    Color corNormal = LIGHTGRAY;
    Color corSelecionada = YELLOW;

    for (int i = 0; i < TOTAL_OPCOES_MENU; i++) {
        Color cor = (i == opcao) ? corSelecionada : corNormal;

        DrawRectangle(x, y + i * 70, largura, altura, cor);

        int textoX = x + (largura - MeasureText(opcoes[i], 20)) / 2;
        DrawText(opcoes[i], textoX, y + i * 70 + 15, 20, BLACK);
    }
}
