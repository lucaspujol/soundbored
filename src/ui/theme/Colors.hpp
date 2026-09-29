#pragma once

#include "clay.h"

namespace theme {
    // neutrals
    inline constexpr Clay_Color background = { 20, 20, 23, 255 };
    inline constexpr Clay_Color panel      = { 27, 27, 32, 255 };
    inline constexpr Clay_Color cards      = { 34, 34, 40, 255 };
    inline constexpr Clay_Color control    = { 42, 42, 49, 255 };
    inline constexpr Clay_Color selected   = { 52, 52, 60, 255 };
    inline constexpr Clay_Color strong_sep = { 60, 60, 69, 255 };

    // text
    inline constexpr Clay_Color text_primary   = { 236, 236, 240, 255 };
    inline constexpr Clay_Color text_secondary = { 184, 184, 193, 255 };
    inline constexpr Clay_Color text_tertiary  = { 150, 150, 160, 255 };

    // accents
    inline constexpr Clay_Color orange      = { 240, 138,  66, 255 };
    inline constexpr Clay_Color blue        = {  94, 168, 242, 255 };
    inline constexpr Clay_Color purple      = { 182, 144, 242, 255 };
    inline constexpr Clay_Color green       = {  79, 191, 128, 255 };
    inline constexpr Clay_Color red         = { 240, 100,  95, 255 };

    // danger
    inline constexpr Clay_Color danger       = { 210,  45,  45, 255 };
    inline constexpr Clay_Color danger_hover = { 232,  62,  62, 255 };

    inline constexpr Clay_Color transparent = { 0, 0, 0, 0 };
}
