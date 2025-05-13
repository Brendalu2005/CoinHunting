#include "raylib.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include "ghost.h"

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direcao;

#define MAX_FRAMES_CIMA_BAIXO 6
#define MAX_FRAMES_LADO 4

typedef struct {
    Vector2 posicao;
    float velocidade;
    Direcao direcaoAtual;
    float tempoFrame;
    float temporizador;
    int indiceFrame;

    Texture2D cima[MAX_FRAMES_CIMA_BAIXO];
    Texture2D baixo[MAX_FRAMES_CIMA_BAIXO];
    Texture2D esquerda[MAX_FRAMES_LADO];
    Texture2D direita[MAX_FRAMES_LADO];
} Jogador;

void CarregarTexturas(Texture2D *imagens, const cJSON *array, int *quantidade) {
    *quantidade = cJSON_GetArraySize(array);
    for (int i = 0; i < *quantidade; i++) {
        const char *caminho = cJSON_GetArrayItem(array, i)->valuestring;
        imagens[i] = LoadTexture(caminho);
        if (imagens[i].id == 0) {
            TraceLog(LOG_ERROR, "Falha ao carregar textura: %s", caminho);
        } else {
            TraceLog(LOG_INFO, "Textura carregada com sucesso: %s", caminho);
        }
    }
}


Jogador CriarJogador(const char *jsonPath, const char *nome, Vector2 posicaoInicial) {
    Jogador j = {0};
    j.posicao = posicaoInicial;
    j.velocidade = 4;
    j.direcaoAtual = DOWN;
    j.indiceFrame = 0;
    j.tempoFrame = 0.15f;
    j.temporizador = 0.0f;

    FILE *arquivo = fopen(jsonPath, "rb");
    if (!arquivo) return j;

    fseek(arquivo, 0, SEEK_END);
    long tamanho = ftell(arquivo);
    rewind(arquivo);

    char *dados = malloc(tamanho + 1);
    fread(dados, 1, tamanho, arquivo);
    dados[tamanho] = '\0';
    fclose(arquivo);

    cJSON *raiz = cJSON_Parse(dados);
    free(dados);
    if (!raiz) return j;

    cJSON *obj = cJSON_GetObjectItem(raiz, nome);
    if (!obj) {
        cJSON_Delete(raiz);
        return j;
    }

    CarregarTexturas(j.cima, cJSON_GetObjectItem(obj, "up"), MAX_FRAMES_CIMA_BAIXO);
    CarregarTexturas(j.baixo, cJSON_GetObjectItem(obj, "down"), MAX_FRAMES_CIMA_BAIXO);
    CarregarTexturas(j.esquerda, cJSON_GetObjectItem(obj, "left"), MAX_FRAMES_LADO);
    CarregarTexturas(j.direita, cJSON_GetObjectItem(obj, "right"), MAX_FRAMES_LADO);

    cJSON_Delete(raiz);
    return j;
}

void AtualizarAnimacao(Jogador *j) {
    int maxFrames = (j->direcaoAtual == LEFT || j->direcaoAtual == RIGHT)
                    ? MAX_FRAMES_LADO : MAX_FRAMES_CIMA_BAIXO;

    j->temporizador += GetFrameTime();
    if (j->temporizador >= j->tempoFrame) {
        j->indiceFrame = (j->indiceFrame + 1) % maxFrames;
        j->temporizador = 0.0f;
    }
}

void DesenharJogador(Jogador *j) {
    Texture2D atual;
    switch (j->direcaoAtual) {
        case UP: atual = j->cima[j->indiceFrame]; break;
        case DOWN: atual = j->baixo[j->indiceFrame]; break;
        case LEFT: atual = j->esquerda[j->indiceFrame]; break;
        case RIGHT: atual = j->direita[j->indiceFrame]; break;
    }
    DrawTexture(atual, j->posicao.x, j->posicao.y, WHITE);
}

int main(void) {
    const int largura = 1280;
    const int altura = 720;
    InitWindow(largura, altura, "Jogo: Caça às Moedas");

    const char *CAMINHO_JSON = "sprites/json/movimentaçãoPlayer.json";

    Jogador p1 = CriarJogador(CAMINHO_JSON, "edu_walk", (Vector2){100, 100});
    Jogador p2 = CriarJogador(CAMINHO_JSON, "brenda_walk", (Vector2){600, 400});
    Ghost fantasma = CriarFantasma(CAMINHO_JSON, "ghost_walk", (Vector2){300, 300});

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        bool p1Andando = false;
        bool p2Andando = false;

        AtualizarFantasma(&fantasma, p1.posicao, largura, altura);  // Atualiza o fantasma para perseguir o jogador

        if (IsKeyDown(KEY_W) && p1.posicao.y > 0) {
            p1.posicao.y -= p1.velocidade;
            p1.direcaoAtual = UP;
            p1Andando = true;
        } else if (IsKeyDown(KEY_S) && p1.posicao.y + p1.baixo[0].height < altura) {
            p1.posicao.y += p1.velocidade;
            p1.direcaoAtual = DOWN;
            p1Andando = true;
        } else if (IsKeyDown(KEY_A) && p1.posicao.x > 0) {
            p1.posicao.x -= p1.velocidade;
            p1.direcaoAtual = LEFT;
            p1Andando = true;
        } else if (IsKeyDown(KEY_D) && p1.posicao.x + p1.direita[0].width < largura) {
            p1.posicao.x += p1.velocidade;
            p1.direcaoAtual = RIGHT;
            p1Andando = true;
        }

        if (IsKeyDown(KEY_UP) && p2.posicao.y > 0) {
            p2.posicao.y -= p2.velocidade;
            p2.direcaoAtual = UP;
            p2Andando = true;
        } else if (IsKeyDown(KEY_DOWN) && p2.posicao.y + p2.baixo[0].height < altura) {
            p2.posicao.y += p2.velocidade;
            p2.direcaoAtual = DOWN;
            p2Andando = true;
        } else if (IsKeyDown(KEY_LEFT) && p2.posicao.x > 0) {
            p2.posicao.x -= p2.velocidade;
            p2.direcaoAtual = LEFT;
            p2Andando = true;
        } else if (IsKeyDown(KEY_RIGHT) && p2.posicao.x + p2.direita[0].width < largura) {
            p2.posicao.x += p2.velocidade;
            p2.direcaoAtual = RIGHT;
            p2Andando = true;
        }

        if (p1Andando) AtualizarAnimacao(&p1); else p1.indiceFrame = 0;
        if (p2Andando) AtualizarAnimacao(&p2); else p2.indiceFrame = 0;

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DesenharJogador(&p1);
        DesenharJogador(&p2);
        DesenharFantasma(&fantasma);
        EndDrawing();
    }

   
    for (int i = 0; i < MAX_FRAMES_CIMA_BAIXO; i++) {
        UnloadTexture(p1.cima[i]);
        UnloadTexture(p1.baixo[i]);
        UnloadTexture(p2.cima[i]);
        UnloadTexture(p2.baixo[i]);
        if (i < MAX_FRAMES_LADO) {
            UnloadTexture(p1.esquerda[i]);
            UnloadTexture(p1.direita[i]);
            UnloadTexture(p2.esquerda[i]);
            UnloadTexture(p2.direita[i]);
        }
    }

    UnloadGhost(&fantasma);
    CloseWindow();
    return 0;
}
