#pragma once

#include <memory>

class FontCache;
class Icons;

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
    std::unique_ptr<FontCache> fonts;
    std::unique_ptr<Icons> icons;
};