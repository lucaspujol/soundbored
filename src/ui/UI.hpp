#pragma once

#include <memory>
#include <vector>

#include "data/Actions.hpp"
#include "data/AppState.hpp"
#include "ui/FrameData.hpp"
#include "ui/TextArena.hpp"

class FontCache;
class Icons;

class UI {
public:
    UI(int width, int height, const char *title);
    ~UI();

    UI(const UI&) = delete;
    UI& operator=(const UI&) = delete;

    bool shouldClose() const;
    std::vector<Action> frame(const AppState &appState);

private:
    void buildLayout(FrameData &f);

    UiState ui;

    // per-frame strings, cleared at the start of frame()
    TextArena text;

    std::unique_ptr<char[]> clayMemory;
    std::unique_ptr<FontCache> fonts;
    std::unique_ptr<Icons> icons;
};
