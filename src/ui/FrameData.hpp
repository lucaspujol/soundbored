#pragma once

#include <vector>

#include "data/Actions.hpp"
#include "data/AppState.hpp"

class FontCache;
class Icons;
class TextArena;

// "All sounds" is not a real Board: it takes this reserved id, so real board ids start at 1
inline constexpr BoardId allSoundsBoard = 0;

struct UiState {
    BoardId selectedBoard = allSoundsBoard;
};

struct FrameData {
    const AppState &state;
    UiState &ui;
    std::vector<Action> &actions;
    FontCache &fonts;
    Icons &icons;
    TextArena &text;
};
