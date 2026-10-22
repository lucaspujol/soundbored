#pragma once

#include "data/Types.hpp"

#include <optional>
#include <string>
#include <string_view>

/**
 * @brief Formats a keybind as text, mods first in a fixed order: "Ctrl+Alt+Shift+A".
 * @return The formatted keybind, or an empty string if the key has no known name.
 */
std::string keybindToString(const Keybind &keybind);

/**
 * @brief Parses text produced by keybindToString(). Case-insensitive, spaces around '+' allowed.
 * @return The keybind, or std::nullopt if the text is empty, a mod is unknown or the key is unknown.
 */
std::optional<Keybind> keybindFromString(std::string_view text);
