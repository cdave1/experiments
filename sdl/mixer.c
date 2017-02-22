#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"

static const char *MY_COOL_MP3 = "AlsoSprachZarathustra.mp3";
static unsigned callbacks = 0;

void effect_callback(int chan, void *stream, int len, void *udata) {
    callbacks++;
    printf("callbacks: %d\n", callbacks);
}


void post_mix_callback(void *udata, Uint8 *stream, int len) {
    const unsigned int STEP = 4;
    for (int i = 0; i < len; i += 2) {
        for (int j = 0; j < ((sizeof(Uint8) * 256) / STEP); ++j) {
            printf(" ");
        }
        printf("\r");
        for (int j = 0; j < stream[i]; j += STEP) {
            printf("-");
        }
        printf("\r");
    }
}


int main(int argc, char ** argv) {
    int result = 0;
    int flags = MIX_INIT_MP3;

    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("Failed to init SDL\n");
        exit(1);
    }

    if (flags != (result = Mix_Init(flags))) {
        printf("Could not initialize mixer (result: %d).\n", result);
        printf("Mix_Init: %s\n", Mix_GetError());
        exit(1);
    }

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 256);
    Mix_Music *music = Mix_LoadMUS(MY_COOL_MP3);
    Mix_PlayMusic(music, 1);

    if (Mix_RegisterEffect(1, &effect_callback, NULL, NULL) == 0) {
        printf("Effect not registered.\n");
        printf("Mix_Init: %s\n", Mix_GetError());
    } else {
        printf("Effect registered.\n");
    }

    Mix_SetPostMix(&my_awesome_callback, NULL);

    while (!SDL_QuitRequested()) {
        SDL_Delay(1);
    }

    Mix_FreeMusic(music);
    SDL_Quit();
    return 0;
}
