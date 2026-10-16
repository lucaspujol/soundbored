#pragma once

#include <vector>

#include "data/Actions.hpp"
#include "data/AppState.hpp"
#include "ui/SmoothScroll.hpp"

class FontCache;
class Icons;
class TextArena;

// "All sounds" is not a real Board: it takes this reserved id, so real board ids start at 1
inline constexpr BoardId allSoundsBoard = 0;

// one rule for board counts and the grid filter
inline bool inBoard(const SoundData &sound, BoardId board) {
    return board == allSoundsBoard || sound.board == board;
}

struct UiState {
    BoardId selectedBoard = allSoundsBoard;
    SmoothScroll sidebarScroll;
};

struct FrameData {
    const AppState &state;
    UiState &ui;
    std::vector<Action> &actions;
    FontCache &fonts;
    Icons &icons;
    TextArena &text;
};
