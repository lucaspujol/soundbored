#pragma once

#include <cstdint>
#include <filesystem>
#include <map>

#include "raylib.h"
#include "ui/theme/Icons.hpp"

class Icons {
    public:
        explicit Icons(std::filesystem::path assetDir);
        ~Icons();

        Icons(const Icons&) = delete;
        Icons& operator=(const Icons&) = delete;

        // texture resized to exactly size x size px, so it draws 1:1 (sharp)
        Texture2D *get(theme::icon::Id id, uint16_t size = theme::icon_size_px);

    private:
        struct Key {
            theme::icon::Id id;
            uint16_t size;
            auto operator<=>(const Key&) const = default;
        };

        std::filesystem::path dir;
        std::map<Key, Texture2D> textures;  // std::map: pointers stay valid while Clay holds them
};
