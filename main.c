
#include "raylib.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

#define MAX_FRAMES_UD 6
#define MAX_FRAMES_LR 4

typedef struct {
    Vector2 position;
    float speed;
    Direction currentDirection;
    float frameTime;
    float timer;
    int frameIndex;
    Texture2D up[MAX_FRAMES_UD];
    Texture2D down[MAX_FRAMES_UD];
    Texture2D left[MAX_FRAMES_LR];
    Texture2D right[MAX_FRAMES_LR];
} PlayerAnimado;

void LoadAnimationTextures(Texture2D *textures, const cJSON *array, int max) {
    for (int i = 0; i < max; i++) {
        const char *path = cJSON_GetArrayItem(array, i)->valuestring;
        textures[i] = LoadTexture(path);
    }
}

PlayerAnimado LoadPlayerFromJSON(const char *jsonPath, const char *playerKey, Vector2 startPos) {
    PlayerAnimado player = {0};
    player.position = startPos;
    player.speed = 4;
    player.currentDirection = DOWN;
    player.frameIndex = 0;
    player.frameTime = 0.15f;
    player.timer = 0.0f;

    FILE *file = fopen(jsonPath, "rb");
    if (!file) {
        TraceLog(LOG_ERROR, "Erro ao abrir JSON: %s", jsonPath);
        return player;
    }

    fseek(file, 0, SEEK_END);
    long len = ftell(file);
    rewind(file);
    char *data = malloc(len + 1);
    fread(data, 1, len, file);
    data[len] = '\0';
    fclose(file);

    cJSON *root = cJSON_Parse(data);
    free(data);
    if (!root) {
        TraceLog(LOG_ERROR, "Erro ao fazer parse do JSON");
        return player;
    }

    cJSON *playerObj = cJSON_GetObjectItem(root, playerKey);
    if (!playerObj) {
        TraceLog(LOG_ERROR, "Chave do jogador nao encontrada: %s", playerKey);
        cJSON_Delete(root);
        return player;
    }

    LoadAnimationTextures(player.up, cJSON_GetObjectItem(playerObj, "up"), MAX_FRAMES_UD);
    LoadAnimationTextures(player.down, cJSON_GetObjectItem(playerObj, "down"), MAX_FRAMES_UD);
    LoadAnimationTextures(player.left, cJSON_GetObjectItem(playerObj, "left"), MAX_FRAMES_LR);
    LoadAnimationTextures(player.right, cJSON_GetObjectItem(playerObj, "right"), MAX_FRAMES_LR);

    cJSON_Delete(root);
    return player;
}

void UpdateAnimation(PlayerAnimado *p) {
    int maxFrames = (p->currentDirection == LEFT || p->currentDirection == RIGHT) ? MAX_FRAMES_LR : MAX_FRAMES_UD;
    p->timer += GetFrameTime();
    if (p->timer >= p->frameTime) {
        p->frameIndex = (p->frameIndex + 1) % maxFrames;
        p->timer = 0.0f;
    }
}

void DrawPlayer(PlayerAnimado *p) {
    Texture2D current;
    switch (p->currentDirection) {
        case UP: current = p->up[p->frameIndex]; break;
        case DOWN: current = p->down[p->frameIndex]; break;
        case LEFT: current = p->left[p->frameIndex]; break;
        case RIGHT: current = p->right[p->frameIndex]; break;
    }
    DrawTexture(current, p->position.x, p->position.y, WHITE);
}

int main(void) {
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Coin Hunting");

    #define JSON_PATH "sprites/json/movimentaçãoPlayer.json"

    PlayerAnimado p1 = LoadPlayerFromJSON(JSON_PATH, "edu_walk", (Vector2){100, 100});
    PlayerAnimado p2 = LoadPlayerFromJSON(JSON_PATH, "brenda_walk", (Vector2){600, 400});

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        bool p1Moving = false;
        bool p2Moving = false;

        // Controles Player 1 (WASD)
        if (IsKeyDown(KEY_W) && p1.position.y > 0) {
            p1.position.y -= p1.speed;
            p1.currentDirection = UP;
            p1Moving = true;
        } else if (IsKeyDown(KEY_S) && p1.position.y + p1.down[0].height < screenHeight) {
            p1.position.y += p1.speed;
            p1.currentDirection = DOWN;
            p1Moving = true;
        } else if (IsKeyDown(KEY_A) && p1.position.x > 0) {
            p1.position.x -= p1.speed;
            p1.currentDirection = LEFT;
            p1Moving = true;
        } else if (IsKeyDown(KEY_D) && p1.position.x + p1.right[0].width < screenWidth) {
            p1.position.x += p1.speed;
            p1.currentDirection = RIGHT;
            p1Moving = true;
        }

        // Controles Player 2 (Setas)
        if (IsKeyDown(KEY_UP) && p2.position.y > 0) {
            p2.position.y -= p2.speed;
            p2.currentDirection = UP;
            p2Moving = true;
        } else if (IsKeyDown(KEY_DOWN) && p2.position.y + p2.down[0].height < screenHeight) {
            p2.position.y += p2.speed;
            p2.currentDirection = DOWN;
            p2Moving = true;
        } else if (IsKeyDown(KEY_LEFT) && p2.position.x > 0) {
            p2.position.x -= p2.speed;
            p2.currentDirection = LEFT;
            p2Moving = true;
        } else if (IsKeyDown(KEY_RIGHT) && p2.position.x + p2.right[0].width < screenWidth) {
            p2.position.x += p2.speed;
            p2.currentDirection = RIGHT;
            p2Moving = true;
        }

        if (p1Moving) UpdateAnimation(&p1);
        else p1.frameIndex = 0;

        if (p2Moving) UpdateAnimation(&p2);
        else p2.frameIndex = 0;

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawPlayer(&p1);
        DrawPlayer(&p2);
        EndDrawing();
    }

    // Libera texturas
    for (int i = 0; i < MAX_FRAMES_UD; i++) {
        UnloadTexture(p1.up[i]);
        UnloadTexture(p1.down[i]);
        UnloadTexture(p2.up[i]);
        UnloadTexture(p2.down[i]);
        if (i < MAX_FRAMES_LR) {
            UnloadTexture(p1.left[i]);
            UnloadTexture(p1.right[i]);
            UnloadTexture(p2.left[i]);
            UnloadTexture(p2.right[i]);
        }
    }

    CloseWindow();
    return 0;
}
