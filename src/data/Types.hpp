#pragma once

#include <cstdint>

using SoundId = uint32_t;
using BoardId = uint32_t;

enum class RetriggerMode {
    Overlap,
    Restart,
    Stop,
};

enum Mods : uint8_t {
    ModNone  = 0,
    ModCtrl  = 1 << 0,
    ModAlt   = 1 << 1,
    ModShift = 1 << 2,
};

struct Keybind {
    int key;
    uint8_t mods;
};
