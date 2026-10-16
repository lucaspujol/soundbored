#include "ui/sections/SoundGrid.hpp"
#include "ui/ClayHelpers.hpp"
#include "ui/FontCache.hpp"
#include "ui/FrameData.hpp"
#include "ui/TextArena.hpp"
#include "ui/theme/Layout.hpp"
#include "ui/theme/Colors.hpp"

#include <format>
#include <algorithm>

namespace {
    void BoardTitle(FrameData &frameData) {
        CLAY(CLAY_ID("boardTitle"), Clay_ElementDeclaration{
            .layout = {
                .sizing = { .width = CLAY_SIZING_GROW(0) },
                .padding = { .left = 24, .right = 24, .top = 24, .bottom = 12 },
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

    }
}
