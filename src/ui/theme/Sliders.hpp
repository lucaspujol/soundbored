#pragma once

#include "clay.h"
#include "ui/theme/Colors.hpp"

namespace theme {
    struct SliderStyle {
        Clay_Color track;
        Clay_Color fill;
        Clay_Color thumb;
        float trackHeight = 4;
        float thumbSize = 14;    // also the height of the clickable area
    };

    inline constexpr SliderStyle slider {
        .track = control,
        .fill = orange,
        .thumb = text_primary,
    };
}
