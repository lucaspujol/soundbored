#pragma once

#include <cstdint>

#include "clay.h"
#include "ui/theme/Colors.hpp"

namespace theme {
    struct SegmentedStyle {
        Clay_Color background;
        Clay_Color border;
        Clay_Color segmentSelected;
        Clay_Color segmentHover;
        Clay_Color text;
        Clay_Color textSelected;
        uint16_t padding = 4;           // around the segments
        uint16_t segmentPaddingY = 10;
        float radius = 12;
    };

    inline constexpr SegmentedStyle segmented {
        .background = background,
        .border = strong_sep,
        .segmentSelected = selected,
        .segmentHover = control,
        .text = text_secondary,
        .textSelected = text_primary,
    };
}
