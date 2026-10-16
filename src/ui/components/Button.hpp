#pragma once

#include "clay.h"
#include "ui/ClayHelpers.hpp"
#include "ui/theme/Buttons.hpp"

// sizing defaults to fit-content; pass e.g. { .width = CLAY_SIZING_GROW(0) } to stretch
template <class Content>
bool Button(Clay_ElementId id, const theme::ButtonStyle &style, Content &&content, Clay_Sizing sizing = {}) {
    bool clicked = false;

    CLAY(id, Clay_ElementDeclaration{
        .layout = {
            .sizing = sizing,
            .padding = style.padding,
            .childGap = style.gap,
            .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
        },
        .backgroundColor = Clay_Hovered() ? style.backgroundHover : style.background,
        .cornerRadius = CLAY_CORNER_RADIUS(style.radius),
        .border = { .color = style.border, .width = CLAY_BORDER_OUTSIDE(1) },
    }) {
        clicked = Clicked();
        content();
    }
    return clicked;
}
