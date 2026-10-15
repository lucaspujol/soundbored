#pragma once

#include "clay.h"
#include "ui/theme/Segmented.hpp"

#include <cstddef>
#include <optional>
#include <span>

// Returns the index clicked this frame.
// text sets font and size; its color is replaced by the style's text colors
std::optional<size_t> SegmentedControl(Clay_ElementId id, std::span<const Clay_String> labels, size_t selected,
                                       Clay_TextElementConfig text, const theme::SegmentedStyle &style,
                                       Clay_Sizing sizing = {});
