#include "ui/components/Slider.hpp"

#include <algorithm>
#include <cstdint>

#include "raylib.h"

namespace {
    // id of the slider being dragged, 0 = none. One mouse, so one drag at a time
    uint32_t activeId = 0;
}

std::optional<float> Slider(Clay_ElementId id, float value, const theme::SliderStyle &style, Clay_Sizing sizing) {
    if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        activeId = 0;
    }
    if (Clay_PointerOver(id) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        activeId = id.id;
    }

    // box is last frame's layout; not found on the very first frame
    std::optional<float> changed;
    Clay_ElementData data = Clay_GetElementData(id);
    if (activeId == id.id && data.found && data.boundingBox.width > 0) {
        const float x = GetMousePosition().x - data.boundingBox.x;
        const float dragged = std::clamp(x / data.boundingBox.width, 0.0f, 1.0f);
        if (dragged != value) {
            changed = dragged;
            value = dragged;    // draw the new value this frame, not next
        }
    }

    // vertical padding centers the track and makes the clickable area thumbSize tall
    const uint16_t padY = (style.thumbSize - style.trackHeight) / 2;

    CLAY(id, Clay_ElementDeclaration{
        .layout = {
            .sizing = sizing,
            .padding = { .top = padY, .bottom = padY },
        },
    }) {
        CLAY_AUTO_ID(Clay_ElementDeclaration{
            .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(style.trackHeight) } },
            .backgroundColor = style.track,
            .cornerRadius = CLAY_CORNER_RADIUS(style.trackHeight / 2.0f),
        }) {
            CLAY_AUTO_ID(Clay_ElementDeclaration{
                .layout = { .sizing = { CLAY_SIZING_PERCENT(value), CLAY_SIZING_GROW(0) } },
                .backgroundColor = style.fill,
                .cornerRadius = CLAY_CORNER_RADIUS(style.trackHeight / 2.0f),
            }) {
                CLAY_AUTO_ID(Clay_ElementDeclaration{
                    .layout = { .sizing = { CLAY_SIZING_FIXED(style.thumbSize), CLAY_SIZING_FIXED(style.thumbSize) } },
                    .backgroundColor = style.thumb,
                    .cornerRadius = CLAY_CORNER_RADIUS(style.thumbSize / 2.0f),
                    .floating = {
                        .zIndex = 1,
                        .attachPoints = { .element = CLAY_ATTACH_POINT_CENTER_CENTER, .parent = CLAY_ATTACH_POINT_RIGHT_CENTER },
                        .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
                        .attachTo = CLAY_ATTACH_TO_PARENT,
                    },
                }) {}
            }
        }
    }
    return changed;
}
