#pragma once

#include "cord.hpp"

#include <string>
#include <vector>
#include <optional>


class Config {
    public:
        Config(bool verbose = false) : verbose(verbose) {}
        ~Config() = default;

        bool load();
        bool save();

        // Getters

        std::optional<std::string> getPhysicalDevice() const { return physicalDevice; }
        std::optional<std::string> getVirtualDevice() const { return virtualDevice; }
        std::vector<std::string> getSoundPaths() const { return soundPaths; }
        std::optional<int> getOnRetrigger() const { return onRetrigger; }

        // Setters

        void setPhysicalDevice(const std::string& device) { physicalDevice = device; }
        void setVirtualDevice(const std::string& device) { virtualDevice = device; }
        void setSoundPaths(const std::vector<std::string>& paths) { soundPaths = paths; }
        void setOnRetrigger(int retrigger) { onRetrigger = retrigger; }

    private:
        static constexpr const char* CONFIG_FILE_PATH = "soundbored.conf";

        static cord::Schema getSchema();
        cord::Result result;

        bool verbose = false;

        std::optional<std::string> physicalDevice;
        std::optional<std::string> virtualDevice;

        std::vector<std::string> soundPaths;

        std::optional<int> onRetrigger;
};
