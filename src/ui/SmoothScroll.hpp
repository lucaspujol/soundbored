#pragma once

#include <algorithm>
#include <cmath>

#include "clay.h"

// eases a Clay scroll container toward a wheel-driven target, instead of Clay's instant jump.
struct SmoothScroll {
    float target = 0;

    void update(Clay_ElementId id, float wheel, float dt) {
        Clay_ScrollContainerData d = Clay_GetScrollContainerData(id);
        if (!d.found) return;
        if (wheel != 0 && Clay_PointerOver(id)) {
            target += wheel * step;
        }
        // also catches content getting shorter, e.g. on window resize
        const float maxScroll = std::max(d.contentDimensions.height - d.scrollContainerDimensions.height, 0.0f);
        target = std::clamp(target, -maxScroll, 0.0f);

        float &pos = d.scrollPosition->y;
        pos += (target - pos) * (1 - std::exp(-speed * dt));
        // snap the tail, so text doesn't sit on a fractional offset
        if (std::abs(target - pos) < 0.5f) {
            pos = target;
        }
    }

private:
    static constexpr float step  = 60;   // px per wheel notch
    static constexpr float speed = 15;  // higher = snappier
};
