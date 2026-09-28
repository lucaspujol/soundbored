#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <vector>

#include "clay.h"
#include "raylib.h"
#include "ui/Theme.hpp"

class FontCache {
public:
    explicit FontCache(std::filesystem::path dir);
    ~FontCache();

    FontCache(const FontCache&) = delete;
    FontCache& operator=(const FontCache&) = delete;

    Clay_TextElementConfig text(theme::font::Face face, uint16_t size, Clay_Color color);

    Font *data() { return fonts.data(); }

    static Clay_Dimensions measure(Clay_StringSlice text, Clay_TextElementConfig *config, void *userData);

private:
    struct Key {
        theme::font::Face face;
        uint16_t size;
        auto operator<=>(const Key&) const = default;
    };

    uint16_t idFor(theme::font::Face face, uint16_t size);

    std::filesystem::path dir;
    std::vector<Font> fonts;           // indexed by Clay's fontId
    std::map<Key, uint16_t> ids;       // (face, size) -> index into fonts
};