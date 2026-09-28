#include "ui/Button.hpp"
#include "ui/FontCache.hpp"
#include "ui/Theme.hpp"

namespace {
    void ButtonIcon(Texture2D *icon, Clay_Color color) {
        CLAY_AUTO_ID(Clay_ElementDeclaration{
            .layout = { .sizing = { CLAY_SIZING_FIXED(theme::icon_size_px), CLAY_SIZING_FIXED(theme::icon_size_px) } },
            .overlayColor = color,
            .image = { .imageData = icon },
        }) {}
    }
}

bool Button(FontCache &fonts, Clay_String label, Texture2D *icon, const theme::ButtonStyle &style) {
    bool clicked = false;

    CLAY(CLAY_SID(label), Clay_ElementDeclaration{
        .layout = {
            .padding = { .left = 16, .right = 16, .top = 12, .bottom = 12 },
            .childGap = 8,
            .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
        },
        .backgroundColor = Clay_Hovered() ? style.backgroundHover : style.background,
        .cornerRadius = CLAY_CORNER_RADIUS(12),
        .border = { .color = style.border, .width = CLAY_BORDER_OUTSIDE(1) },
    }) {
        clicked = Clay_Hovered() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

        if (icon) {
            ButtonIcon(icon, style.content);
        }
        CLAY_TEXT(label, fonts.text(theme::font::sans_semibold, 24, style.content));
    }
    return clicked;
}
