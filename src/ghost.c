#include "fixo.h"
#include "ghost.h"
#include "cJSON.h"
#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>


Ghost CriarFantasma(const char *caminhoJSON, const char *chaveFantasma, Vector2 posicaoInicial) {
    Ghost g = {0};   
    g.posicao = posicaoInicial;
    g.velocidade = 4;
    g.currentDirection = DOWN;
    g.frameTime = 0.2f;
    g.moveLimite = 1.5f;

   
    FILE *f = fopen(caminhoJSON, "rb");
    if (f == NULL){
        return g;
    } 

    
    fseek(f, 0, SEEK_END);
    long tamanho = ftell(f);
    rewind(f);

    char *buffer = malloc(tamanho + 1);
    if (buffer == NULL) {
        fclose(f);
        return g;
    }

    
    size_t lidos = fread(buffer, 1, tamanho, f);
    fclose(f);

    if (lidos != tamanho) {
        free(buffer);
        return g;
    }

    buffer[tamanho] = '\0';  

   
    cJSON *root = cJSON_Parse(buffer);
    free(buffer);
    if (root == NULL){
        return g;
    } 

    cJSON *obj = cJSON_GetObjectItem(root, chaveFantasma);
    if (obj == NULL) {
        cJSON_Delete(root);
        return g;
    }

    
    CarregarTexturas(g.up,    cJSON_GetObjectItem(obj, "up"),    &g.frameCount[UP]);
    CarregarTexturas(g.down,  cJSON_GetObjectItem(obj, "down"),  &g.frameCount[DOWN]);
    CarregarTexturas(g.left,  cJSON_GetObjectItem(obj, "left"),  &g.frameCount[LEFT]);
    CarregarTexturas(g.right, cJSON_GetObjectItem(obj, "right"), &g.frameCount[RIGHT]);

    cJSON_Delete(root);

    return g;
}


void AtualizarFantasma(Ghost *g, Vector2 jogadorPos, Rectangle areaJogo) {
    float dt = GetFrameTime();
    g->moveTimer += dt;
    g->timer += dt;

   
    if (g->moveTimer >= g->moveLimite) {
        g->currentDirection = GetRandomValue(0, 3);
        g->moveTimer = 0;
    }

    
    float largura = g->right[0].width;
    float altura = g->down[0].height;

    
    switch (g->currentDirection) {
        case UP:
            if (g->posicao.y > areaJogo.y)
                g->posicao.y -= g->velocidade;
            break;
        case DOWN:
            if (g->posicao.y + altura < areaJogo.y + areaJogo.height)
                g->posicao.y += g->velocidade;
            break;
        case LEFT:
            if (g->posicao.x > areaJogo.x)
                g->posicao.x -= g->velocidade;
            break;
        case RIGHT:
            if (g->posicao.x + largura < areaJogo.x + areaJogo.width)
                g->posicao.x += g->velocidade;
            break;
    }

    
    int totalFrames = g->frameCount[g->currentDirection];
    if (totalFrames > 1 && g->timer >= g->frameTime) {
        g->frameIndex = (g->frameIndex + 1) % totalFrames;
        g->timer = 0;
    }
}


void DesenharFantasma(Ghost *g) {
    Texture2D *animation[] = { g->up, g->down, g->left, g->right };

    int indice = 0;
    if (g->frameCount[g->currentDirection] > 1) {
        indice = g->frameIndex;
    }

    DrawTexture(animation[g->currentDirection][indice], g->posicao.x, g->posicao.y, WHITE);
}


void DestruirFantasma(Ghost *g) {
    for (int d = 0; d < 4; d++) {
        Texture2D *sprites[] = { g->up, g->down, g->left, g->right };
        for (int i = 0; i < g->frameCount[d]; i++){
            UnloadTexture(sprites[d][i]);
        } 
    }
}

