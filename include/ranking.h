#ifndef RANKING_H
#define RANKING_H

#define TAM_NOME 6 

typedef struct Ranking {
    char nome[TAM_NOME];
    int pontuacao;
    struct Ranking *next;
} Ranking;

Ranking *CarregarRanking();
void SalvarRanking(Ranking **head);
void AdicionarAoRanking(Ranking **head, const char *nome, int pontuacao);
void LiberarRanking(Ranking **head);
void DesenharRanking(Ranking **head);

#endif
