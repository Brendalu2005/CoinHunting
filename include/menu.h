#ifndef MENU_H
#define MENU_H

#include "raylib.h"
#include "game.h"


void AtualizarMenu(Rectangle botao, TelaAtual *telaAtual, int *opcao);
void DesenharMenu(Rectangle botao, Texture2D background, int opcao);


#endif
