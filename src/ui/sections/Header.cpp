#include "ui/sections/Header.hpp"
#include "ui/components/Button.hpp"
#include "ui/FontCache.hpp"
#include "ui/Icons.hpp"
#include "ui/Theme.hpp"

namespace {
    void Logo(FontCache &fonts) {
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
            CLAY_TEXT(CLAY_STRING("sound"), fonts.text(theme::font::Face::display_bold, 42, theme::text_primary));
            CLAY_TEXT(CLAY_STRING("bored"), fonts.text(theme::font::Face::display_bold, 42, theme::orange));
        }
    }

    void SearchBar(FontCache &fonts) {
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
                fonts.text(theme::font::Face::sans_regular, 20, theme::text_primary)
            );
        }
    }

    void Spacer() {
        CLAY(CLAY_ID("spacer"), { .layout = { .sizing = { CLAY_SIZING_GROW(0) } } }) {}
    }
}

HeaderActions Header(FontCache &fonts, Icons &icons) {
    HeaderActions actions;
    
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
        Logo(fonts);
        SearchBar(fonts);
        Spacer();
        CLAY(CLAY_ID("headerButtons"), Clay_ElementDeclaration{
            .layout = {
                .sizing = { .height = CLAY_SIZING_GROW(0), },
                .padding = { .right = 20, },
                .childGap = 8,
                .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
            },
        }) {
            actions.importClicked  = Button(fonts, CLAY_STRING("Import"),   icons.get(theme::icon::upload), theme::secondaryButton);
            actions.stopAllClicked = Button(fonts, CLAY_STRING("Stop all"), icons.get(theme::icon::stop),   theme::dangerButton);
        }
    }
    return actions;
}