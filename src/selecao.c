#include "selecao.h"
#include "raylib.h"

#define TOTAL_OPCOES_SELECAO 2

void AtualizarSelecaoJogadores(Rectangle botao1, Rectangle botao2, TelaAtual *tela, int *opcao)
{
    if (IsKeyPressed(KEY_DOWN)) {
        *opcao = (*opcao + 1) % TOTAL_OPCOES_SELECAO;
    }
    if (IsKeyPressed(KEY_UP)) {
        *opcao = (*opcao - 1 + TOTAL_OPCOES_SELECAO) % TOTAL_OPCOES_SELECAO;
    }

   if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        *tela = JOGO;
       
        if (*opcao == 0){
            *opcao = 1; 
        }  
        else if (*opcao == 1){
            *opcao = 2;
        }else {
            *tela = MENU;  
        }   
    }

}


void DesenharSelecaoJogadores(Rectangle botao1, Rectangle botao2, Texture2D background, int opcao){
    DrawTexturePro(
        background,
        (Rectangle){ 0, 0, background.width, background.height },
        (Rectangle){ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
        (Vector2){ 0, 0 },
        0.0f,
        WHITE
    );

    const char *titulo = "Selecione o número de jogadores:";
    int fonteTitulo = 30;
    int larguraTitulo = MeasureText(titulo, fonteTitulo);
    DrawText(titulo, (GetScreenWidth() - larguraTitulo) / 2, 100, fonteTitulo, WHITE);

    int largura = 220;
    int altura = 50;
    int x = GetScreenWidth() / 2 - largura / 2;
    int y1 = 200;
    int y2 = 270;

    Color corSelecionada = YELLOW;
    Color corNormal = LIGHTGRAY;

    Color cor;
    if (opcao == 0) {
        cor = corSelecionada;
    } else {
        cor = corNormal;
    }
    DrawRectangle(x, y1, largura, altura, cor);

    DrawText("1 JOGADOR", x + 40, y1 + 15, 20, BLACK);

    if (opcao == 1) {
        cor = corSelecionada;
    } else {
        cor = corNormal;
    }
    DrawRectangle(x, y2, largura, altura, cor);

    DrawText("2 JOGADORES", x + 30, y2 + 15, 20, BLACK);
}
void AtualizarSobre(TelaAtual *tela) {
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) {
        *tela = MENU;
    }
}

void DesenharSobre(Texture2D background) {
    DrawTexturePro(
        background,
        (Rectangle){ 0, 0, background.width, background.height },
        (Rectangle){ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
        (Vector2){ 0, 0 },
        0.0f,
        WHITE
    );

    const char *titulo = "Sobre o Jogo";
    const char *descricao = "Ajude os exploradores a coletarem o máximo de moedas possivel\n sem ser atingido pelos fantasmas protetores da caverna.\n";
     const char *descricao2 = "Fantasma vermelho: Tira moedas e congela o jogador por um tempo, apesar de ser lerdo, ele lhe seguirá pelo mapa.\n\n"
                              "Fantasma azul: Joga o jogador pro centro do mapa, são rápidos, mas andam aleatoriamente pelo mapa.\n";
    const char *descricao3 =
        "\nControles:\n"
        "Jogador 1: W A S D \n"
        "Jogador 2: Setas";

    int fonteTitulo = 30;
    int fonteDescricao = 20;
    int larguraTitulo = MeasureText(titulo, fonteTitulo);
    int larguraDescricao = MeasureText(descricao, fonteDescricao);
    int larguraDescricao2 = MeasureText(descricao2, fonteDescricao);
    int larguraDescricao3 = MeasureText(descricao3, fonteDescricao);

    DrawText(titulo, (GetScreenWidth() - larguraTitulo) / 2, 100, fonteTitulo, WHITE);
    DrawText(descricao, (GetScreenWidth() - larguraDescricao) / 2, 180, fonteDescricao, WHITE);
    DrawText(descricao2, (GetScreenWidth() - larguraDescricao2) / 2, 260, fonteDescricao, WHITE);
    DrawText(descricao3, (GetScreenWidth() - larguraDescricao3) / 22, 320, fonteDescricao, WHITE);
}

