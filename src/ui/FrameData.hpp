#pragma once

#include <vector>

#include "data/Actions.hpp"
#include "data/AppState.hpp"

class FontCache;
class Icons;
class TextArena;

// "All sounds" is not a real Board: it takes this reserved id, so real board ids start at 1
inline constexpr BoardId allSoundsBoard = 0;

// UI-only state: not saved, audio doesn't care
struct UiState {
    BoardId selectedBoard = allSoundsBoard;
};

// everything sections need for one frame. Built in UI::frame(), passed down by reference
struct FrameData {
    const AppState &state;
    UiState &ui;
    std::vector<Action> &actions;   // sections push here, main's apply() handles them
    FontCache &fonts;
    Icons &icons;
    TextArena &text;
};
