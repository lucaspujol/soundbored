#include "ui/sections/Sidebar.hpp"
#include "ui/ClayHelpers.hpp"
#include "ui/FontCache.hpp"
#include "ui/components/Button.hpp"
#include "ui/components/Icon.hpp"
#include "ui/theme/Colors.hpp"
#include "ui/theme/Layout.hpp"

namespace {
    void SectionLabel(FontCache &fonts, Clay_String text) {
        Clay_TextElementConfig label = fonts.text(theme::font::Face::sans_semibold, 20, theme::text_tertiary);
        label.letterSpacing = 2;
        CLAY_TEXT(text, label);
    }

    // what differs between the two output sections
    struct OutputLook {
        Clay_String name;
        Clay_String hint;
        theme::icon::Id icon;
        Clay_Color accent;
    };

    const OutputLook monitorLook { CLAY_STRING("Monitor"), CLAY_STRING("what you hear"),  theme::icon::headphones, theme::orange };
    const OutputLook micLook     { CLAY_STRING("Mic out"), CLAY_STRING("what they hear"), theme::icon::mic, theme::blue };

    void OutputSection(const OutputLook &look, const Output &output, FontCache &fonts, Icons &icons) {
        CLAY(CLAY_SID(look.name), Clay_ElementDeclaration{
            .layout = {
                .sizing = { CLAY_SIZING_GROW(0) },
                .childGap = 8,
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
            },
        }) {
            CLAY(CLAY_ID_LOCAL("infoRow"), Clay_ElementDeclaration{
                .layout = {
                    .sizing = { CLAY_SIZING_GROW(0) },
                    .childGap = 12,
                    .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
                    .layoutDirection = CLAY_LEFT_TO_RIGHT,
                },
            }) {
                Icon(icons.get(look.icon, 20), look.accent);
                CLAY_TEXT(look.name, fonts.text(theme::font::Face::display_semibold, 24, look.accent));
                Spacer();
                CLAY_TEXT(look.hint, fonts.text(theme::font::Face::sans_medium, 18, theme::text_tertiary));
            }

            Button(CLAY_ID_LOCAL("device"), theme::secondaryButton, [&] {
                CLAY_TEXT(ToClay(output.deviceName), fonts.text(theme::font::Face::sans_medium, 20, theme::text_primary));
                Spacer();
                Icon(icons.get(theme::icon::chevron_down), theme::text_secondary);
            }, { .width = CLAY_SIZING_GROW(0) });
            // TODO: volume slider (output.volume)
            
        }
    }

    void OutputBlock(FontCache &fonts, Icons &icons, const Settings &settings) {
        SectionLabel(fonts, CLAY_STRING("OUTPUT"));
        OutputSection(monitorLook, settings.physicalOut, fonts, icons);
        if (settings.virtualOut) {
            OutputSection(micLook, *settings.virtualOut, fonts, icons);
        }
    }

    void RetriggerBlock(FontCache &fonts, RetriggerMode mode) {
        SectionLabel(fonts, CLAY_STRING("ON RETRIGGER"));
        // TODO: segmented control Overlap / Restart / Stop, highlight `mode`
    }

    bool BoardRow(FontCache &fonts, const Board &board, bool selected) {
        bool clicked = false;

        CLAY(CLAY_IDI("board", board.id), Clay_ElementDeclaration{
            .layout = {
                .sizing = { CLAY_SIZING_GROW(0) },
                .padding = { .left = 12, .right = 12, .top = 10, .bottom = 10 },
                .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
            },
            .backgroundColor = selected ? theme::selected
                             : Clay_Hovered() ? theme::control
                             : theme::transparent,
            .cornerRadius = CLAY_CORNER_RADIUS(8),
        }) {
            clicked = Clicked();
            CLAY_TEXT(ToClay(board.name), fonts.text(theme::font::Face::sans_medium, 20, theme::text_primary));
            // TODO: spacer + sound count on the right (a formatted number needs per-frame string storage)
        }
        return clicked;
    }

    std::optional<BoardId> BoardList(FontCache &fonts, const std::vector<Board> &boards, BoardId selected) {
        SectionLabel(fonts, CLAY_STRING("BOARDS"));
        // TODO: "All sounds" row first. It is not a real Board, pick a reserved id (0)

        std::optional<BoardId> clicked;
        for (const Board &board : boards) {
            if (BoardRow(fonts, board, board.id == selected)) {
                clicked = board.id;
            }
        }
        return clicked;
    }
}

SidebarActions Sidebar(FontCache &fonts, Icons &icons, const AppState &state, BoardId selectedBoard) {
    SidebarActions actions;

    CLAY(CLAY_ID("sidebar"), Clay_ElementDeclaration{
        .layout = {
            .sizing = {
                CLAY_SIZING_FIXED(theme::sidebar_width_px),
                CLAY_SIZING_GROW(0),
            },
            .padding = CLAY_PADDING_ALL(24),
            .childGap = 24,
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
        .backgroundColor = theme::panel,
        .border = {
            .color = theme::strong_sep,
            .width = { .right = 1 },
        }
    }) {
        OutputBlock(fonts, icons, state.settings);
        RetriggerBlock(fonts, state.settings.retrigger);
        actions.boardClicked = BoardList(fonts, state.library.boards, selectedBoard);
        // TODO: spacer + "Global hotkeys active" status at the bottom
    }
    return actions;
}
