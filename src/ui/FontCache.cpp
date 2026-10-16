#include "ui/FontCache.hpp"

#include <algorithm>
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
    static_assert(files.size() == static_cast<uint16_t>(theme::font::Face::face_count), "every Face needs exactly one file");
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

    const std::string path = (dir / files[static_cast<size_t>(face)]).string();
    Font font = LoadFontEx(path.c_str(), size, nullptr, 400);
    
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

    const uint16_t id = static_cast<uint16_t>(fonts.size());
    fonts.push_back(font);
    ids[{ face, size }] = id;
    return id;
}

// replaces Raylib_MeasureText, which indexes glyphs per byte: any UTF-8 character ("·", "é")
// reads out of bounds. Decodes codepoints and advances like DrawTextEx, so measure matches drawing
Clay_Dimensions FontCache::measure(Clay_StringSlice text, Clay_TextElementConfig *config, void *userData) {
    FontCache *cache = static_cast<FontCache*>(userData);
    Font font = cache->fonts[config->fontId];
    if (!font.glyphs) {
        font = GetFontDefault();
    }
    const float scale = config->fontSize / static_cast<float>(font.baseSize);

    float line = 0;
    float widest = 0;
    for (int i = 0; i < text.length;) {
        int bytes = 0;
        const int codepoint = GetCodepointNext(text.chars + i, &bytes);
        i += bytes;
        if (codepoint == '\n') {
            widest = std::max(widest, line);
            line = 0;
            continue;
        }
        const int g = GetGlyphIndex(font, codepoint);
        const float advance = font.glyphs[g].advanceX != 0 ? font.glyphs[g].advanceX
                                                           : font.recs[g].width + font.glyphs[g].offsetX;
        line += advance * scale + config->letterSpacing;
    }
    return { std::max(widest, line), static_cast<float>(config->fontSize) };
}