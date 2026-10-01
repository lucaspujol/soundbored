#pragma once

#include "data/Types.hpp"

#include <variant>

namespace action {

    struct ImportFiles {};
    struct StopAll {};
    struct PlaySound { SoundId id; };
    struct SetOutputVolume { OutputSlot output; float volume; };
    struct SetRetrigger { RetriggerMode mode; };
    struct ToggleHotkeys {};

}

using Action = std::variant<
    action::ImportFiles,
    action::StopAll,
    action::PlaySound,
    action::SetOutputVolume,
    action::SetRetrigger,
    action::ToggleHotkeys
>;
