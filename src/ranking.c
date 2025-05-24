#include "ranking.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"

Ranking *CarregarRanking() {
    FILE *arquivo = fopen("arquivo/ranking.txt", "r");
    if (!arquivo){
        return NULL;
    } 

    Ranking *inicio = NULL;
    Ranking *ultimo = NULL;

    char nome[TAM_NOME];
    int pontos;

    while (fscanf(arquivo, "%5s %d", nome, &pontos) == 2) {
        struct Ranking *novo_no = (struct Ranking*)malloc(sizeof(struct Ranking));
        strcpy(novo_no->nome, nome);
        novo_no->pontuacao = pontos;
        novo_no->next = NULL;

        if (!inicio){
            inicio = novo_no;
            
        }else{
            ultimo->next = novo_no;

        } 

        ultimo = novo_no;
    }

    fclose(arquivo);
    return inicio;
}

void SalvarRanking(Ranking **head) {
    FILE *arquivo = fopen("arquivo/ranking.txt", "w");
    if (arquivo == NULL){
        return;
    } 
    Ranking *temp = *head;
    while (temp != NULL) {
        fprintf(arquivo, "%s %d\n", temp->nome, temp->pontuacao);
        temp = temp->next;
    }

    fclose(arquivo);
}

void AdicionarAoRanking(Ranking **head, const char *nome, int pontuacao) {
    struct Ranking *novo_no =(struct Ranking*)malloc(sizeof(struct Ranking));
    strncpy(novo_no->nome, nome, TAM_NOME - 1);
    novo_no->nome[TAM_NOME-1] = '\0';
    novo_no->pontuacao = pontuacao;
    novo_no->next = NULL;

    
    if (*head == NULL || pontuacao > (*head)->pontuacao) {
        novo_no->next = *head;
        *head = novo_no;
    } else {
        Ranking *temp = *head;
        while (temp->next && temp->next->pontuacao >= pontuacao) {
            temp = temp->next;
        }
        novo_no->next = temp->next;
        temp->next = novo_no;
    }
}

void LiberarRanking(Ranking **head) {
    while (*head != NULL) {
        struct Ranking *temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}

void DesenharRanking(Ranking **head) {
    static int scrollOffset = 0;
    int totalExibidos = 5;

    ClearBackground(DARKGRAY);
    
    DrawText("TOP JOGADORES", 
            GetScreenWidth()/2 - MeasureText("TOP JOGADORES", 30)/2, 
            20, 
            30, 
            GOLD);

    DrawText("Melhores Pontuações", 
            GetScreenWidth()/2 - MeasureText("Melhores Pontuações", 20)/2, 
            60, 
            20, 
            LIGHTGRAY);

    
    int totalJogadores = 0;
    Ranking *conta = *head;
    while (conta != NULL) {
        totalJogadores++;
        conta = conta->next;
    }

    
    if (IsKeyPressed(KEY_DOWN) && scrollOffset + totalExibidos < totalJogadores) {
        scrollOffset++;
    }
    if (IsKeyPressed(KEY_UP) && scrollOffset > 0) {
        scrollOffset--;
    }

   
    Ranking *temp = *head;
    int i = 0;
    while (i < scrollOffset && temp != NULL) {
        temp = temp->next;
        i++;
    }

    int y = 100; 
    int pos = scrollOffset + 1; 
    int exibidos = 0;

    while (temp && exibidos < totalExibidos) {
        char texto[64];
        snprintf(texto, sizeof(texto), "%d. %s - %d", pos, temp->nome, temp->pontuacao);

        Color corTexto = WHITE;
        Color fundo = Fade(LIGHTGRAY, 0.2f); 

        if (pos == 1) {
            fundo = GOLD;
            corTexto = BLACK;
        } else if (pos % 2 == 0) {
            corTexto = SKYBLUE;
        }

        
        int boxWidth = 400;
        int boxHeight = 35;
        int x = GetScreenWidth()/2 - boxWidth/2;

        DrawRectangleRounded((Rectangle){x, y, boxWidth, boxHeight}, 0.3f, 10, fundo);
        DrawText(texto, x + 10, y + 5, 25, corTexto);

        y += 45;
        pos++;
        exibidos++;
        temp = temp->next;
    }

    
}

