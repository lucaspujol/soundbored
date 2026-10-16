#pragma once

#include <cstdint>
#include <string>

#include "clay.h"
#include "raylib.h"

// call inside a CLAY element: true on the frame the element gets clicked
inline bool Clicked() {
    return Clay_Hovered() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

// Clay keeps the pointer and reads it at render time, so s must outlive the frame
inline Clay_String ToClay(const std::string &s) {
    return { .isStaticallyAllocated = false, .length = static_cast<int32_t>(s.size()), .chars = s.c_str() };
}

// grows on both axes, so it pushes siblings apart in rows and columns
inline void Spacer() {
    CLAY_AUTO_ID({ .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) } } }) {}
}

enum class FadeEdge { Top, Bottom };

// call inside the scroll container: fades its edge into color while content is hidden past it.
// Floating on the container's own box, which stays put when its content scrolls
inline void ScrollFade(Clay_ElementId scrollId, FadeEdge edge, Clay_Color color) {
    Clay_ScrollContainerData d = Clay_GetScrollContainerData(scrollId);
    if (!d.found) return;

    const float hiddenAbove = -d.scrollPosition->y;
    const float hiddenBelow = d.contentDimensions.height - d.scrollContainerDimensions.height - hiddenAbove;

    // defaults are for the bottom edge
    float hidden = hiddenBelow;
    Clay_FloatingAttachPointType point = CLAY_ATTACH_POINT_LEFT_BOTTOM;
    if (edge == FadeEdge::Top) {
        hidden = hiddenAbove;
        point = CLAY_ATTACH_POINT_LEFT_TOP;
    }
    if (hidden <= 1) return;

    constexpr int strips = 12;

    CLAY(CLAY_IDI_LOCAL("scrollFade", static_cast<uint32_t>(edge)), Clay_ElementDeclaration{
        .layout = {
            .sizing = { .width = CLAY_SIZING_GROW(0) },
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
        .floating = {
            .attachPoints = { .element = point, .parent = point },
            .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
            .attachTo = CLAY_ATTACH_TO_PARENT,
        }
    }) {
        for (int i = 1; i <= strips; ++i) {
            // opaque at the container's edge, transparent toward the content
            int step = i;
            if (edge == FadeEdge::Top) step = strips + 1 - i;
            Clay_Color c = color;
            c.a = color.a * step / strips;
            CLAY_AUTO_ID(Clay_ElementDeclaration{
                .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(4) } },
                .backgroundColor = c,
            }) {}
        }
    }
}

// sugar for both edges
inline void ScrollFadeTopBottom(Clay_ElementId scrollId, Clay_Color color) {
    ScrollFade(scrollId, FadeEdge::Top, color);
    ScrollFade(scrollId, FadeEdge::Bottom, color);
}
