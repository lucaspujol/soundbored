#include "ui/FontCache.hpp"
#include "clay_renderer_raylib.h"

#include <array>
#include <string>
#include <string_view>

namespace {
    constexpr auto files = std::to_array<std::string_view>({
        "BricolageGrotesque-SemiBold.ttf",
        "BricolageGrotesque-Bold.ttf",
        "IBMPlexSans-Regular.ttf",
        "IBMPlexSans-Medium.ttf",
        "IBMPlexSans-SemiBold.ttf",
        "IBMPlexMono-Medium.ttf",
    });
    static_assert(files.size() == theme::font::face_count, "every Face needs exactly one file");
}

FontCache::FontCache(std::filesystem::path dir) : dir(std::move(dir)) {}

FontCache::~FontCache() {
    for (Font &font : fonts) {
        UnloadFont(font);
    }
}

Clay_TextElementConfig FontCache::text(theme::font::Face face, uint16_t size, Clay_Color color) {
    return {
        .textColor = color,
        .fontId = idFor(face, size),
        .fontSize = size,
    };
}

uint16_t FontCache::idFor(theme::font::Face face, uint16_t size) {
    auto found = ids.find({ face, size });
    if (found != ids.end()) {
        return found->second;
    }

    const std::string path = (dir / files[face]).string();
    Font font = LoadFontEx(path.c_str(), size, nullptr, 400);
    
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

    const uint16_t id = static_cast<uint16_t>(fonts.size());
    fonts.push_back(font);
    ids[{ face, size }] = id;
    return id;
}

Clay_Dimensions FontCache::measure(Clay_StringSlice text, Clay_TextElementConfig *config, void *userData) {
    FontCache *cache = static_cast<FontCache*>(userData);
    return Raylib_MeasureText(text, config, cache->data());
}