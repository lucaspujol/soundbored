#pragma once

#include "AudioEngine.hpp"

#include <cstdint>
#include <string>
#include <map>
#include <memory>

using SoundId = uint32_t;

class SoundBank {
    public:
        SoundBank(AudioEngine& engine);
        ~SoundBank() = default;
        
        bool loadSound(SoundId id, const std::string& path);
        bool unloadSound(SoundId id); // unload sound and path

        void play(SoundId id);
        void stop(SoundId id);
        void restart(SoundId id);
        
        bool hasSound(SoundId id) const;
        bool hasSound(std::string path) const;
        std::string getSoundPath(SoundId id) const;
        std::map<SoundId, std::string> getSoundMap() const;

    private:
        AudioEngine& engine;  // non-owning reference (engine owned by AudioManager)
        std::map<SoundId, std::unique_ptr<MaSound>> sounds;
        std::map<SoundId, std::string> paths;
};
