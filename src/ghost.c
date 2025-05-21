#include "ghost.h"
#include "cJSON.h"
#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "fixo.h"

void CarregarTexturas(Texture2D *imagens, const cJSON *array, int *quantidade) {
    *quantidade = cJSON_GetArraySize(array);
    for (int i = 0; i < *quantidade; i++) {
        imagens[i] = LoadTexture(cJSON_GetArrayItem(array, i)->valuestring);
    }
}

Ghost CriarFantasma(const char *caminhoJSON, const char *chaveFantasma, Vector2 posicaoInicial) {
    Ghost g = {0};
    g.position = posicaoInicial;
    g.speed = 4;
    g.currentDirection = DOWN;
    g.frameTime = 0.2f;
    g.moveCooldown = 1.5f;

    FILE *f = fopen(caminhoJSON, "rb");
    if (!f) return g;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);
    char *data = malloc(size + 1);
    if (fread(data, 1, size, f) != size) {
        free(data);
        fclose(f);
        return g;
    }
    
    data[size] = '\0';
    fclose(f);

    cJSON *root = cJSON_Parse(data);
    free(data);
    if (!root) return g;

    cJSON *obj = cJSON_GetObjectItem(root, chaveFantasma);
    if (!obj) {
        cJSON_Delete(root);
        return g;
    }

    CarregarTexturas(g.up,    cJSON_GetObjectItem(obj, "up"),    &g.frameCount[UP]);
    CarregarTexturas(g.down,  cJSON_GetObjectItem(obj, "down"),  &g.frameCount[DOWN]);
    CarregarTexturas(g.left,  cJSON_GetObjectItem(obj, "left"),  &g.frameCount[LEFT]);
    CarregarTexturas(g.right, cJSON_GetObjectItem(obj, "right"), &g.frameCount[RIGHT]);

    cJSON_Delete(root);
    srand(time(NULL));
    return g;
}

void AtualizarFantasma(Ghost *g, Vector2 jogadorPos, float largura, float altura) {
    float dt = GetFrameTime();
    g->moveTimer += dt;
    g->timer += dt;

    if (g->moveTimer >= g->moveCooldown) {
        g->currentDirection = GetRandomValue(0, 3);
        g->moveTimer = 0.0f;
    }

    float w = g->right[0].width;
    float h = g->down[0].height;

    if (g->currentDirection == UP    && g->position.y > 0)               g->position.y -= g->speed;
    if (g->currentDirection == DOWN  && g->position.y + h < altura)      g->position.y += g->speed;
    if (g->currentDirection == LEFT  && g->position.x > 0)               g->position.x -= g->speed;
    if (g->currentDirection == RIGHT && g->position.x + w < largura)     g->position.x += g->speed;

    int total = g->frameCount[g->currentDirection];
    if (total > 1 && g->timer >= g->frameTime) {
        g->frameIndex = (g->frameIndex + 1) % total;
        g->timer = 0.0f;
    }
}

void DesenharFantasma(Ghost *g) {
    Texture2D *anim[] = { g->up, g->down, g->left, g->right };
    int idx = (g->frameCount[g->currentDirection] > 1) ? g->frameIndex : 0;
    DrawTexture(anim[g->currentDirection][idx], g->position.x, g->position.y, WHITE);
}

void DestruirFantasma(Ghost *g) {
    for (int d = 0; d < 4; d++) {
        Texture2D *sprites[] = { g->up, g->down, g->left, g->right };
        for (int i = 0; i < g->frameCount[d]; i++) UnloadTexture(sprites[d][i]);
    }
}

bool VerificarColisaoFantasma(Ghost *g, Jogador *j, Sound somSusto) {
    Rectangle retJogador = { j->posicao.x, j->posicao.y, 32, 32 };
    Rectangle retFantasma = { g->position.x, g->position.y, g->right[0].width, g->down[0].height };

    if (CheckCollisionRecs(retJogador, retFantasma)) {
        PlaySound(somSusto);
        j->posicao = (Vector2){ WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f };
        return true;
    }
    return false;
}
void InicializarListaFantasmas(ListaFantasmas *lista, const char *caminhoJSON, const char *chaveFantasma) {
    lista->quantidade = 1;
    lista->tempoDesdeUltimo = 0.0f;
    lista->fantasmas[0] = CriarFantasma(caminhoJSON, chaveFantasma, (Vector2){ 300, 300 });
}

