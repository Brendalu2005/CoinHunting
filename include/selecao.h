#ifndef SELECAO_H
#define SELECAO_H

#include "raylib.h"
#include "game.h" 

void AtualizarSelecaoJogadores(Rectangle botao1, Rectangle botao2, TelaAtual *tela, int *opcao);
void DesenharSelecaoJogadores(Rectangle botao1, Rectangle botao2, Texture2D background, int opcao);

#endif
