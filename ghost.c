#include "ghost.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

void CarregarTexturas(Texture2D *imagens, const cJSON *array, int *quantidade) {
    *quantidade = cJSON_GetArraySize(array);
    for (int i = 0; i < *quantidade; i++) {
        const char *caminho = cJSON_GetArrayItem(array, i)->valuestring;
        imagens[i] = LoadTexture(caminho);
    }
}

Ghost CriarFantasma(const char *caminhoJSON, const char *chaveFantasma, Vector2 posicaoInicial) {
    Ghost fantasma = {0};
    fantasma.position = posicaoInicial;
    fantasma.speed = 2;
    fantasma.currentDirection = DOWN;
    fantasma.frameIndex = 0;
    fantasma.frameTime = 0.2f;
    fantasma.timer = 0.0f;
    fantasma.moveCooldown = 1.5f;
    fantasma.moveTimer = 0.0f;

    FILE *arquivo = fopen(caminhoJSON, "rb");
    if (!arquivo) {
        TraceLog(LOG_ERROR, "Erro ao abrir JSON: %s", caminhoJSON);
        return fantasma;
    }

    fseek(arquivo, 0, SEEK_END);
    long tamanho = ftell(arquivo);
    rewind(arquivo);
    char *dados = malloc(tamanho + 1);
    fread(dados, 1, tamanho, arquivo);
    dados[tamanho] = '\0';
    fclose(arquivo);

    cJSON *raiz = cJSON_Parse(dados);
    free(dados);
    if (!raiz) {
        TraceLog(LOG_ERROR, "Erro ao ler JSON");
        return fantasma;
    }

    cJSON *objFantasma = cJSON_GetObjectItem(raiz, chaveFantasma);
    if (!objFantasma) {
        TraceLog(LOG_ERROR, "Fantasma '%s' não encontrado no JSON", chaveFantasma);
        cJSON_Delete(raiz);
        return fantasma;
    }

    CarregarTexturas(fantasma.up, cJSON_GetObjectItem(objFantasma, "up"), &fantasma.frameCount[UP]);
    CarregarTexturas(fantasma.down, cJSON_GetObjectItem(objFantasma, "down"), &fantasma.frameCount[DOWN]);
    CarregarTexturas(fantasma.left, cJSON_GetObjectItem(objFantasma, "left"), &fantasma.frameCount[LEFT]);
    CarregarTexturas(fantasma.right, cJSON_GetObjectItem(objFantasma, "right"), &fantasma.frameCount[RIGHT]);

    cJSON_Delete(raiz);
    srand(time(NULL));
    return fantasma;
}

void AtualizarFantasma(Ghost *g, Vector2 jogadorPos, float largura, float altura) {
    g->moveTimer += GetFrameTime();
    g->timer += GetFrameTime();

    if (g->moveTimer >= g->moveCooldown) {
        // Persegue o jogador
        if (jogadorPos.x < g->position.x) g->currentDirection = LEFT;
        else if (jogadorPos.x > g->position.x) g->currentDirection = RIGHT;
        else if (jogadorPos.y < g->position.y) g->currentDirection = UP;
        else if (jogadorPos.y > g->position.y) g->currentDirection = DOWN;

        g->moveTimer = 0.0f;
    }

    // Move o fantasma
    switch (g->currentDirection) {
        case UP:
            if (g->position.y > 0) g->position.y -= g->speed;
            break;
        case DOWN:
            if (g->position.y + g->down[0].height < altura) g->position.y += g->speed;
            break;
        case LEFT:
            if (g->position.x > 0) g->position.x -= g->speed;
            break;
        case RIGHT:
            if (g->position.x + g->right[0].width < largura) g->position.x += g->speed;
            break;
    }

    int totalFrames = g->frameCount[g->currentDirection];
    if (totalFrames > 1 && g->timer >= g->frameTime) {
        g->frameIndex = (g->frameIndex + 1) % totalFrames;
        g->timer = 0.0f;
    }
}

void DesenharFantasma(Ghost *g) {
    Texture2D atual;
    int indice = (g->frameCount[g->currentDirection] > 1) ? g->frameIndex : 0;

    switch (g->currentDirection) {
        case UP: atual = g->up[indice]; break;
        case DOWN: atual = g->down[indice]; break;
        case LEFT: atual = g->left[indice]; break;
        case RIGHT: atual = g->right[indice]; break;
    }

    DrawTexture(atual, g->position.x, g->position.y, WHITE);
}

void DestruirFantasma(Ghost *g) {
    for (int i = 0; i < g->frameCount[UP]; i++) UnloadTexture(g->up[i]);
    for (int i = 0; i < g->frameCount[DOWN]; i++) UnloadTexture(g->down[i]);
    for (int i = 0; i < g->frameCount[LEFT]; i++) UnloadTexture(g->left[i]);
    for (int i = 0; i < g->frameCount[RIGHT]; i++) UnloadTexture(g->right[i]);
}
