#pragma once

#include <memory>
#include <vector>

#include "raylib.h"

class UI {
public:
    UI(int width, int height, const char *title);
    ~UI();

    UI(const UI&) = delete;
    UI& operator=(const UI&) = delete;

    bool shouldClose() const;
    void frame();

private:
    void buildLayout();

    std::unique_ptr<char[]> clayMemory;
    std::vector<Font> fonts;
    std::vector<Texture2D> icons;

    // helpers
    void renderLogo() const;
    void renderSearchBar() const;
};