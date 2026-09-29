#pragma once

#include "AudioEngine.hpp" // this include miniaudio.h and its cpp file declares MINIAUDIO_IMPLEMENTATION
#include "SoundBank.hpp"

#include <string>
#include <vector>
#include <memory>
#include <map>

/**
 * @brief A RAII wrapper class for miniaudio context (ma_context)
 */
class MaContext {
    public:
        MaContext() {
            ma_context_config contextConfig = ma_context_config_init();
            if (ma_context_init(NULL, 0, &contextConfig, &context) != MA_SUCCESS) {
                throw std::runtime_error("Failed to initialize ma_context.");
            }
        }

        ~MaContext() { ma_context_uninit(&context); }
        MaContext(const MaContext&) = delete;
        MaContext& operator=(const MaContext&) = delete;

        ma_context* get() { return &context; }

    private:
        ma_context context;
};

struct DeviceInfo {
    std::string name;
    ma_device_id id;
    bool isDefault;
};

class AudioManager {
    public:
        static AudioManager& getInstance() {
            static AudioManager instance;
            return instance;
        }

        ~AudioManager() = default;

        AudioManager(const AudioManager&) = delete;
        AudioManager& operator=(const AudioManager&) = delete;

        std::vector<DeviceInfo> getAvailableDevices();

        bool setPhysicalDevice(std::string dName);
        bool setVirtualDevice(std::string dName);
        void clearVirtualDevice();

        // returns sound id, or 0 on any failure
        SoundId importSound(std::string path);
        void removeSound(SoundId id);

        // continues where the sound left off, on the physical device and the virtual device if set
        void playSound(SoundId id);
        // stops the sound on the physical device and the virtual device if set, play will resume from there
        void stopSound(SoundId id);
        // plays a sound from the beginning on the physical device and the virtual device if set
        void restartSound(SoundId id);

        std::map<SoundId, std::string> getSoundMap() const { return soundPaths; }

        // get total length of sound in milliseconds, or 0 if sound not found
        uint64_t getSoundLengthMs(SoundId id) const;
        // get remaining length of sound in milliseconds, or 0 if sound not found
        uint64_t getSoundRemainingLengthMs(SoundId id) const;

    private:
        AudioManager();

        MaContext context;

        std::unique_ptr<AudioEngine> physicalEngine;
        std::unique_ptr<SoundBank> physicalSoundBank;

        std::unique_ptr<AudioEngine> virtualEngine;
        std::unique_ptr<SoundBank> virtualSoundBank;

        SoundId nextSoundId = MIN_SOUND_ID;
        std::map<SoundId, std::string> soundPaths;

        std::vector<DeviceInfo> cachedDevices;

        void refreshDeviceCache();
        ma_device_id getDefaultDeviceId();
        bool resolveDeviceName(std::string dName, ma_device_id* outId);
};
