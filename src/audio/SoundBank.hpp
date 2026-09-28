#pragma once

#include "AudioEngine.hpp"

#include <map>
#include <string>
#include <vector>
#include <memory>
#include <cstdint>

// actual ids will be > 0, -1 is used to indicate failure for sound related stuff
using SoundId = int32_t;

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
        std::vector<SoundId> getAllSoundIds() const;

    private:
        AudioEngine& engine;  // non-owning reference (engine owned by AudioManager)
        std::map<SoundId, std::unique_ptr<MaSound>> sounds;
};
