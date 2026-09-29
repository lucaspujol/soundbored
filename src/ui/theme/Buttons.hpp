#pragma once

#include <cstdint>

#include "clay.h"
#include "ui/theme/Colors.hpp"

namespace theme {
    struct ButtonStyle {
        Clay_Color background;
        Clay_Color backgroundHover;
        Clay_Color border;
        Clay_Color content;
        Clay_Padding padding = { 16, 16, 12, 12 };
        uint16_t gap = 8;
        float radius = 12;
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

    inline constexpr ButtonStyle compactButton {
        .background = control,
        .backgroundHover = selected,
        .border = strong_sep,
        .content = text_primary,
        .padding = { 8, 8, 4, 4 },
        .radius = 6,
    };
}
