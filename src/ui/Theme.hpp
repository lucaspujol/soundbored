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

    // danger (destructive actions): deeper than the red accent
    inline constexpr Clay_Color danger       = { 210,  45,  45, 255 };
    inline constexpr Clay_Color danger_hover = { 232,  62,  62, 255 };

    inline constexpr Clay_Color transparent = { 0, 0, 0, 0 };

    namespace font {
        inline constexpr uint16_t display_semibold = 0;     // headings
        inline constexpr uint16_t display_bold     = 1;     // logo
        inline constexpr uint16_t sans_regular     = 2;     // body
        inline constexpr uint16_t sans_medium      = 3;     // ui
        inline constexpr uint16_t sans_semibold    = 4;     // ui emphasis
        inline constexpr uint16_t mono_medium      = 5;     // key chips, times, percentages
    }

    inline constexpr Clay_Sizing expand = {
        .width = CLAY_SIZING_GROW(0),
        .height = CLAY_SIZING_GROW(0),
    };

    inline constexpr Clay_TextElementConfig bodyText {
        .textColor = text_primary,
        .fontId = font::sans_regular,
        .fontSize = 20,
    };

    // layout constants
    inline constexpr size_t header_size_px = 90;
    inline constexpr size_t footer_size_px = 90;
    inline constexpr size_t sidebar_width_px = 284;

    struct ButtonStyle {
        Clay_Color background;
        Clay_Color backgroundHover;
        Clay_Color border;
        Clay_Color content;
    };

    inline constexpr ButtonStyle secondaryButton {
        .background = control,
        .backgroundHover = selected,
        .border = strong_sep,
        .content = text_primary,
    };

    inline constexpr ButtonStyle dangerButton {
        .background = danger,
        .backgroundHover = danger_hover,
        .border = transparent,
        .content = text_primary,
    };
}
