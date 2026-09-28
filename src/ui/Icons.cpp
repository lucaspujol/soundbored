#include "Icons.hpp"
#include "raylib.h"
#include "ui/Theme.hpp"

#include <string_view>

namespace {

    constexpr auto files = std::to_array<std::string_view>({
        "upload.png",
        "stop.png",
    });
    static_assert(files.size() == theme::icon::icon_count, "every icon needs exactly one file");

}

Icons::Icons(const std::filesystem::path &assetDir) {
    for (size_t i = 0; i < textures.size(); i++) {
        Image image = LoadImage((assetDir / files[i]).string().c_str());
        ImageResize(&image,theme::icon_size_px, theme::icon_size_px);

        textures[i] = LoadTextureFromImage(image);
        UnloadImage(image);
        SetTextureFilter(textures[i], TEXTURE_FILTER_BILINEAR);
    }
}

Icons::~Icons() {
    for (Texture2D &texture : textures) {
        UnloadTexture(texture);
    }
}