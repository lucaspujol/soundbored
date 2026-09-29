#pragma once

#include <memory>
#include <vector>

#include "data/Actions.hpp"
#include "data/AppState.hpp"

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
    void buildLayout(const AppState &appState, std::vector<Action> &actions);

    // UI-only state: not saved, audio doesn't care
    BoardId selectedBoard = 0;

    std::unique_ptr<char[]> clayMemory;
    std::unique_ptr<FontCache> fonts;
    std::unique_ptr<Icons> icons;
};
