#include "ui/sections/SoundGrid.hpp"

#include "data/AppState.hpp"
#include "ui/ClayHelpers.hpp"
#include "ui/FontCache.hpp"
#include "ui/FrameData.hpp"
#include "ui/TextArena.hpp"

#include "ui/theme/Layout.hpp"
#include "ui/theme/Colors.hpp"
#include "ui/theme/Grid.hpp"

#include <format>
#include <algorithm>
#include <cmath>
#include <vector>

namespace {
    void BoardTitle(FrameData &frameData) {
        CLAY(CLAY_ID("boardTitle"), Clay_ElementDeclaration{
            .layout = {
                .sizing = { .width = CLAY_SIZING_GROW(0) },
                .padding = { .left = theme::grid::padding, .right = theme::grid::padding, .top = 24, .bottom = 12 },
                .childGap = 12,
                .childAlignment = { .y = CLAY_ALIGN_Y_BOTTOM },
            },
            .clip = { .horizontal = true },
        }) {
            const BoardId selectedBoard = frameData.ui.selectedBoard;
            const Library &library = frameData.state.library;

            Clay_String title = CLAY_STRING("All sounds");
            if (selectedBoard != allSoundsBoard) {
                auto it = std::ranges::find(library.boards, selectedBoard, &Board::id);
                title = it != library.boards.end() ? ToClay(it->name) : CLAY_STRING("Unknown board");
            }
            Clay_TextElementConfig titleText = frameData.fonts.text(theme::font::Face::display_semibold, 40, theme::text_primary);
            titleText.wrapMode = CLAY_TEXT_WRAP_NONE;
            CLAY_TEXT(title, titleText);

            const auto count = std::ranges::count_if(library.sounds, [&](const SoundData &s) { return inBoard(s, selectedBoard); });
            Clay_TextElementConfig subtitleText = frameData.fonts.text(theme::font::Face::sans_regular, 24, theme::text_secondary);
            subtitleText.wrapMode = CLAY_TEXT_WRAP_NONE;

            CLAY_AUTO_ID(Clay_ElementDeclaration{
                .layout = { .padding = { .bottom = 4 } },
            }) {
                CLAY_TEXT(frameData.text.keep(std::format("{} sound{}. Click a pad or press its key", count, count == 1 ? "" : "s")),
                    subtitleText);
            }
        }
    }

    struct Columns { int count; float cardWidth; };

    // Clay has no flex-wrap: split rows by hand from last frame's width.
    // Stable only because gridScroll clips: its width comes from the window, never from the cards
    Columns columnsFor(Clay_ElementId id) {
        using namespace theme::grid;

        Clay_ElementData data = Clay_GetElementData(id);
        const float inner = data.found ? data.boundingBox.width - 2 * padding : min_card_width;
        const int count = std::max(1, static_cast<int>((inner + gap) / (min_card_width + gap)));
        // whole pixels: fractional widths blur card edges and text
        const float width = std::floor((inner - gap * (count - 1)) / count);
        return { count, std::max(width, 0.0f) };
    }

    // stub cards
    void DropCard(FrameData &frameData, float width) {
        CLAY_AUTO_ID(Clay_ElementDeclaration{
            .layout = {
                .sizing = { .width = CLAY_SIZING_FIXED(width), .height = CLAY_SIZING_FIXED(theme::grid::card_height) },
                .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER },
            },
            .backgroundColor = theme::panel,
            .cornerRadius = CLAY_CORNER_RADIUS(8),
            .border = {
                .color = theme::strong_sep,
                .width = CLAY_BORDER_OUTSIDE(1),
            }
        }) {
            CLAY_TEXT(CLAY_STRING("+"), frameData.fonts.text(theme::font::Face::display_bold, 48, theme::text_secondary));
        }
    }

    void SoundCard(FrameData &frameData, const SoundData &sound, float width) {
        CLAY_AUTO_ID(Clay_ElementDeclaration{
            .layout = {
                .sizing = { .width = CLAY_SIZING_FIXED(width), .height = CLAY_SIZING_FIXED(theme::grid::card_height) },
                .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER },
            },
            .backgroundColor = theme::panel,
            .cornerRadius = CLAY_CORNER_RADIUS(8),
            .border = {
                .color = theme::strong_sep,
                .width = CLAY_BORDER_OUTSIDE(1),
            }
        }) {
            CLAY_TEXT(ToClay(sound.name), frameData.fonts.text(theme::font::Face::sans_medium, 20, theme::text_primary));
        }
    }
    
    void Cards(FrameData &frameData) {
        std::vector<const SoundData *> sounds;
        for (const SoundData &s : frameData.state.library.sounds) {
            if (inBoard(s, frameData.ui.selectedBoard)) sounds.push_back(&s);
        }

        const Columns cols = columnsFor(CLAY_ID("gridScroll"));
        const size_t cells = sounds.size() + 1;      // "add sound" card

        CLAY(CLAY_ID("gridScroll"), Clay_ElementDeclaration{
            .layout = {
                .sizing = theme::expand,
                // no top padding: the title above already has bottom padding
                .padding = { .left = theme::grid::padding, .right = theme::grid::padding, .bottom = theme::grid::padding },
                .childGap = theme::grid::gap,
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
            },
            // horizontal too: cards sized from last frame overflow for one frame while shrinking
            .clip = { .horizontal = true, .vertical = true, .childOffset = Clay_GetScrollOffset() },
        }) {
            for (size_t row = 0; row < cells; row += cols.count) {
                CLAY_AUTO_ID({ .layout = { .childGap = theme::grid::gap } }) {
                    for (size_t i = row; i < std::min(row + cols.count, cells); ++i) {
                        if (i < sounds.size())
                            SoundCard(frameData, *sounds[i], cols.cardWidth);
                        else
                            DropCard(frameData, cols.cardWidth);
                    }
                }
            }
            ScrollFadeTopBottom(CLAY_ID("gridScroll"), theme::background);
        }
    }
}

void SoundGrid(FrameData &frameData) {
    CLAY(CLAY_ID("soundGrid"), Clay_ElementDeclaration{
        .layout = {
            .sizing = theme::expand,
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
        .backgroundColor = theme::background,
    }) {
        BoardTitle(frameData);
        Cards(frameData);
    }
}
