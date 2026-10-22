#pragma once

#include "data/Types.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

struct SoundMeta {
    SoundId id;
    std::string name;
    std::string path;
    uint64_t durationMs;
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
    std::vector<SoundMeta> sounds;
    std::vector<Board> boards;
};

struct Settings {
    Output physicalOut;
    std::optional<Output> virtualOut;
    RetriggerMode retrigger;
    bool hotkeysActive = true;
};

struct Playback {
    std::optional<SoundId> currentlyPlaying;
    uint64_t positionMs;
};

struct PersistentState {
    Library library;
    Settings settings;

    bool valid() { return false; }; // TODO impl
};

struct AppState {
    Library library;
    Settings settings;
    Playback playback;
    std::vector<std::string> devices;
};

// Note that sound related data must still be read and updated from actual audio files (eg. durationMs)
inline AppState fromPersisted(PersistentState state) {
    return {
        .library = state.library,
        .settings = state.settings,
        .playback = {},
        .devices = {}
    };
};