#pragma once

#include <miniaudio.h>

#include <string>
#include <stdexcept>

/**
 * A RAII wrapper class for miniaudio engine (ma_engine)
 */
class MaEngine {
    public:
        MaEngine(ma_device_id* deviceID) {
            ma_engine_config engineConfig = ma_engine_config_init();
            engineConfig.pPlaybackDeviceID = deviceID;
            if (ma_engine_init(&engineConfig, &engine) != MA_SUCCESS) {
                throw std::runtime_error("Failed to initialize ma_engine.");
                // TODO: Maybe don't throw, need to consider this later
            }
        }

        ~MaEngine() { ma_engine_uninit(&engine); }
        ma_engine* get() { return &engine; }

        MaEngine(const MaEngine&) = delete;
        MaEngine& operator=(const MaEngine&) = delete;


    private:
        ma_engine engine;
};

/**
 * A RAII wrapper class for miniaudio sound (ma_sound)
 */
class MaSound {
    public:
        MaSound(MaEngine& engine, const std::string& filePath) {
            if (ma_sound_init_from_file(engine.get(), filePath.c_str(), 0, NULL, NULL, &sound) != MA_SUCCESS) {
                throw std::runtime_error("Failed to initialize ma_sound from file: " + filePath);
                // TODO: Maybe don't throw, need to consider this later
            }
        }

        ~MaSound() { ma_sound_uninit(&sound); }
        ma_sound* get() { return &sound; }

        MaSound(const MaSound&) = delete;
        MaSound& operator=(const MaSound&) = delete;

        MaSound(MaSound&& other) noexcept
            : sound(other.sound)
        {
            other.sound = {};  // zero out moved-from object
        }

        MaSound& operator=(MaSound&& other) noexcept {
            if (this != &other) {
                ma_sound_uninit(&sound);
                sound = other.sound;
                other.sound = {};
            }
            return *this;
        }

    private:
        ma_sound sound;
};


class AudioEngine {
    public:
        AudioEngine(ma_device_id* deviceId);
        ~AudioEngine() = default;

        // path can be relative or absolute
        MaSound createSound(const std::string& path);

        void play(MaSound& sound);
        void stop(MaSound& sound);
        void restart(MaSound& sound);

    private:
        MaEngine engine;
};
