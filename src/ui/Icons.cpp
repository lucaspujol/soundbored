#include "Icons.hpp"
#include "raylib.h"

#include <array>
#include <string_view>

namespace {

    constexpr auto files = std::to_array<std::string_view>({
        "upload.png",
        "stop.png",
        "headphones.png",
        "mic.png",
        "chevron-down.png",
    });
    static_assert(files.size() == theme::icon::icon_count, "every icon needs exactly one file");

}

Icons::Icons(std::filesystem::path assetDir) : dir(std::move(assetDir)) {}

Icons::~Icons() {
    for (auto &[key, texture] : textures) {
        UnloadTexture(texture);
    }
}

Texture2D *Icons::get(theme::icon::Id id, uint16_t size) {
    auto found = textures.find({ id, size });
    if (found != textures.end()) {
        return &found->second;
    }

    Image image = LoadImage((dir / files[id]).string().c_str());
    ImageResize(&image, size, size);
    Texture2D texture = LoadTextureFromImage(image);
    UnloadImage(image);
    SetTextureFilter(texture, TEXTURE_FILTER_BILINEAR);

    return &(textures[{ id, size }] = texture);
}
