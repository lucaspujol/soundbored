#include "raylib.h"
#define CLAY_IMPLEMENTATION
#include "clay.h"
#include "clay_renderer_raylib.h"

#include "ui/UI.hpp"
#include "ui/FontCache.hpp"
#include "ui/sections/Header.hpp"
#include "ui/sections/Sidebar.hpp"
#include "ui/Icons.hpp"
#include "ui/theme/Colors.hpp"
#include "ui/theme/Layout.hpp"

#include <cstdio>
#include <filesystem>

namespace {

void handleClayErrors(Clay_ErrorData errorData) {
    std::fprintf(stderr, "clay: %.*s\n", errorData.errorText.length, errorData.errorText.chars);
}

}

UI::UI(int width, int height, const char *title) {
    Clay_Raylib_Initialize(width, height, title, FLAG_WINDOW_RESIZABLE);
    SetWindowMinSize(800, 500);

    const uint64_t memSize = Clay_MinMemorySize();
    clayMemory = std::make_unique<char[]>(memSize);
    Clay_Arena arena = Clay_CreateArenaWithCapacityAndMemory(memSize, clayMemory.get());
    Clay_Initialize(arena, { (float)width, (float)height }, { handleClayErrors, nullptr });

    using fpath = std::filesystem::path;
    const fpath assets = fpath(GetApplicationDirectory()) / "assets";
    fonts = std::make_unique<FontCache>(assets / "fonts");
    icons = std::make_unique<Icons>(assets / "icons");

    // needs a live Clay context, so after Clay_Initialize
    Clay_SetMeasureTextFunction(FontCache::measure, fonts.get());
}

UI::~UI() {
    // GPU resources must be freed while the window (GL context) still exists
    icons.reset();
    fonts.reset();
    Clay_Raylib_Close();
}

bool UI::shouldClose() const {
    return WindowShouldClose();
}

std::vector<Action> UI::frame(const AppState &appState) {
    std::vector<Action> actions;
    // clay setup
    Clay_SetLayoutDimensions({ (float)GetScreenWidth(), (float)GetScreenHeight() });
    Vector2 mouse = GetMousePosition();
    Clay_SetPointerState({ mouse.x, mouse.y }, IsMouseButtonDown(MOUSE_BUTTON_LEFT));

    // debug panel
    if (IsKeyPressed(KEY_D)) {
        Clay_SetDebugModeEnabled(!Clay_IsDebugModeEnabled());
    }
    if (IsKeyPressed(KEY_F1)) {
        actions.push_back(action::ToggleHotkeys{});
    }

    // last frame's strings are rendered already
    text.clear();

    // layout computing
    Clay_BeginLayout();
    FrameData f { appState, ui, actions, *fonts, *icons, text };
    buildLayout(f);      // internal function to our UI class
    Clay_RenderCommandArray commandArray = Clay_EndLayout(GetFrameTime());

    // raylib rendering
    BeginDrawing();
    ClearBackground(BLACK);
    Clay_Raylib_Render(commandArray, fonts->data());
    EndDrawing();
    return actions;
}

void UI::buildLayout(FrameData &frameData) {
    CLAY(CLAY_ID("root"), Clay_ElementDeclaration {
        .layout = {
            .sizing = theme::expand,
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
        .backgroundColor = theme::background,
    }) {
        Header(frameData);

        CLAY(CLAY_ID("mainContent"), Clay_ElementDeclaration{
            .layout = {
                .sizing = theme::expand,
                .layoutDirection = CLAY_LEFT_TO_RIGHT,
            },
            .backgroundColor = theme::background,
        }) {
            Sidebar(frameData);
            CLAY(CLAY_ID("contentArea"), Clay_ElementDeclaration{
                .layout = {
                    .sizing = theme::expand,
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                },
                .backgroundColor = theme::background,
            }) {}
        }
        CLAY(CLAY_ID("footer"), Clay_ElementDeclaration{
            .layout = {
                .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(theme::footer_size_px), },
                .layoutDirection = CLAY_LEFT_TO_RIGHT,
            },
            .backgroundColor = theme::panel,
            .border = {
                .color = theme::strong_sep,
                .width = { .top = 1 },
            }
        }) {}
    }
}
