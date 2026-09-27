#define MINIAUDIO_IMPLEMENTATION
#include "AudioEngine.hpp"

AudioEngine::AudioEngine(ma_device_id* pDeviceID) {
    engine = new MaEngine(pDeviceID);
}

AudioEngine::~AudioEngine() {
    delete engine;
}

MaSound AudioEngine::createSound(const std::string& path) const{
    return MaSound(engine, path);
}

void AudioEngine::play(MaSound* sound) const {
    ma_sound_start(sound->get());
}

void AudioEngine::stop(MaSound* sound) const {
    ma_sound_stop(sound->get());
}

void AudioEngine::restart(MaSound* sound) const {
    ma_sound* s = sound->get();
    ma_sound_seek_to_pcm_frame(s, 0);
    ma_sound_start(s);
}
