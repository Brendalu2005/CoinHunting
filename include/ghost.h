#ifndef GHOST_H
#define GHOST_H

#include "raylib.h"
#include "cJSON.h"
#include "game.h"

#define MAX_GHOST_FRAMES 10  
#define MAX_FANTASMAS 50

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

typedef struct {
    Ghost ghost;
    float tempoTextoMoeda;
    float speed;
} Ghost2;


typedef struct {
    Ghost fantasmas[MAX_FANTASMAS];
    int quantidade;
    float tempoDesdeUltimo;
} ListaFantasmas;

void InicializarListaFantasmas(ListaFantasmas *lista, const char *caminhoJSON, const char *chaveFantasma);
void AtualizarListaFantasmas(ListaFantasmas *lista, Vector2 jogadorPos, int largura, int altura, float delta);
void DesenharListaFantasmas(ListaFantasmas *lista);
void DestruirListaFantasmas(ListaFantasmas *lista);
void CarregarTexturas(Texture2D *imagens, const cJSON *array, int *quantidade);
Ghost CriarFantasma(const char *caminhoJSON, const char *chaveFantasma, Vector2 posicaoInicial);
Ghost2 CriarGhost2(const char *jsonPath, const char *spriteID, Vector2 pos);
void AtualizarFantasma(Ghost *g, Vector2 jogadorPos, float largura, float altura);
void DesenharFantasma(Ghost *g);
void DestruirFantasma(Ghost *g);
bool VerificarColisaoFantasma(Ghost *g, Jogador *j, Sound somSusto);
void VerificarColisaoGhost2(Ghost2 *ghost, Jogador *j, Sound somSusto, TextoPerda textos[MAX_TEXTS]);
void DesenharTextosPerda(TextoPerda textos[MAX_TEXTS]);
void CarregarSomFantasma(void);
void DescarregarSomFantasma(void);


#endif
