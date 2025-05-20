#include "audio.h"

Sound somMoeda;
Sound somColisao;
Music musicaMenu;
Music musicaPartida;

void CarregarSons(void) {
    musicaMenu     = LoadMusicStream("audio/musicaTelasIniciais.mp3");
    musicaPartida  = LoadMusicStream("audio/musicaPartida2.mp3");

    somMoeda       = LoadSound("audio/coinCatch.wav");
    somColisao     = LoadSound("audio/ghostColisao.wav");

    SetSoundVolume(somMoeda,      1.0f);
    SetSoundVolume(somColisao,    0.6f);
    SetMusicVolume(musicaMenu,    0.4f);
    SetMusicVolume(musicaPartida, 0.15f);
}

void DescarregarSons(void) {
    UnloadSound(somMoeda);
    UnloadSound(somColisao);
    UnloadMusicStream(musicaMenu);
    UnloadMusicStream(musicaPartida);
}
