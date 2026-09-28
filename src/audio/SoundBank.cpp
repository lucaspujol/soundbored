#include "SoundBank.hpp"

SoundBank::SoundBank(AudioEngine& engine) : engine(engine) {}

bool SoundBank::loadSound(SoundId id, const std::string& path) {
    if (sounds.find(id) != sounds.end()) return false;

    sounds[id] = engine.createSound(path);
    paths[id] = path;
    return true;
}

bool SoundBank::unloadSound(SoundId id) {
    auto soundIt = sounds.find(id);
    if (soundIt == sounds.end()) return false;
    sounds.erase(soundIt);

    auto pathIt = paths.find(id);
    if (pathIt != paths.end()) {
        paths.erase(pathIt);
    }

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

bool SoundBank::hasSound(std::string path) const {
    for (const auto& pair : paths) {
        if (pair.second == path) {
            return true;
        }
    }
    return false;
}

std::string SoundBank::getSoundPath(SoundId id) const {
    auto it = paths.find(id);
    if (it != paths.end()) {
        return it->second;
    }
    return "";
}

std::map<SoundId, std::string> SoundBank::getSoundMap() const {
    return paths;
}