#include "raylib.h"
#include <iostream>
#define CLAY_IMPLEMENTATION
#include "clay.h"
#include "clay_renderer_raylib.h"

#include "UI.hpp"
#include "ui/Theme.hpp"

#include <algorithm>
#include <array>
#include <cstdio>
#include <filesystem>
#include <string>
#include <string_view>

namespace {

void handleClayErrors(Clay_ErrorData errorData) {
    std::fprintf(stderr, "clay: %s\n", errorData.errorText.chars);
}

struct FontFile {
    uint16_t id;
    std::string_view file;
};

constexpr auto fontFiles = std::to_array<FontFile>({
    { theme::font::display_semibold, "BricolageGrotesque-SemiBold.ttf" },
    { theme::font::display_bold,     "BricolageGrotesque-Bold.ttf" },
    { theme::font::sans_regular,     "IBMPlexSans-Regular.ttf" },
    { theme::font::sans_medium,      "IBMPlexSans-Medium.ttf" },
    { theme::font::sans_semibold,    "IBMPlexSans-SemiBold.ttf" },
    { theme::font::mono_medium,      "IBMPlexMono-Medium.ttf" },
});

// if a font id is used, it must have a matching entry in fontFiles.
static_assert(std::ranges::all_of(
    fontFiles,
    [](const FontFile &font) {
        return font.id < fontFiles.size();
    }),
    "font id without a matching entry in fontFiles");

}

UI::UI(int width, int height, const char *title) {
    Clay_Raylib_Initialize(width, height, title, FLAG_WINDOW_RESIZABLE);
    SetWindowMinSize(800, 500);

    uint64_t memSize { Clay_MinMemorySize() };
    clayMemory = std::make_unique<char[]>(memSize);
    Clay_Arena arena { Clay_CreateArenaWithCapacityAndMemory(memSize, clayMemory.get()) };

    Clay_Initialize(arena, { (float)width, (float)height }, { handleClayErrors, nullptr });
    Clay_SetDebugModeEnabled(false);

    fonts.resize(fontFiles.size());
    using fpath = std::filesystem::path;
    const fpath fontDir = fpath(GetApplicationDirectory()) / "assets" / "fonts";
    for (const FontFile &font : fontFiles) {
        const std::string path = (fontDir / font.file).string();
        fonts[font.id] = LoadFontEx(path.c_str(), 48, nullptr, 400);
        SetTextureFilter(fonts[font.id].texture, TEXTURE_FILTER_BILINEAR);
    }
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
    Clay_SetPointerState({ mouse.x, mouse.y }, IsMouseButtonDown(MOUSE_BUTTON_LEFT));

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
            .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0), },
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
        .backgroundColor = theme::background,
    }) {
        CLAY(CLAY_ID("header"), Clay_ElementDeclaration{
            .layout = {
                .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(theme::header_size_px), },
                .layoutDirection = CLAY_LEFT_TO_RIGHT,
            },
            .backgroundColor = theme::panel,
            .border = {
                .color = theme::strong_sep,
                .width = { .bottom = 1 },
            }
        }) {
            CLAY(CLAY_ID("logo"), Clay_ElementDeclaration{
                .layout = {
                    .sizing = {
                        CLAY_SIZING_GROW(
                            .min = 200,
                            .max = theme::sidebar_width_px
                        ),
                        CLAY_SIZING_GROW(0),
                    },
                    .padding = { .left = 20, },
                    .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
                },
            }) { renderLogo(); }
            CLAY(CLAY_ID("searchBar"), Clay_ElementDeclaration{
                .layout = {
                    .sizing = {
                        CLAY_SIZING_GROW(
                            .min = 200,
                            .max = theme::sidebar_width_px
                        ),
                        CLAY_SIZING_GROW(0),
                    },
                    .padding = { .left = 20, },
                    .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
                },
            }) { renderSearchBar(); }
        }
        CLAY(CLAY_ID("mainContent"), Clay_ElementDeclaration{
            .layout = {
                .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0), },
                .layoutDirection = CLAY_LEFT_TO_RIGHT,
            },
            .backgroundColor = theme::background,
        }) {
            CLAY(CLAY_ID("sidebar"), Clay_ElementDeclaration{
                .layout = {
                    .sizing = {
                        CLAY_SIZING_GROW(
                            .min = 200,
                            .max = theme::sidebar_width_px
                        ),
                        CLAY_SIZING_GROW(0),
                    },
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                },
                .backgroundColor = theme::panel,
                .border = {
                    .color = theme::strong_sep,
                    .width = { .right = 1 },
                }
            }) {}
            CLAY(CLAY_ID("contentArea"), Clay_ElementDeclaration{
                .layout = {
                    .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0), },
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

void UI::renderLogo() const {
    uint16_t fontId = theme::font::display_bold;
    uint16_t fontSize = 32;

    CLAY_TEXT(CLAY_STRING("sound"), CLAY_TEXT_CONFIG({
        .textColor = theme::text_primary,
        .fontId = fontId,
        .fontSize = fontSize,
    }));
    CLAY_TEXT(CLAY_STRING("bored"), CLAY_TEXT_CONFIG({
        .textColor = theme::orange,
        .fontId = fontId,
        .fontSize = fontSize,
    }));
}

void UI::renderSearchBar() const {
    // TODO
    CLAY_TEXT(CLAY_STRING("Search bar placeholder"), theme::bodyText);
}