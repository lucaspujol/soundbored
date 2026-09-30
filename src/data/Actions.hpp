#pragma once

#include "data/Types.hpp"

#include <variant>

namespace action {

struct ImportFiles {};
struct StopAll {};
struct PlaySound { SoundId id; };

}

using Action = std::variant<action::ImportFiles, action::StopAll, action::PlaySound>;