void AtualizarListaFantasmas(ListaFantasmas *lista, Vector2 jogadorPos, int largura, int altura, float delta) {
    lista->tempoDesdeUltimo += delta;

    if (lista->tempoDesdeUltimo >= 30.0f && lista->quantidade < MAX_FANTASMAS) {
        lista->fantasmas[lista->quantidade] = CriarFantasma("sprites/json/movimentaçãoPlayer.json", "ghost_walk",
            (Vector2){ rand() % (largura - 64), rand() % (altura - 64) });
        lista->quantidade++;
        lista->tempoDesdeUltimo = 0.0f;
    }

    for (int i = 0; i < lista->quantidade; i++) {
        AtualizarFantasma(&lista->fantasmas[i], jogadorPos, largura, altura);
    }
}

void DesenharListaFantasmas(ListaFantasmas *lista) {
    for (int i = 0; i < lista->quantidade; i++) {
        DesenharFantasma(&lista->fantasmas[i]);
    }
}

void DestruirListaFantasmas(ListaFantasmas *lista) {
    for (int i = 0; i < lista->quantidade; i++) {
        DestruirFantasma(&lista->fantasmas[i]);
    }
}

Ghost2 CriarGhost2(const char *caminhoJSON, const char *id, Vector2 posicao) {
    Ghost2 g2;
    g2.ghost = CriarFantasma(caminhoJSON, id, posicao);
    g2.speed = g2.ghost.speed; 
    g2.tempoTextoMoeda = 0.0f;
    return g2;
}



void VerificarColisaoGhost2(Ghost2 *ghost, Jogador *j, Sound somColisao, TextoPerda textos[MAX_TEXTS]) {
    float tempoAtual = GetTime();

    Rectangle rectJogador = {
        j->posicao.x,
        j->posicao.y,
        j->direita[0].width,
        j->direita[0].height
    };

    Texture2D *directions[] = {
        ghost->ghost.up,
        ghost->ghost.down,
        ghost->ghost.left,
        ghost->ghost.right
    };

    Texture2D texturaAtual = directions[ghost->ghost.currentDirection][ghost->ghost.frameIndex];

    Rectangle rectGhost = {
        ghost->ghost.position.x,
        ghost->ghost.position.y,
        texturaAtual.width,
        texturaAtual.height
    };

    if (CheckCollisionRecs(rectJogador, rectGhost)) {
        if (tempoAtual - j->tempoUltimaColisaoGhost2 > 5.0f) {
            PlaySound(somColisao);
            j->tempoUltimaColisaoGhost2 = tempoAtual;

            if (j->moedasOuro > 0) {
                j->moedasOuro--;
            } else if (j->moedasPrata > 0) {
                j->moedasPrata--;
            }

            
            for (int i = 0; i < MAX_TEXTS; i++) {
                if (!textos[i].active) {
                    textos[i].active = true;
                    textos[i].position = (Vector2){ 
                        j->posicao.x + j->direita[0].width / 2 - MeasureText("-1 moeda", 20) / 2, 
                        j->posicao.y - 10 
                    };
                    textos[i].Congelado = tempoAtual + 5.0f;
                    strcpy(textos[i].texto, "-1 moeda");
                    break;
                }
            }

            j->tempoTextoMoeda = tempoAtual + 5.0f;
        }
    }
}



void DesenharTextosPerda(TextoPerda textos[MAX_TEXTS]) {
    for (int i = 0; i < MAX_TEXTS; i++) {
        if (textos[i].active) {
            DrawText(textos[i].texto,
                     textos[i].position.x,
                     textos[i].position.y - 20,
                     20,
                     RED);

            if (GetTime() > textos[i].Congelado) {
                textos[i].active = false;
            }
        }
    }
}




