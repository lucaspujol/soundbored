#pragma once

#include "data/Types.hpp"

#include <optional>
#include <string>
#include <vector>

// TODO: MAYBE RENAME TO SOUNDMETA? name already in use in @audio/AudioManager.hpp
struct SoundData {
    SoundId id;
    std::string name;
    std::string path;
    uint32_t durationMs;
    float volume;
    std::optional<Keybind> keybind;
    // add color/tint, but could be tied to a board. we can imagine user picks a color for his board(s)
    BoardId board;
};

struct Board {
    BoardId id;
    std::string name;
};

struct Output {
    std::string deviceName;
    float volume;
};

struct Library {
    std::vector<SoundData> sounds;
    std::vector<Board> boards;
};

struct Settings {
    Output physicalOut;
    std::optional<Output> virtualOut;
    RetriggerMode retrigger;
};

struct Playback {
    std::optional<SoundId> currentlyPlaying;
    uint32_t positionMs;
};

struct AppState {
    Library library;
    Settings settings;
    Playback playback;
    std::vector<std::string> devices;
};
