#pragma once

#include "AudioEngine.hpp"

#include <map>
#include <string>
#include <vector>
#include <memory>
#include <cstdint>

using SoundId = uint32_t;
static const SoundId MIN_SOUND_ID = 1;
static const SoundId ERROR_SOUND_ID = 0;

class SoundBank {
    public:
        SoundBank(AudioEngine& engine);
        ~SoundBank() = default;
        
        bool loadSound(SoundId id, const std::string& path);
        bool unloadSound(SoundId id);

        void play(SoundId id);
        void stop(SoundId id);
        void restart(SoundId id);
        
        bool hasSound(SoundId id) const;
        std::vector<SoundId> getAllSoundIds() const;

        uint64_t getSoundLengthMs(SoundId id) const;
        uint64_t getSoundRemainingLengthMs(SoundId id) const;

    private:
        AudioEngine& engine;  // non-owning reference (engine owned by AudioManager)
        std::map<SoundId, std::unique_ptr<MaSound>> sounds;
};
