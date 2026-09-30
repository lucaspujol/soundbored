#pragma once

#include "clay.h"
#include "ui/theme/Sliders.hpp"

#include <optional>

// value in [0, 1]. Returns the new value only on frames where it changed.
// Pass a width in sizing: a slider has no content to fit
std::optional<float> Slider(Clay_ElementId id, float value, const theme::SliderStyle &style, Clay_Sizing sizing = {});
