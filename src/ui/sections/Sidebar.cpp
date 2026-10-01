#include "ui/sections/Sidebar.hpp"
#include "data/Types.hpp"
#include "ui/ClayHelpers.hpp"
#include "ui/FontCache.hpp"
#include "ui/FrameData.hpp"
#include "ui/Icons.hpp"
#include "ui/TextArena.hpp"
#include "ui/components/Button.hpp"
#include "ui/components/Icon.hpp"
#include "ui/components/SegmentedControl.hpp"
#include "ui/components/Slider.hpp"
#include "ui/theme/Colors.hpp"
#include "ui/theme/Layout.hpp"

#include <algorithm>
#include <cmath>
#include <array>

namespace {
    void SectionLabel(FrameData &frameData, Clay_String text) {
        Clay_TextElementConfig label = frameData.fonts.text(theme::font::Face::sans_semibold, 20, theme::text_tertiary);
        label.letterSpacing = 2;
        CLAY_TEXT(text, label);
    }

    // label + content stacked tight. The sidebar's childGap spaces the blocks apart
    template <class Content>
    void Block(FrameData &frameData, Clay_ElementId id, Clay_String label, Content &&content) {
        CLAY(id, Clay_ElementDeclaration{
            .layout = {
                .sizing = { CLAY_SIZING_GROW(0) },
                .childGap = 8,
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
            },
        }) {
            SectionLabel(frameData, label);
            content();
        }
    }

    Clay_TextElementConfig numberText(FrameData &frameData) {
        return frameData.fonts.text(theme::font::Face::mono_medium, 18, theme::text_tertiary);
    }

    struct OutputLook {
        OutputSlot output;
        Clay_String name;
        Clay_String hint;
        theme::icon::Id icon;
        Clay_Color accent;
    };


    const OutputLook monitorLook { OutputSlot::Monitor, CLAY_STRING("Monitor"), CLAY_STRING("what you hear"),  theme::icon::headphones, theme::orange };
    const OutputLook micLook     { OutputSlot::Mic,     CLAY_STRING("Mic out"), CLAY_STRING("what they hear"), theme::icon::mic, theme::blue };

    // output stays a parameter: this section is drawn twice, once per output
    void OutputSection(FrameData &frameData, const OutputLook &look, const Output &output) {
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
                Icon(frameData.icons.get(look.icon, 20), look.accent);
                CLAY_TEXT(look.name, frameData.fonts.text(theme::font::Face::display_semibold, 24, look.accent));
                Spacer();
                CLAY_TEXT(look.hint, frameData.fonts.text(theme::font::Face::sans_medium, 18, theme::text_tertiary));
            }

            Button(CLAY_ID_LOCAL("device"), theme::secondaryButton, [&] {
                CLAY_TEXT(ToClay(output.deviceName), frameData.fonts.text(theme::font::Face::sans_medium, 20, theme::text_primary));
                Spacer();
                Icon(frameData.icons.get(theme::icon::chevron_down), theme::text_secondary);
            }, { .width = CLAY_SIZING_GROW(0) });

