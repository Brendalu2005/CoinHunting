#ifndef GHOST_H
#define GHOST_H

#include "raylib.h"
#include "cJSON.h"

#define MAX_GHOST_FRAMES 10  


typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;


typedef struct {
    Vector2 position; 
    float speed;   
    Direction currentDirection; 
    float frameTime; 
    float timer;     
    int frameIndex;  
    float moveCooldown;  
    float moveTimer;  

    Texture2D up[MAX_GHOST_FRAMES]; 
    Texture2D down[MAX_GHOST_FRAMES];  
    Texture2D left[MAX_GHOST_FRAMES];  
    Texture2D right[MAX_GHOST_FRAMES]; 

    int frameCount[4]; 
} Ghost;


void CarregarTexturas(Texture2D *imagens, const cJSON *array, int *quantidade);


Ghost CriarFantasma(const char *caminhoJSON, const char *chaveFantasma, Vector2 posicaoInicial);


void AtualizarFantasma(Ghost *g, Vector2 jogadorPos, float largura, float altura);
void DesenharFantasma(Ghost *g);
void DestruirFantasma(Ghost *g);

#endif
