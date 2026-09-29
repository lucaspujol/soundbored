#pragma once

#include <cstddef>

#include "clay.h"

namespace theme {
    inline constexpr Clay_Sizing expand = {
        .width = CLAY_SIZING_GROW(0),
        .height = CLAY_SIZING_GROW(0),
    };

    // layout constants
    inline constexpr size_t header_size_px = 90;
    inline constexpr size_t footer_size_px = 90;
    inline constexpr size_t sidebar_width_px = 350;
}
