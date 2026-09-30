#pragma once

#include "data/AppState.hpp"
#include "ui/TextArena.hpp"
#include "ui/Icons.hpp"

#include <optional>

class FontCache;

struct SidebarActions {
    std::optional<BoardId> boardClicked;

};

SidebarActions Sidebar(FontCache &fonts, Icons &icons, TextArena &text, const AppState &state, BoardId selectedBoard);
