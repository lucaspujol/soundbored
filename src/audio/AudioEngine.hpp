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

            engine = (ma_engine*)malloc(sizeof(ma_engine));
            if (engine == nullptr) throw std::runtime_error("Failed to allocate memory for ma_engine.");
            //                       ^ TODO: Maybe don't throw, need to consider this later

            if (ma_engine_init(&engineConfig, engine) != MA_SUCCESS) {
                free(engine);
                throw std::runtime_error("Failed to initialize ma_engine.");
                // TODO: Maybe don't throw, need to consider this later
            }
        }

        ~MaEngine() {
            if (engine) {
                ma_engine_uninit(engine);
                free(engine);
            }
        }

        MaEngine(const MaEngine&) = delete;
        MaEngine& operator=(const MaEngine&) = delete;

        ma_engine* get() const {
            return engine;
        }

    private:
        ma_engine* engine;
};

/**
 * A RAII wrapper class for miniaudio sound (ma_sound)
 */
class MaSound {
    public:
        MaSound(MaEngine* engine, const std::string& filePath) {
            sound = (ma_sound*)malloc(sizeof(ma_sound));
            if (sound == nullptr) throw std::runtime_error("Failed to allocate memory for ma_sound.");
            //                      ^ TODO: Maybe don't throw, need to consider this later

            if (ma_sound_init_from_file(engine->get(), filePath.c_str(), 0, NULL, NULL, sound) != MA_SUCCESS) {
                free(sound);
                throw std::runtime_error("Failed to initialize ma_sound from file: " + filePath);
                // TODO: Maybe don't throw, need to consider this later
            }
        }

        ~MaSound() {
            if (sound) {
                ma_sound_uninit(sound);
                free(sound);
            }
        }
        
        MaSound(const MaSound&) = delete;
        MaSound& operator=(const MaSound&) = delete;

        ma_sound* get() const {
            return sound;
        }

    private:
        ma_sound* sound;
};


class AudioEngine {
    public:
        AudioEngine(ma_device_id* deviceId);
        ~AudioEngine();

        // path can be relative or absolute
        MaSound createSound(const std::string& path) const;

        void play(MaSound* sound) const;
        void stop(MaSound* sound) const;
        void restart(MaSound* sound) const;

    private:
        MaEngine* engine;
};
