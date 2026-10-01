#pragma once

#include <cstdint>

namespace theme {
    // font faces. Each face is loaded at a pixel size the first time that size is used (see FontCache)
    namespace font {
        enum class Face : uint16_t {
            display_semibold,   // headings
            display_bold,       // logo
            sans_regular,       // body
            sans_medium,        // ui
            sans_semibold,      // ui emphasis
            mono_medium,        // key chips, times, percentages

            face_count,         // must be last, used to check that every face has a file
        };
    }
}