            CLAY(CLAY_ID_LOCAL("volumeRow"), Clay_ElementDeclaration{
                .layout = {
                    .sizing = { CLAY_SIZING_GROW(0) },
                    .padding = { .top = 6, .bottom = 6 },
                    .childGap = 12,
                    .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
                    .layoutDirection = CLAY_LEFT_TO_RIGHT,
                },
            }) {
                Clay_TextElementConfig hintText = frameData.fonts.text(theme::font::Face::sans_medium, 18, theme::text_tertiary);
                CLAY_TEXT(CLAY_STRING("Vol"), hintText);
                theme::SliderStyle s = theme::slider;
                s.fill = look.accent;
                if (auto v = Slider(CLAY_ID_LOCAL("volume"), output.volume, s, { .width = CLAY_SIZING_GROW(0) })) {
                    frameData.actions.push_back(action::SetOutputVolume{ .output = look.output, .volume = *v });
                }
                const long percent = std::lround(output.volume * 100);
                CLAY_TEXT(frameData.text.keep(std::to_string(percent) + "%"), numberText(frameData));
            }
        }
    }

    void OutputBlock(FrameData &frameData) {
        const Settings &settings = frameData.state.settings;
        Block(frameData, CLAY_ID("outputBlock"), CLAY_STRING("OUTPUT"), [&] {
            OutputSection(frameData, monitorLook, settings.physicalOut);
            if (settings.virtualOut) {
                OutputSection(frameData, micLook, *settings.virtualOut);
            }
        });
    }

    void RetriggerBlock(FrameData &frameData) {
        // same order as RetriggerMode, so index <-> enum is a cast
        static constexpr std::array labels {
            CLAY_STRING("Overlap"), CLAY_STRING("Restart"), CLAY_STRING("Stop"),
        };
        const auto current = static_cast<size_t>(frameData.state.settings.retrigger);
        const Clay_TextElementConfig text = frameData.fonts.text(theme::font::Face::sans_medium, 20, theme::text_primary);

        Block(frameData, CLAY_ID("retriggerBlock"), CLAY_STRING("ON RETRIGGER"), [&] {
            auto index = SegmentedControl(
                CLAY_ID("retrigger"), labels, current,
                text, theme::segmented, { .width = CLAY_SIZING_GROW(0) });

            if (index) {
                frameData.actions.push_back(action::SetRetrigger{ static_cast<RetriggerMode>(*index) });
            }
        });
    }

    // sets frameData.ui.selectedBoard when clicked
    void BoardRow(FrameData &frameData, BoardId id, Clay_String name, size_t soundCount) {
        BoardId &selectedBoard = frameData.ui.selectedBoard;
        const bool selected = id == selectedBoard;

        CLAY(CLAY_IDI("board", id), Clay_ElementDeclaration{
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
            if (Clicked()) {
                selectedBoard = id;
            }
            CLAY_TEXT(name, frameData.fonts.text(theme::font::Face::sans_medium, 20, theme::text_primary));
            Spacer();
            CLAY_TEXT(frameData.text.keep(std::to_string(soundCount)), numberText(frameData));
        }
    }

    void BoardList(FrameData &frameData) {
        const Library &library = frameData.state.library;

        Block(frameData, CLAY_ID("boardsBlock"), CLAY_STRING("BOARDS"), [&] {
            BoardRow(frameData, allSoundsBoard, CLAY_STRING("All sounds"), library.sounds.size());
            for (const Board &board : library.boards) {
                const auto count = std::ranges::count(library.sounds, board.id, &SoundData::board);
                BoardRow(frameData, board.id, ToClay(board.name), static_cast<size_t>(count));
            }
        });
    }

    // TODO: real state once platform/ global hotkeys exist
    void HotkeyStatus(FrameData &frameData) {
        CLAY(CLAY_ID("hotkeyStatus"), Clay_ElementDeclaration{
            .layout = {
                .childGap = 10,
                .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
            },
        }) {
            CLAY_AUTO_ID(Clay_ElementDeclaration{
                .layout = { .sizing = { CLAY_SIZING_FIXED(8), CLAY_SIZING_FIXED(8) } },
                .backgroundColor = frameData.state.settings.hotkeysActive ? theme::green : theme::red,
                .cornerRadius = CLAY_CORNER_RADIUS(4),
            }) {}
            CLAY_TEXT(frameData.state.settings.hotkeysActive ? CLAY_STRING("Global hotkeys active") : CLAY_STRING("Global hotkeys inactive"),
                frameData.fonts.text(theme::font::Face::sans_medium, 18, theme::text_tertiary));
        }
    }
}

void Sidebar(FrameData &frameData) {
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
        OutputBlock(frameData);
        RetriggerBlock(frameData);
        BoardList(frameData);
        Spacer();
        HotkeyStatus(frameData);
    }
}
