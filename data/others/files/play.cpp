/*! \file play.cpp
 *  \brief Enter description here.
 *  \author Georgi Gerganov
 */

#include "constants.h"
#include "common.h"

#include <SDL.h>
#include <SDL_audio.h>

#include <fstream>

bool g_terminate = false;

void cbPlayback(void * userdata, uint8_t * stream, int len) {
    std::ifstream * fin = (std::ifstream *)(userdata);
    int keyPressed = -1;
    fin->read((char *)(&keyPressed), sizeof(keyPressed));
    if (fin->eof()) {
        g_terminate = true;
        return;
    }
    printf("%c", keyPressed);
    fflush(stdout);
    fin->read((char *)(stream), len); // todo
    if (fin->eof()) g_terminate = true;
}

int main(int argc, char ** argv) {
    printf("Usage: %s input.kbd [-pN]\n", argv[0]);
    printf("    -pN - select playback device N\n");
    printf("\n");

    if (argc < 2) {
        return -127;
    }

    auto argm = parseCmdArguments(argc, argv);
    int captureId = argm["p"].empty() ? 0 : std::stoi(argm["p"]);

    std::ifstream fin(argv[1], std::ios::binary);
    if (fin.good() == false) {
        fprintf(stderr, "Failed to open file '%s'\n", argv[1]);
        return -1;
    }

    int bufferSize_frames = 1;
    fin.read((char *)(&bufferSize_frames), sizeof(bufferSize_frames));
    printf("Buffer size = %d frames\n", bufferSize_frames);

    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s\n", SDL_GetError());
        return -1;
    }

    int nDevices = SDL_GetNumAudioDevices(SDL_FALSE);
    printf("Found %d playback devices:\n", nDevices);
    for (int i = 0; i < nDevices; i++) {
        printf("    - Playback device #%d: '%s'\n", i, SDL_GetAudioDeviceName(i, SDL_FALSE));
    }

    if (captureId < 0 || captureId >= nDevices) {
        printf("Invalid playback device id selected - %d\n", captureId);
        return -1;
    }

    SDL_AudioSpec playbackSpec;
    SDL_zero(playbackSpec);

    playbackSpec.freq = kSampleRate;
    playbackSpec.format = AUDIO_F32SYS;
    playbackSpec.channels = 1;
    playbackSpec.samples = bufferSize_frames*kSamplesPerFrame;
    playbackSpec.callback = cbPlayback;
    playbackSpec.userdata = (void *)(&fin);

    SDL_AudioSpec obtainedSpec;
    SDL_zero(obtainedSpec);

    printf("Attempt to open playback device %d : '%s' ...\n", captureId, SDL_GetAudioDeviceName(captureId, SDL_FALSE));
    auto deviceIdOut = SDL_OpenAudioDevice(SDL_GetAudioDeviceName(captureId, SDL_FALSE), SDL_FALSE, &playbackSpec, &obtainedSpec, 0);
    if (!deviceIdOut) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't open an audio device for playback: %s!\n", SDL_GetError());
        SDL_Quit();
        return -2;
    }

    int sampleSize_bytes = 4; // todo

    printf("Opened playback device succesfully!\n");
    printf("    Frequency:  %d\n", obtainedSpec.freq);
    printf("    Format:     %d (%d bytes)\n", obtainedSpec.format, sampleSize_bytes);
    printf("    Channels:   %d\n", obtainedSpec.channels);
    printf("    Samples:    %d\n", obtainedSpec.samples);

    SDL_PauseAudioDevice(deviceIdOut, 0);

    while(g_terminate == false) {
        SDL_Delay(100);
    }

    fin.close();

    SDL_CloseAudio();

    return 0;
}
