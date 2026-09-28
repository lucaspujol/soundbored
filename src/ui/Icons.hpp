#pragma once

#include <array>
#include <filesystem>

#include "raylib.h"
#include "ui/Theme.hpp"

class Icons {
    public:
        explicit Icons(const std::filesystem::path &assetDir);
        ~Icons();

        Icons(const Icons&) = delete;
        Icons& operator=(const Icons&) = delete;

        Texture2D *get(theme::icon::Id id) { return &textures[id]; }

    private:
        std::array<Texture2D, theme::icon::icon_count> textures;
};