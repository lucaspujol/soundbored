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

inline void Spacer() {
    CLAY_AUTO_ID({ .layout = { .sizing = { CLAY_SIZING_GROW(0) } } }) {}
}