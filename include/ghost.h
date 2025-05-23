#ifndef GHOST_H
#define GHOST_H

#include "raylib.h"
#include "cJSON.h"
#include "game.h"

#define MAX_GHOST_FRAMES 10  
#define MAX_FANTASMAS 50

typedef struct {
    Vector2 posicao; 
    float velocidade;   
    Direction currentDirection; 
    float frameTime; 
    float timer;     
    int frameIndex;  
    float moveLimite;  
    float moveTimer;  
    Texture2D up[MAX_GHOST_FRAMES]; 
    Texture2D down[MAX_GHOST_FRAMES];  
    Texture2D left[MAX_GHOST_FRAMES];  
    Texture2D right[MAX_GHOST_FRAMES]; 
    int frameCount[4]; 
} Ghost;

typedef enum { MOVENDO_X, MOVENDO_Y } EixoMovimento;

typedef struct {
    Ghost ghost;
    float tempoTextoMoeda;
    float velocidade;
    float tempoEixo;
    EixoMovimento eixoAtual;
} Ghost2;


typedef struct {
    Ghost fantasmas[MAX_FANTASMAS];
    int quantidade;
    float tempoDesdeUltimo;
} ListaFantasmas;

void InicializarListaFantasmas(ListaFantasmas *lista, const char *caminhoJSON, const char *chaveFantasma);
void AtualizarListaFantasmas(ListaFantasmas *lista, Vector2 jogadorPos, Rectangle areaJogo, float delta);
void DesenharListaFantasmas(ListaFantasmas *lista);
void DestruirListaFantasmas(ListaFantasmas *lista);
Ghost CriarFantasma(const char *caminhoJSON, const char *chaveFantasma, Vector2 posicaoInicial);
Ghost2 CriarGhost2(const char *caminhoJSON, const char *chaveFantasma, Vector2 posicao);
void AtualizarFantasma(Ghost *g, Vector2 jogadorPos, Rectangle areaJogo);
void DesenharFantasma(Ghost *g);
void DestruirFantasma(Ghost *g);
bool VerificarColisaoFantasma(Ghost *g, Jogador *j, Sound somSusto);
void VerificarColisaoGhost2(Ghost2 *ghost, Jogador *j, Sound somSusto, TextoPerda textos[MAX_TEXTS]);
void DesenharTextosPerda(TextoPerda textos[MAX_TEXTS]);
void CarregarSomFantasma(void);
void DescarregarSomFantasma(void);

void AtualizarGhost2(Ghost2 *Ghost, Vector2 jogadorPos, Rectangle areaJogo);
void VerificarColisaoGhost2(Ghost2 *ghost, Jogador *j, Sound somColisao, TextoPerda textos[MAX_TEXTS]);

#endif
