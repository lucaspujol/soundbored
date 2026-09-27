#define CLAY_IMPLEMENTATION
#include "clay.h"
#include "clay_renderer_raylib.h"

#include "UI.hpp"
#include "ui/Theme.hpp"

#include <algorithm>
#include <array>
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <string>
#include <string_view>

namespace {

// Clay_String isn't null-terminated: print with an explicit length
void handleClayErrors(Clay_ErrorData errorData) {
    std::fprintf(stderr, "clay: %.*s\n", errorData.errorText.length, errorData.errorText.chars);
}

struct AssetFile {
    uint16_t id;
    std::string_view file;
};

// assets are stored at assets[id], so every id must fit inside its table
template <size_t N>
constexpr bool idsFitTable(const std::array<AssetFile, N> &table) {
    return std::ranges::all_of(table, [](const AssetFile &asset) { return asset.id < N; });
}

constexpr auto fontFiles = std::to_array<AssetFile>({
    { theme::font::display_semibold, "BricolageGrotesque-SemiBold.ttf" },
    { theme::font::display_bold,     "BricolageGrotesque-Bold.ttf" },
    { theme::font::sans_regular,     "IBMPlexSans-Regular.ttf" },
    { theme::font::sans_medium,      "IBMPlexSans-Medium.ttf" },
    { theme::font::sans_semibold,    "IBMPlexSans-SemiBold.ttf" },
    { theme::font::mono_medium,      "IBMPlexMono-Medium.ttf" },
});
static_assert(idsFitTable(fontFiles), "font id without a matching entry in fontFiles");

constexpr auto iconFiles = std::to_array<AssetFile>({
    { theme::icon::upload, "upload.png" },
    { theme::icon::stop,     "stop.png" },
});
static_assert(idsFitTable(iconFiles), "icon id without a matching entry in iconFiles");

// Only the icon's alpha channel matters: its color comes from the element's overlayColor at draw time.
// Mipmaps + trilinear let the GPU pick a pre-shrunk copy for any display size, so the source resolution doesn't matter.
Texture2D loadIcon(const std::filesystem::path &path) {
    Texture2D texture = LoadTexture(path.string().c_str());
    GenTextureMipmaps(&texture);
    SetTextureFilter(texture, TEXTURE_FILTER_TRILINEAR);
    return texture;
}

bool Button(Clay_String text, Texture2D *icon, const theme::ButtonStyle &style) {
    bool clicked = false;

    CLAY(CLAY_SID(text), Clay_ElementDeclaration{
        .layout = {
            .padding = { 16, 16, 16, 16 },
            .childGap = 8,
            .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
        },
        .backgroundColor = Clay_Hovered() ? style.backgroundHover : style.background,
        .cornerRadius = CLAY_CORNER_RADIUS(12),
        .border = { .color = style.border, .width = CLAY_BORDER_OUTSIDE(1) },
    }) {
        clicked = Clay_Hovered() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

        if (icon) {
            CLAY_AUTO_ID(Clay_ElementDeclaration{
                .layout = { .sizing = { CLAY_SIZING_FIXED(theme::icon_size_px), CLAY_SIZING_FIXED(theme::icon_size_px) } },
                // overlay, not backgroundColor: backgroundColor also emits a solid rectangle over the image.
                // the overlay shader keeps the texture's alpha, so this paints the icon's shape in `content`
                .overlayColor = style.content,
                .image = { .imageData = icon },
            }) {}
        }
        CLAY_TEXT(text, Clay_TextElementConfig{
            .textColor = style.content,
            .fontId = theme::font::sans_semibold,
            .fontSize = 24,
        });
    }
    return clicked;
}

}

UI::UI(int width, int height, const char *title) {
    Clay_Raylib_Initialize(width, height, title, FLAG_WINDOW_RESIZABLE);
    SetWindowMinSize(800, 500);

    uint64_t memSize { Clay_MinMemorySize() };
    clayMemory = std::make_unique<char[]>(memSize);
    Clay_Arena arena { Clay_CreateArenaWithCapacityAndMemory(memSize, clayMemory.get()) };

    Clay_Initialize(arena, { (float)width, (float)height }, { handleClayErrors, nullptr });
    Clay_SetDebugModeEnabled(false);

    using fpath = std::filesystem::path;
    const fpath assetDir = fpath(GetApplicationDirectory()) / "assets";

    fonts.resize(fontFiles.size());
    for (const AssetFile &font : fontFiles) {
        const std::string path = (assetDir / "fonts" / font.file).string();
        fonts[font.id] = LoadFontEx(path.c_str(), 48, nullptr, 400);
        SetTextureFilter(fonts[font.id].texture, TEXTURE_FILTER_BILINEAR);
    }
    Clay_SetMeasureTextFunction(Raylib_MeasureText, fonts.data());

    // sized once here and never resized: Clay holds pointers into it as imageData
    icons.resize(iconFiles.size());
    for (const AssetFile &icon : iconFiles) {
        icons[icon.id] = loadIcon(assetDir / "icons" / icon.file);
    }
}

UI::~UI() {
    for (auto &icon : icons) {
        UnloadTexture(icon);
    }
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
    if (IsKeyPressed(KEY_D)) {
        Clay_SetDebugModeEnabled(!Clay_IsDebugModeEnabled());
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
            .sizing = theme::expand,
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
                        CLAY_SIZING_FIXED(theme::sidebar_width_px),
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
            CLAY(CLAY_ID("spacer"), { .layout = { .sizing = { CLAY_SIZING_GROW(0) } } }) {}
            CLAY(CLAY_ID("headerButtons"), Clay_ElementDeclaration{
                .layout = {
                    .sizing = { .height = CLAY_SIZING_GROW(0), },
                    .padding = { .right = 20, },
                    .childGap = 8,
                    .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
                },
            }) {
                if (Button(CLAY_STRING("Import"), &icons[theme::icon::upload], theme::secondaryButton)) {
                    std::cout << "Import button clicked" << std::endl;
                }
                if (Button(CLAY_STRING("Stop all"), &icons[theme::icon::stop], theme::dangerButton)) {
                    std::cout << "Stop all button clicked" << std::endl;
                }
            }
        }
        CLAY(CLAY_ID("mainContent"), Clay_ElementDeclaration{
            .layout = {
                .sizing = theme::expand,
                .layoutDirection = CLAY_LEFT_TO_RIGHT,
            },
            .backgroundColor = theme::background,
        }) {
            CLAY(CLAY_ID("sidebar"), Clay_ElementDeclaration{
                .layout = {
                    .sizing = {
                        CLAY_SIZING_FIXED(theme::sidebar_width_px),
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

void UI::renderLogo() const {
    const uint16_t fontId = theme::font::display_bold;
    const uint16_t fontSize = 32;

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