bool VerificarColisaoFantasma(Ghost *g, Jogador *j, Sound somSusto) {
    Rectangle retJogador = { j->posicao.x, j->posicao.y, 32, 32 };
    Rectangle retFantasma = { g->posicao.x, g->posicao.y, g->right[0].width, g->down[0].height };

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

void AtualizarListaFantasmas(ListaFantasmas *lista, Vector2 jogadorPos, Rectangle areaJogo, float delta) {
    lista->tempoDesdeUltimo += delta;

    if (lista->tempoDesdeUltimo >= 30.0f && lista->quantidade < MAX_FANTASMAS) {
        lista->fantasmas[lista->quantidade] = CriarFantasma("sprites/json/movimentaçãoPlayer.json", "ghost_walk",
            (Vector2){ rand() % (int)(areaJogo.width - 64) + (int)areaJogo.x, 
                       rand() % (int)(areaJogo.height - 64) + (int)areaJogo.y });

        lista->quantidade++;
        lista->tempoDesdeUltimo = 0.0f;
    }

    for (int i = 0; i < lista->quantidade; i++) {
        AtualizarFantasma(&lista->fantasmas[i], jogadorPos, areaJogo);
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

Ghost2 CriarGhost2(const char *caminhoJSON, const char *chaveFantasma, Vector2 posicao) {
    Ghost2 g2;
    g2.eixoAtual = MOVENDO_X;
    g2.tempoEixo = 0.2f;  

    g2.ghost = CriarFantasma(caminhoJSON, chaveFantasma, posicao);
    g2.velocidade = g2.ghost.velocidade;
    g2.tempoTextoMoeda = 0.0f;
    return g2;
}

Vector2 Normaliza(Vector2 v) {
    float comprimento = sqrtf(v.x * v.x + v.y * v.y);
    if (comprimento == 0) return (Vector2){ 0, 0 };
    return (Vector2){ v.x / comprimento, v.y / comprimento };
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
        ghost->ghost.posicao.x,
        ghost->ghost.posicao.y,
        texturaAtual.width,
        texturaAtual.height
    };

    if (CheckCollisionRecs(rectJogador, rectGhost)) {
        if (tempoAtual - j->tempoUltimaColisaoGhost2 >= 10.0f) {
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
                    textos[i].posicao = (Vector2){ 
                        j->posicao.x + j->direita[0].width / 2 - MeasureText("-1 moeda", 20) / 2, 
                        j->posicao.y - 10 
                    };
                    textos[i].Congelado = tempoAtual + 5.0f;
                    strcpy(textos[i].texto, "-1 moeda");
                    break;
                }
            }

            j->tempoTextoMoeda = tempoAtual + 5.0f;

            Vector2 des = {
                ghost->ghost.posicao.x - j->posicao.x,
                ghost->ghost.posicao.y - j->posicao.y
            };
            
            Vector2 direcaoOposta = Normaliza(des);
            
            ghost->ghost.posicao.x += direcaoOposta.x * 1200.0f;
            ghost->ghost.posicao.y += direcaoOposta.y * 1200.0f;
            
            if (ghost->ghost.posicao.x < areaJogo.x)
                ghost->ghost.posicao.x = areaJogo.x;
            if (ghost->ghost.posicao.x + ghost->ghost.right[0].width > areaJogo.x + areaJogo.width)
                ghost->ghost.posicao.x = areaJogo.x + areaJogo.width - ghost->ghost.right[0].width;
            
            if (ghost->ghost.posicao.y < areaJogo.y)
                ghost->ghost.posicao.y = areaJogo.y;
            if (ghost->ghost.posicao.y + ghost->ghost.down[0].height > areaJogo.y + areaJogo.height)
                ghost->ghost.posicao.y = areaJogo.y + areaJogo.height - ghost->ghost.down[0].height;
        }
    }
}



void DesenharTextosPerda(TextoPerda textos[MAX_TEXTS]) {
    for (int i = 0; i < MAX_TEXTS; i++) {
        if (textos[i].active) {
            DrawText(textos[i].texto,
                     textos[i].posicao.x,
                     textos[i].posicao.y - 20, 20, RED);

            if (GetTime() > textos[i].Congelado) {
                textos[i].active = false;
            }
        }
    }
}




void AtualizarGhost2(Ghost2 *ghost, Vector2 jogadorPos, Rectangle areaJogo) {
    float dt = GetFrameTime();
    ghost->ghost.timer += dt;
    ghost->tempoEixo -= dt;

    float dx = jogadorPos.x - ghost->ghost.posicao.x;
    float dy = jogadorPos.y - ghost->ghost.posicao.y;

    float velocidade = ghost->velocidade * 0.4f;

    if (ghost->tempoEixo <= 0) {
        if (ghost->eixoAtual == MOVENDO_X && fabs(dy) > fabs(dx)) {
            ghost->eixoAtual = MOVENDO_Y;
            ghost->tempoEixo = 0.2f;  
        } else if (ghost->eixoAtual == MOVENDO_Y && fabs(dx) > fabs(dy)) {
            ghost->eixoAtual = MOVENDO_X;
            ghost->tempoEixo = 0.2f;
        }
    }

    if (ghost->eixoAtual == MOVENDO_X) {
        if (dx > 0 && ghost->ghost.posicao.x + ghost->ghost.right[0].width < areaJogo.x + areaJogo.width) {
            ghost->ghost.posicao.x += velocidade;
            ghost->ghost.currentDirection = RIGHT;
        } else if (dx < 0 && ghost->ghost.posicao.x > areaJogo.x) {
            ghost->ghost.posicao.x -= velocidade;
            ghost->ghost.currentDirection = LEFT;
        }
    } else {
        if (dy > 0 && ghost->ghost.posicao.y + ghost->ghost.down[0].height < areaJogo.y + areaJogo.height) {
            ghost->ghost.posicao.y += velocidade;
            ghost->ghost.currentDirection = DOWN;
        } else if (dy < 0 && ghost->ghost.posicao.y > areaJogo.y) {
            ghost->ghost.posicao.y -= velocidade;
            ghost->ghost.currentDirection = UP;
        }
    }
    int totalFrames = ghost->ghost.frameCount[ghost->ghost.currentDirection];
    if (totalFrames > 1 && ghost->ghost.timer >= ghost->ghost.frameTime) {
        ghost->ghost.frameIndex = (ghost->ghost.frameIndex + 1) % totalFrames;
        ghost->ghost.timer = 0;
    }
}

