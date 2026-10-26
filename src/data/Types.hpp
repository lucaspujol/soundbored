#pragma once

#include <cstdint>
#include <string>

using SoundId = uint32_t;
using BoardId = uint32_t;

enum class RetriggerMode {
    Overlap,
    Restart,
    Stop,
    Unknown, // bad state, error case
};

inline std::string retriggerModeToString(RetriggerMode mode) {
    switch (mode) {
        case RetriggerMode::Overlap: return "Overlap";
        case RetriggerMode::Restart: return "Restart";
        case RetriggerMode::Stop: return "Stop";
        default: return "Unknown";
    }
}

inline RetriggerMode stringToRetriggerMode(std::string in) {
    if (in == "Overlap") return RetriggerMode::Overlap;
    else if (in == "Restart") return RetriggerMode::Restart;
    else if (in == "Stop") return RetriggerMode::Stop;
    else return RetriggerMode::Unknown;
}

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

enum class OutputSlot { Monitor, Mic };