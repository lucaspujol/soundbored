#define MINIAUDIO_IMPLEMENTATION
#include "AudioEngine.hpp"

AudioEngine::AudioEngine(ma_device_id* pDeviceID) : engine(pDeviceID) {}

std::unique_ptr<MaSound> AudioEngine::createSound(const std::string& path) {
    return std::make_unique<MaSound>(engine, path);
}

void AudioEngine::play(MaSound& sound) {
    ma_sound_start(sound.get());
}

void AudioEngine::stop(MaSound& sound) {
    ma_sound_stop(sound.get());
}

void AudioEngine::restart(MaSound& sound) {
    ma_sound* s = sound.get();
    ma_sound_seek_to_pcm_frame(s, 0);
    ma_sound_start(s);
}
