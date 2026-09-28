#include "SoundBank.hpp"

SoundBank::SoundBank(AudioEngine& engine) : engine(engine) {}

bool SoundBank::loadSound(SoundId id, const std::string& path) {
    if (sounds.find(id) != sounds.end()) return false;

    sounds[id] = engine.createSound(path);
    return true;
}

bool SoundBank::unloadSound(SoundId id) {
    auto soundIt = sounds.find(id);
    if (soundIt == sounds.end()) return false;
    sounds.erase(soundIt);

    return true;
}

void SoundBank::play(SoundId id) {
    auto it = sounds.find(id);
    if (it != sounds.end()) {
        engine.play(*(it->second));
    }
}

void SoundBank::stop(SoundId id) {
    auto it = sounds.find(id);
    if (it != sounds.end()) {
        engine.stop(*(it->second));
    }
}

void SoundBank::restart(SoundId id) {
    auto it = sounds.find(id);
    if (it != sounds.end()) {
        engine.restart(*(it->second));
    }
}

bool SoundBank::hasSound(SoundId id) const {
    return sounds.find(id) != sounds.end();
}

std::vector<SoundId> SoundBank::getAllSoundIds() const {
    std::vector<SoundId> ids;
    for (const auto& pair : sounds) {
        ids.push_back(pair.first);
    }
    return ids;
}
