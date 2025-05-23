#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <raylib.h>
#include "cJSON.h"


#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT  720

#define MAX_FRAMES_CIMA_BAIXO 6
#define MAX_FRAMES_LADO       4

#define LIMITE_X 100
#define LIMITE_Y 100
#define LIMITE_LARGURA 800
#define LIMITE_ALTURA 500


typedef enum { 
    MENU,
    SELECAO,
    SOBRE,
    RANKING, 
    JOGO 
} TelaAtual;

extern Rectangle areaJogo;

void CarregarTexturas(Texture2D *imagens, cJSON *array, int *quantidade);

#endif
