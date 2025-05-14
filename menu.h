// menu.h
#ifndef MENU_H
#define MENU_H

#include "raylib.h"

typedef enum {
    MENU,
    JOGO
} TelaAtual;

void AtualizarMenu(Rectangle botao, TelaAtual *telaAtual);
void DesenharMenu(Rectangle botao, Texture2D background);

#endif
