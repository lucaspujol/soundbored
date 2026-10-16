#include "ui/sections/Header.hpp"
#include "ui/FrameData.hpp"
#include "ui/components/Button.hpp"
#include "ui/FontCache.hpp"
#include "ui/Icons.hpp"
#include "ui/ClayHelpers.hpp"
#include "ui/components/Icon.hpp"
#include "ui/theme/Layout.hpp"

namespace {
    void Logo(FrameData &f) {
        CLAY(CLAY_ID("logo"), Clay_ElementDeclaration{
            .layout = {
                .sizing = {
                    CLAY_SIZING_FIXED(theme::sidebar_width_px),
                    CLAY_SIZING_GROW(0),
                },
                .padding = { .left = 20, },
                .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
            },
        }) {
            CLAY_TEXT(CLAY_STRING("sound"), f.fonts.text(theme::font::Face::display_bold, 42, theme::text_primary));
            CLAY_TEXT(CLAY_STRING("bored"), f.fonts.text(theme::font::Face::display_bold, 42, theme::orange));
        }
    }

    void SearchBar(FrameData &f) {
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
        }) {
            // TODO
            CLAY_TEXT(
                CLAY_STRING("Search bar placeholder"),
                f.fonts.text(theme::font::Face::sans_regular, 20, theme::text_primary)
            );
        }
    }
}

void Header(FrameData &f) {
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
        Logo(f);
        SearchBar(f);
        Spacer();
        CLAY(CLAY_ID("headerButtons"), Clay_ElementDeclaration{
            .layout = {
                .sizing = { .height = CLAY_SIZING_GROW(0), },
                .padding = { .right = 20, },
                .childGap = 8,
                .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
            },
        }) {
            if (Button(CLAY_ID("importButton"), theme::secondaryButton, [&] {
                Icon(f.icons.get(theme::icon::upload), theme::secondaryButton.content);
                CLAY_TEXT(CLAY_STRING("Import"), f.fonts.text(theme::font::Face::sans_semibold, 24, theme::secondaryButton.content));
            })) {
                f.actions.push_back(action::ImportFiles{});
            }
            if (Button(CLAY_ID("stopAllButton"), theme::dangerButton, [&] {
                Icon(f.icons.get(theme::icon::stop), theme::dangerButton.content);
                CLAY_TEXT(CLAY_STRING("Stop all"), f.fonts.text(theme::font::Face::sans_semibold, 24, theme::dangerButton.content));
            })) {
                f.actions.push_back(action::StopAll{});
            }
        }
    }
}
