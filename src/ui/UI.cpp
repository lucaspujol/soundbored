#define CLAY_IMPLEMENTATION
#include "clay.h"
#include "clay_renderer_raylib.h"

#include "UI.hpp"
#include "ui/Theme.hpp"

#include <cstdio>

namespace {

void handleClayErrors(Clay_ErrorData errorData) {
    std::fprintf(stderr, "clay: %s\n", errorData.errorText.chars);
}

}

UI::UI(int width, int height, const char *title) {
    Clay_Raylib_Initialize(width, height, title, FLAG_WINDOW_RESIZABLE);

    uint64_t memSize { Clay_MinMemorySize() };
    clayMemory = std::make_unique<char[]>(memSize);
    Clay_Arena arena { Clay_CreateArenaWithCapacityAndMemory(memSize, clayMemory.get()) };

    Clay_Initialize(arena, { (float)width, (float)height }, { handleClayErrors, nullptr });
    Clay_SetDebugModeEnabled(false);
    
    fonts.resize(1);
    fonts[0] = LoadFontEx("/usr/share/fonts/truetype/JetBrainsMono-Regular.ttf", 48, 0, 400);
    SetTextureFilter(fonts[0].texture, TEXTURE_FILTER_BILINEAR);
    Clay_SetMeasureTextFunction(Raylib_MeasureText, fonts.data());
}

UI::~UI() {
    for (auto &font : fonts) {
        UnloadFont(font);
    }
    Clay_Raylib_Close();
}

bool UI::shouldClose() const {
    return WindowShouldClose();
}

void UI::frame() {
    // clay setup
    Clay_SetLayoutDimensions({ (float)GetScreenWidth(), (float)GetScreenHeight() });
    Vector2 mouse = GetMousePosition();
    Clay_SetPointerState({ mouse.x, mouse.y }, MOUSE_LEFT_BUTTON);

    // debug panel
    bool debugEnabled = Clay_IsDebugModeEnabled();
    if (IsKeyPressed(KEY_D)) {
        debugEnabled = !debugEnabled;
        Clay_SetDebugModeEnabled(debugEnabled);
    }
    
    // layout computing
    Clay_BeginLayout();
    buildLayout();      // internal function to our UI class
    Clay_RenderCommandArray commandArray = Clay_EndLayout(GetFrameTime());

    // raylib rendering
    BeginDrawing();
    ClearBackground(BLACK);
    Clay_Raylib_Render(commandArray, fonts.data());
    EndDrawing();
}

void UI::buildLayout() {
    CLAY(CLAY_ID("root"), Clay_ElementDeclaration {
        .layout = {
            .sizing = {
                .width  = CLAY_SIZING_GROW(0),
                .height = CLAY_SIZING_GROW(0),
            },
            .padding = CLAY_PADDING_ALL(10),
            .childGap = 16,
            .layoutDirection = CLAY_LEFT_TO_RIGHT,
        },
        .backgroundColor = theme::background,
    }) {}
}