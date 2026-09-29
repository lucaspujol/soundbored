#include "data/AppState.hpp"
#include "data/Types.hpp"
#include <format>
#include <iostream>

Library stubLibrary() {
    return Library {
        .sounds = {
            { 1, "Sound 1", "path/to/sound1.wav", 3000, 0.8f, std::nullopt, 1 },
            { 2, "Sound 2", "path/to/sound2.wav", 5000, 0.5f, Keybind{ 'A', ModCtrl | ModAlt }, 1 },
            { 3, "Sound 3", "path/to/sound3.wav", 2000, 1.0f, Keybind{ 'B', ModAlt }, 2 },
        },
        .boards = {
            { 1, "Board 1" },
            { 2, "Board 2" },
        },
    };
}

Settings stubSettings() {
    return Settings {
        .physicalOut = { "Device 2", 0.8f },
        .virtualOut = Output { "Virtual 1", 0.5f },
        .retrigger = RetriggerMode::Restart,
    };
}

Playback stubPlayback() {
    return Playback {
        .currentlyPlaying = 2,
        .positionMs = 1500,
    };
}

AppState getStubAppState() {
    return AppState {
        .library = stubLibrary(),
        .settings = stubSettings(),
        .playback = stubPlayback(),
        .devices = {
            "Device 1", "Device 2", "Device 3",
            "Virtual 1", "Virtual 2", "Virtual 3",
        },
    };
}

std::string retriggerModeToString(RetriggerMode mode) {
    switch (mode) {
        case RetriggerMode::Overlap: return "Overlap";
        case RetriggerMode::Restart: return "Restart";
        case RetriggerMode::Stop: return "Stop";
        default: return "Unknown";
    }
}

void displayAppState(const AppState &appState) {
    std::cout << "\nSettings:\n";
    std::cout << std::format("  Physical Output: {} (volume: {})\n",
        appState.settings.physicalOut.deviceName, appState.settings.physicalOut.volume);
    if (appState.settings.virtualOut) {
        std::cout << std::format("  Virtual Output: {} (volume: {})\n",
            appState.settings.virtualOut->deviceName, appState.settings.virtualOut->volume);
    } else {
        std::cout << "  Virtual Output: None\n";
    }
    std::cout << std::format("  Retrigger Mode: {}\n", retriggerModeToString(appState.settings.retrigger));

    std::cout << "\nLibrary:\n";
    for (const auto &sound : appState.library.sounds) {
        std::cout << std::format("  Sound {}: {} ({} ms, volume: {}, board: {})\n",
            sound.id, sound.name, sound.durationMs, sound.volume, sound.board);
    }
    for (const auto &board : appState.library.boards) {
        std::cout << std::format("  Board {}: {}\n", board.id, board.name);
    }

    std::cout << "\nPlayback:\n";
    if (appState.playback.currentlyPlaying) {
        std::cout << std::format("  Currently Playing Sound ID: {} (position: {} ms)\n",
            *appState.playback.currentlyPlaying, appState.playback.positionMs);
    } else {
        std::cout << "  Currently Playing Sound ID: None\n";
    }

    std::cout << "\nDevices:\n";
    for (const auto &device : appState.devices) {
        std::cout << std::format("  Device: {}\n", device);
    }
    std::cout << std::endl;
}
