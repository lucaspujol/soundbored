#include "SoundBank.hpp"
#include <vector>

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

std::vector<SoundId> SoundBank::getAllSoundIds() const {
    std::vector<SoundId> ids;
    for (const auto& pair : sounds) {
        ids.push_back(pair.first);
    }
    return ids;
}

std::vector<std::string> SoundBank::getAllSoundPaths() const {
    std::vector<std::string> pathsList;
    for (const auto& pair : paths) {
        pathsList.push_back(pair.second);
    }
    return pathsList;
}
