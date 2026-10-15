#pragma once

#include <cstddef>
#include <cstdint>

namespace theme {
    // icon ids: index into Icons
    namespace icon {
        enum Id : uint16_t {
            upload,
            stop,
            headphones,
            mic,
            chevron_down,

            icon_count,     // must be last, used to check that every icon has a file
        };
    }

    inline constexpr size_t icon_size_px = 16;
}
