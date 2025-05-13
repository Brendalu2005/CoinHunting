#ifndef GHOST_H
#define GHOST_H

#include "raylib.h"
#include "cJSON.h"

#define MAX_GHOST_FRAMES 10  

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

typedef struct {
    Vector2 position;
    float speed;
    Direction currentDirection;
    float frameTime;
    float timer;
    int frameIndex;
    float moveCooldown;
    float moveTimer;

    Texture2D up[MAX_GHOST_FRAMES];
    Texture2D down[MAX_GHOST_FRAMES];
    Texture2D left[MAX_GHOST_FRAMES];
    Texture2D right[MAX_GHOST_FRAMES];

    int frameCount[4];  
} Ghost;

Ghost LoadGhostFromJSON(const char *jsonPath, const char *ghostKey, Vector2 startPos);
void UpdateGhost(Ghost *g, float screenWidth, float screenHeight);
void DrawGhost(Ghost *g);
void UnloadGhost(Ghost *g);

#endif
