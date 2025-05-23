#include "fixo.h"

Rectangle areaJogo = { 130, 40, 1030, 600};

void CarregarTexturas(Texture2D *imagens, cJSON *array, int *quantidade) {
    int total = cJSON_GetArraySize(array);
    *quantidade = total;

    for (int i = 0; i < total; i++) {
        cJSON *item_imagem = cJSON_GetArrayItem(array, i);
        char *caminho = item_imagem->valuestring;       
        imagens[i] = LoadTexture(caminho);         
    }
}
