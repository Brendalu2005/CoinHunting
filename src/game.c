#include "fixo.h"
#include "ghost.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>


Jogador CriarJogador(const char *jsonPath, const char *nome, Vector2 pos0)
{
    Jogador j = {0};
    j.posicao      = pos0;
    j.velocidade   = 4;
    j.direcaoAtual = DOWN;
    j.tempoFrame   = 0.15f;
    j.tempoUltimaColisaoGhost2 = -10.0f;


    FILE *f = fopen(jsonPath, "rb");
    if (f == NULL){
        return j;
    } 

    
    fseek(f, 0, SEEK_END);
    long tamanho = ftell(f);
    rewind(f);

    char *buffer = malloc(tamanho + 1);
    if (buffer == NULL) {
        fclose(f);
        return j;
    }

    
    size_t lidos = fread(buffer, 1, tamanho, f);
    fclose(f);

    if (lidos != tamanho) {
        free(buffer);
        return j;
    }

    buffer[tamanho] = '\0';  

   
    cJSON *root = cJSON_Parse(buffer);
    free(buffer);
    if (root == NULL){
        return j;
    } 

    cJSON *obj = cJSON_GetObjectItem(root, nome);
    if (obj == NULL) {
        cJSON_Delete(root);
        return j;
    }

    int q1 = 0, q2 = 0, q3 = 0, q4 = 0;
    CarregarTexturas(j.cima,     cJSON_GetObjectItem(obj,"up"),    &q1);
    CarregarTexturas(j.baixo,    cJSON_GetObjectItem(obj,"down"),  &q2);
    CarregarTexturas(j.esquerda, cJSON_GetObjectItem(obj,"left"),  &q3);
    CarregarTexturas(j.direita,  cJSON_GetObjectItem(obj,"right"), &q4);

    cJSON_Delete(root);
    return j;
}

void AtualizarAnimacao(Jogador *j, int andando)
{
    if (!andando){
        j->indiceFrame = 0;
        return;
    }  

    int maxFrames;
    if (j->direcaoAtual == LEFT || j->direcaoAtual == RIGHT){
        maxFrames = MAX_FRAMES_LADO;

    }else{
        maxFrames = MAX_FRAMES_CIMA_BAIXO;

    }             

    j->temporizador += GetFrameTime();
    if (j->temporizador >= j->tempoFrame) {
        j->indiceFrame = (j->indiceFrame + 1) % maxFrames;
        j->temporizador = 0.0f;
    }
}

void AtualizarJogador(Jogador *j, int upKey, int downKey, int leftKey, int rightKey, Rectangle areaJogo)
{
    if (GetTime() - j->tempoUltimaColisaoGhost2 < 5.0f) {
        return;
    }

    int andando = 0;
    
    if (IsKeyDown(upKey) && j->posicao.y > areaJogo.y) {
        j->posicao.y -= j->velocidade;
        j->direcaoAtual = UP;
        andando = 1;

    } else if (IsKeyDown(downKey) && j->posicao.y + j->baixo[0].height < areaJogo.y + areaJogo.height) {
        j->posicao.y += j->velocidade;
        j->direcaoAtual = DOWN;
        andando = 1;

    } else if (IsKeyDown(leftKey) && j->posicao.x > areaJogo.x) {
        j->posicao.x -= j->velocidade;
        j->direcaoAtual = LEFT;
        andando = 1;

    } else if (IsKeyDown(rightKey) && j->posicao.x + j->direita[0].width < areaJogo.x + areaJogo.width) {
        j->posicao.x += j->velocidade;
        j->direcaoAtual = RIGHT;
        andando = 1;
    }

    AtualizarAnimacao(j, andando);
}


void DesenharJogador(Jogador *j){
    Texture2D txt;
    switch (j->direcaoAtual) {
        case UP:    
            txt = j->cima   [j->indiceFrame]; 
            break;
        case DOWN:  
            txt = j->baixo  [j->indiceFrame]; 
            break;
        case LEFT:  
            txt = j->esquerda[j->indiceFrame]; 
            break;
        case RIGHT: 
            txt = j->direita [j->indiceFrame]; 
            break;
    }

    Color cor = WHITE;

    
    if (GetTime() - j->tempoUltimaColisaoGhost2 < 0.5f) {
        cor = RED;
    }

    DrawTexture(txt, j->posicao.x, j->posicao.y, cor);
}

void DestruirJogador(Jogador *j)
{
    for (int i = 0; i < MAX_FRAMES_CIMA_BAIXO; i++) {
        UnloadTexture(j->cima[i]);
        UnloadTexture(j->baixo[i]);
        if (i < MAX_FRAMES_LADO) {
            UnloadTexture(j->esquerda[i]);
            UnloadTexture(j->direita[i]);
        }
    }
}
