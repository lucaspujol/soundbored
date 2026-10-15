#pragma once

#include <deque>
#include <string>
#include <utility>

#include "clay.h"
#include "ui/ClayHelpers.hpp"

// owns strings built during a frame (counts, "80%", "0:03") until Clay has rendered them.
// deque, not vector: push_back never moves existing strings, so earlier pointers stay valid
class TextArena {
public:
    Clay_String keep(std::string s) {
        return ToClay(strings.emplace_back(std::move(s)));
    }

    // call at the start of a frame, never before render
    void clear() { strings.clear(); }

private:
    std::deque<std::string> strings;
};
