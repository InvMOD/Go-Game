#include "board.hpp"
#include "../lib/includes.hpp"

Board::~Board() {
    closeAudio();
}

void Board::initAudio() {
    if (!IsAudioDeviceReady()) {
        InitAudioDevice();
    }

    if (audioLoaded) return;

    const char* soundFiles[] = {
        "assets/sounds/stone1.wav",
        "assets/sounds/stone2.wav",
        "assets/sounds/stone3.wav"
    };

    stoneSounds.clear();
    for (const char* file : soundFiles) {
        const char* path = file;
        char altPath[256];
        if (!FileExists(path)) {
            snprintf(altPath, sizeof(altPath), "../%s", file);
            if (FileExists(altPath)) {
                path = altPath;
            }
        }

        if (FileExists(path)) {
            Sound s = LoadSound(path);
            if (s.frameCount > 0) {
                stoneSounds.push_back(s);
            }
        }
    }

    audioLoaded = !stoneSounds.empty();
}

void Board::playStoneSound() {
    if (!audioLoaded || stoneSounds.empty()) return;

    int idx = GetRandomValue(0, static_cast<int>(stoneSounds.size()) - 1);
    Sound sound = stoneSounds[idx];

    // Pitch variation: 96% to 104% (0.96f - 1.04f) to prevent machine-gun effect
    float pitch = static_cast<float>(GetRandomValue(96, 104)) / 100.0f;
    SetSoundPitch(sound, pitch);
    SetSoundVolume(sound, 0.90f);
    PlaySound(sound);
}

void Board::closeAudio() {
    for (auto& s : stoneSounds) {
        UnloadSound(s);
    }
    stoneSounds.clear();
    audioLoaded = false;

    if (IsAudioDeviceReady()) {
        CloseAudioDevice();
    }
}
