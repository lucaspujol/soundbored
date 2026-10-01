#include "ui/components/SegmentedControl.hpp"
#include "ui/ClayHelpers.hpp"

std::optional<size_t> SegmentedControl(Clay_ElementId id, std::span<const Clay_String> labels, size_t selected,
                                       Clay_TextElementConfig text, const theme::SegmentedStyle &style,
                                       Clay_Sizing sizing) {
    std::optional<size_t> clicked;

    CLAY(id, Clay_ElementDeclaration{
        .layout = {
            .sizing = sizing,
            .padding = CLAY_PADDING_ALL(style.padding),
            .childGap = style.padding,
        },
        .backgroundColor = style.background,
        .cornerRadius = CLAY_CORNER_RADIUS(style.radius),
        .border = { .color = style.border, .width = CLAY_BORDER_OUTSIDE(1) },
    }) {
        for (size_t i = 0; i < labels.size(); ++i) {
            const bool isSelected = i == selected;

            CLAY(CLAY_IDI_LOCAL("segment", static_cast<uint32_t>(i)), Clay_ElementDeclaration{
                .layout = {
                    .sizing = { CLAY_SIZING_GROW(0) },
                    .padding = { .top = style.segmentPaddingY, .bottom = style.segmentPaddingY },
                    .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER },
                },
                .backgroundColor = isSelected ? style.segmentSelected
                                 : Clay_Hovered() ? style.segmentHover
                                 : theme::transparent,
                .cornerRadius = CLAY_CORNER_RADIUS(style.radius - style.padding),
            }) {
                if (Clicked() && !isSelected) {
                    clicked = i;
                }
                text.textColor = isSelected ? style.textSelected : style.text;
                CLAY_TEXT(labels[i], text);
            }
        }
    }
    return clicked;
}
