#include "fixo.h"
#include "game.h"
#include "ghost.h"
#include "menu.h"
#include "moeda.h"
#include "selecao.h"
#include "ranking.h"
#include <stdio.h>
#include <math.h>
#include "audio.h"



int main(void) {

    InitAudioDevice();
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "CoinHunting");
    SetTargetFPS(60);
    CarregarSons();

    TelaAtual tela = MENU;

    Rectangle botaoIniciar     = { WINDOW_WIDTH / 2 - 100, 200, 200, 50 };
    Rectangle botao1Jogador    = { WINDOW_WIDTH / 2 - 110, 200, 220, 50 };
    Rectangle botao2Jogadores  = { WINDOW_WIDTH / 2 - 110, 270, 220, 50 };

    const char *JSON = "sprites/json/movimentaçãoPlayer.json";
    Texture2D bgMenu = LoadTexture("sprites/png/BackgroundMenu.png");
    Texture2D bgJogo = LoadTexture("sprites/png/backgroundJogo2.png");

    Jogador p1 = CriarJogador(JSON, "edu_walk",    (Vector2){560, 360});
    Jogador p2 = CriarJogador(JSON, "brenda_walk", (Vector2){680, 360});
    Jogador p3 = CriarJogador(JSON, "guto_walk",   (Vector2){640, 360});
    ListaFantasmas fantasmas;
    Ghost2 ghost2 = CriarGhost2(JSON, "ghost2_walk", (Vector2){ 500, 200 });
    InicializarListaFantasmas(&fantasmas, JSON, "ghost_walk");
    ghost2.eixoAtual = MOVENDO_X;

    Moeda moedas[MAX_MOEDAS];
    float tempoRespawn = 0.0f;
    InicializarMoedas(moedas);
    CarregarTexturasMoedas();
    TextoPerda textos[MAX_TEXTS];

    struct Ranking *ranking = CarregarRanking();

    char nomeJogador[TAM_NOME];
    bool solicitandoNome = false;
    int indexLetra = 0;

    int opcaoMenu = 0;
    int opcaojogadores = -1;

    int tempoRestante = 180; 
    float tempoAcumulado = 0.0f;

    bool jogoFinalizado = false;
    float tempoFimDeJogo = 0.0f;
    const float duracaoFimDeJogo = 3.0f;

    while (!WindowShouldClose()) {
        if (tela == JOGO && tempoRestante > 0 && !jogoFinalizado) {
            tempoAcumulado += GetFrameTime();
            if (tempoAcumulado >= 1.0f) {
                tempoRestante--;
                tempoAcumulado = 0.0f;
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        UpdateMusicStream(musicaMenu);
        UpdateMusicStream(musicaPartida);
        
        if (tela == JOGO) {
            if (!IsMusicStreamPlaying(musicaPartida)) {
                StopMusicStream(musicaMenu);
                PlayMusicStream(musicaPartida);
            }
            UpdateMusicStream(musicaPartida);
        } else {
            if (!IsMusicStreamPlaying(musicaMenu)) {
                StopMusicStream(musicaPartida);
                PlayMusicStream(musicaMenu);
            }
            UpdateMusicStream(musicaMenu);
        }
        
        switch (tela) {
            
            case MENU:
            AtualizarMenu(botaoIniciar, &tela, &opcaoMenu);
            DesenharMenu(botaoIniciar, bgMenu, opcaoMenu);
            break;
            
            case SELECAO:
            AtualizarSelecaoJogadores(botao1Jogador, botao2Jogadores, &tela, &opcaojogadores);
            DesenharSelecaoJogadores(botao1Jogador, botao2Jogadores, bgMenu, opcaojogadores);
            break;
            
            case SOBRE:
            AtualizarSobre(&tela);
            DesenharSobre(bgMenu);
            break;
            
            case RANKING:
                DesenharRanking(&ranking);
                if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER)) {
                    
                    tela = MENU;
                    opcaojogadores = -1;
                    jogoFinalizado = false;
                    solicitandoNome = false;
                    
                    InicializarMoedas(moedas);
                    DestruirListaFantasmas(&fantasmas);
                    InicializarListaFantasmas(&fantasmas, JSON, "ghost_walk");
                    p1.moedasPrata = p1.moedasOuro = 0;
                    p2.moedasPrata = p2.moedasOuro = 0;
                    p3.moedasPrata = p3.moedasOuro = 0;
                    
                    
                    tempoRestante = 180;
                    tempoAcumulado = 0.0f;
                }
                break;
                
                case JOGO:
                DrawRectangleLinesEx(areaJogo, 3, DARKGRAY);
                float tempoTotalJogo = GetTime();
                if (opcaojogadores == 1) {
                    if (GetTime() < p3.tempoTextoMoeda) {
                        DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, (Color){255, 0, 0, 100});
                    }
                } else if (opcaojogadores == 2) {
                    if (GetTime() < p1.tempoTextoMoeda) {
                        DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, (Color){255, 0, 0, 100});
                    }
                    if (GetTime() < p2.tempoTextoMoeda) {
                        DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, (Color){255, 0, 0, 100});
                    }
            }

            AtualizarMoedas(moedas, &tempoRespawn, tempoTotalJogo, areaJogo);
                DrawTexturePro(
                    bgJogo,
                    (Rectangle){ 0, 0, (float)bgJogo.width, (float)bgJogo.height },
                    (Rectangle){ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
                    (Vector2){ 0, 0 },
                    0.0f,
                    WHITE
                );
                DesenharMoedas(moedas); 
                DesenharListaFantasmas(&fantasmas);
                DesenharFantasma(&ghost2.ghost);
                DesenharTextosPerda(textos);


                if (!jogoFinalizado) {
                    if (opcaojogadores == 1) {
                        AtualizarJogador(&p3, KEY_W, KEY_S, KEY_A, KEY_D,  areaJogo);
                        AtualizarListaFantasmas(&fantasmas, p3.posicao, areaJogo, GetFrameTime());
                        AtualizarGhost2(&ghost2, p3.posicao, areaJogo);
                        colisaoMoedas(moedas, &p3, somMoeda);  
                        for (int i = 0; i < fantasmas.quantidade; i++) {
                            if (VerificarColisaoFantasma(&fantasmas.fantasmas[i], &p3, somColisao)) {
                            }
                        }

                        VerificarColisaoGhost2(&ghost2, &p3, somColisao, textos);
                        
                    } else if (opcaojogadores == 2) {
                        AtualizarJogador(&p1, KEY_W, KEY_S, KEY_A, KEY_D, areaJogo);
                        AtualizarJogador(&p2, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, areaJogo);
                        AtualizarListaFantasmas(&fantasmas, p3.posicao, areaJogo, GetFrameTime());
                        AtualizarFantasma(&ghost2.ghost, p1.posicao, areaJogo);
                        colisaoMoedas(moedas, &p1, somMoeda);  
                        colisaoMoedas(moedas, &p2, somMoeda); 
                        for (int i = 0; i < fantasmas.quantidade; i++) {
                            if (VerificarColisaoFantasma(&fantasmas.fantasmas[i], &p1, somColisao)) {
                            }
                        }
                        
                        VerificarColisaoGhost2(&ghost2, &p1, somColisao, textos);
                        
                        for (int i = 0; i < fantasmas.quantidade; i++) {
                            if (VerificarColisaoFantasma(&fantasmas.fantasmas[i], &p2, somColisao)) {
                            }
                        }

                        VerificarColisaoGhost2(&ghost2, &p2, somColisao, textos);
                    }

                    if (opcaojogadores == 1) {
                        DesenharJogador(&p3); 
                        int pontuacao = p3.moedasPrata + p3.moedasOuro * 2;
                        DrawText(TextFormat("P1 - Pontuação: %d", pontuacao), 10, 10, 20, GOLD);
                    } else {
                        DesenharJogador(&p1);
                        DesenharJogador(&p2);
                        int p1Pts = p1.moedasPrata + p1.moedasOuro * 2;
                        int p2Pts = p2.moedasPrata + p2.moedasOuro * 2;
                        DrawText(TextFormat("P1 - Pontuação: %d", p1Pts), 10, 10, 20, GOLD);
                        int l = MeasureText(TextFormat("P2 - Pontuação: %d", p2Pts), 20);
                        DrawText(TextFormat("P2 - Pontuação: %d", p2Pts), GetScreenWidth() - l - 10, 10, 20, SKYBLUE);
                    }

                    int minutos = tempoRestante / 60;
                    int segundos = tempoRestante % 60;
                    char textoTimer[20];
                    sprintf(textoTimer, "%02d:%02d", minutos, segundos);
                    int larguraTimer = MeasureText(textoTimer, 30);

                    Color corTimer = WHITE;
                    if (tempoRestante <= 30) {
                        float alpha = 0.5f + 0.5f * sinf(GetTime() * 6.0f);
                        corTimer = (Color){255, 0, 0, (unsigned char)(alpha * 255)};
                    } else if (tempoRestante <= 60){
                        corTimer = ORANGE;
                    } else if (tempoRestante <= 100){
                        corTimer = YELLOW;
                    } 

                    DrawText(textoTimer, GetScreenWidth()/2 - larguraTimer/2, 10, 30, corTimer);

                    if (tempoRestante <= 0) {
                        jogoFinalizado = true;
                        tempoFimDeJogo = 0.0f;
                    }

                } else {
                    int caixaLargura = 400;
                    int caixaAltura = 250;
                    int caixaX = GetScreenWidth()/2 - caixaLargura/2;
                    int caixaY = GetScreenHeight()/2 - caixaAltura/2;

                    DrawRectangle(caixaX, caixaY, caixaLargura, caixaAltura, Fade(BLACK, 0.7f));
                    DrawRectangleLines(caixaX, caixaY, caixaLargura, caixaAltura, WHITE);

                    
                    const char* msg = "FIM DE JOGO";
                    int largura = MeasureText(msg, 50);
                    DrawText(msg, GetScreenWidth() / 2 - largura / 2, caixaY + 15, 50, RED);

                    
                    if (opcaojogadores == 2) {
                        int pontos1 = p1.moedasPrata + p1.moedasOuro * 2;
                        int pontos2 = p2.moedasPrata + p2.moedasOuro * 2;
                    
                        const char* vencedor;
                    
                        if (pontos1 > pontos2) {
                            vencedor = "P1 Venceu!";
                        } else if (pontos2 > pontos1) {
                            vencedor = "P2 Venceu!";
                        } else {
                            vencedor = "Empate!";
                        }
                    
                        int larguraV = MeasureText(vencedor, 30);
                        DrawText(vencedor, GetScreenWidth() / 2 - larguraV / 2, caixaY + 75, 30, GREEN);
                    }
                    
                    
                    DrawText("Digite seu nome (até 5 letras):", caixaX + 30, caixaY + 125, 20, WHITE);

                    
                    DrawRectangle(caixaX + 30, caixaY + 155, caixaLargura - 60, 40, DARKGRAY);
                    DrawRectangleLines(caixaX + 30, caixaY + 155, caixaLargura - 60, 40, RAYWHITE);

                   
                    DrawText(nomeJogador, caixaX + 40, caixaY + 160, 30, YELLOW);

                    tempoFimDeJogo += GetFrameTime();

                    if (tempoFimDeJogo >= duracaoFimDeJogo) {
                        if (!solicitandoNome) {
                            solicitandoNome = true;
                            indexLetra = 0;
                            for (int i = 0; i < 6; i++) {
                                nomeJogador[i] = '\0';
                            }
                        }
                    }

                    int key = GetCharPressed();
                    if (key >= 32 && key <= 125 && indexLetra < 5) {
                        nomeJogador[indexLetra++] = (char)key;
                    }

                    if (IsKeyPressed(KEY_BACKSPACE) && indexLetra > 0) {
                        nomeJogador[--indexLetra] = '\0';
                    }

                    if (IsKeyPressed(KEY_ENTER) && indexLetra > 0) {
                        int pontos = 0;
                        if (opcaojogadores == 1) {
                            pontos = p3.moedasPrata + p3.moedasOuro * 2;
                        } else {
                            int pontos1 = p1.moedasPrata + p1.moedasOuro * 2;
                            int pontos2 = p2.moedasPrata + p2.moedasOuro * 2;
                        
                            if (pontos1 > pontos2) {
                                pontos = pontos1;
                            } else {
                                pontos = pontos2;
                            }
                        }
                        

                        AdicionarAoRanking(&ranking, nomeJogador, pontos);
                        SalvarRanking(&ranking);
                        solicitandoNome = false;

                        tela = RANKING;
                        opcaojogadores = -1;
                    }
                }
                break;
        }

        EndDrawing();
    }

    DestruirJogador(&p1);
    DestruirJogador(&p2);
    DestruirJogador(&p3);
    DestruirFantasma(&ghost2.ghost);
    DestruirListaFantasmas(&fantasmas);
    UnloadTexturasMoedas(); 
    UnloadTexture(bgMenu);
    UnloadTexture(bgJogo);
    SalvarRanking(&ranking);
    LiberarRanking(&ranking);
    DescarregarSons();
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
