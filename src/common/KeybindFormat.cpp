#include "common/KeybindFormat.hpp"

#include "raylib.h"

#include <array>
#include <cctype>
#include <utility>

namespace {

    // Keys whose name isn't their own character. Letters and digits are handled separately,
    // their raylib code is their uppercase ASCII value.
    constexpr std::array<std::pair<int, std::string_view>, 59> NAMED_KEYS = {{
        { KEY_SPACE, "Space" },         { KEY_ENTER, "Enter" },         { KEY_TAB, "Tab" },
        { KEY_ESCAPE, "Escape" },       { KEY_BACKSPACE, "Backspace" }, { KEY_INSERT, "Insert" },
        { KEY_DELETE, "Delete" },       { KEY_HOME, "Home" },           { KEY_END, "End" },
        { KEY_PAGE_UP, "PageUp" },      { KEY_PAGE_DOWN, "PageDown" },  { KEY_UP, "Up" },
        { KEY_DOWN, "Down" },           { KEY_LEFT, "Left" },           { KEY_RIGHT, "Right" },
        { KEY_F1, "F1" },   { KEY_F2, "F2" },   { KEY_F3, "F3" },   { KEY_F4, "F4" },
        { KEY_F5, "F5" },   { KEY_F6, "F6" },   { KEY_F7, "F7" },   { KEY_F8, "F8" },
        { KEY_F9, "F9" },   { KEY_F10, "F10" }, { KEY_F11, "F11" }, { KEY_F12, "F12" },
        { KEY_KP_0, "Num0" }, { KEY_KP_1, "Num1" }, { KEY_KP_2, "Num2" }, { KEY_KP_3, "Num3" },
        { KEY_KP_4, "Num4" }, { KEY_KP_5, "Num5" }, { KEY_KP_6, "Num6" }, { KEY_KP_7, "Num7" },
        { KEY_KP_8, "Num8" }, { KEY_KP_9, "Num9" },
        { KEY_KP_DECIMAL, "NumDecimal" }, { KEY_KP_DIVIDE, "NumDivide" },
        { KEY_KP_MULTIPLY, "NumMultiply" }, { KEY_KP_SUBTRACT, "NumSubtract" },
        { KEY_KP_ADD, "NumAdd" }, { KEY_KP_ENTER, "NumEnter" },
        // Symbols spelled out: '+' is the separator, and words survive hand-editing better
        { KEY_APOSTROPHE, "Apostrophe" }, { KEY_COMMA, "Comma" },     { KEY_MINUS, "Minus" },
        { KEY_PERIOD, "Period" },         { KEY_SLASH, "Slash" },     { KEY_SEMICOLON, "Semicolon" },
        { KEY_EQUAL, "Equal" },           { KEY_LEFT_BRACKET, "LeftBracket" },
        { KEY_BACKSLASH, "Backslash" },   { KEY_RIGHT_BRACKET, "RightBracket" },
        { KEY_GRAVE, "Grave" },           { KEY_CAPS_LOCK, "CapsLock" }, { KEY_PRINT_SCREEN, "PrintScreen" },
        { KEY_PAUSE, "Pause" },           { KEY_SCROLL_LOCK, "ScrollLock" }, { KEY_NUM_LOCK, "NumLock" },
    }};

    constexpr std::array<std::pair<uint8_t, std::string_view>, 3> MOD_NAMES = {{
        { ModCtrl, "Ctrl" }, { ModAlt, "Alt" }, { ModShift, "Shift" },
    }};

    bool equalsIgnoreCase(std::string_view a, std::string_view b) {
        if (a.size() != b.size()) return false;
        for (size_t i = 0; i < a.size(); i++) {
            if (std::tolower(static_cast<unsigned char>(a[i])) != std::tolower(static_cast<unsigned char>(b[i]))) return false;
        }
        return true;
    }

    std::string_view trim(std::string_view text) {
        while (!text.empty() && std::isspace(static_cast<unsigned char>(text.front()))) text.remove_prefix(1);
        while (!text.empty() && std::isspace(static_cast<unsigned char>(text.back()))) text.remove_suffix(1);
        return text;
    }

    std::string keyName(int key) {
        if ((key >= KEY_A && key <= KEY_Z) || (key >= KEY_ZERO && key <= KEY_NINE)) return std::string(1, static_cast<char>(key));
        for (const auto &[code, name] : NAMED_KEYS) {
            if (code == key) return std::string(name);
        }
        return "";
    }

    std::optional<int> keyFromName(std::string_view name) {
        if (name.size() == 1) {
            const int c = std::toupper(static_cast<unsigned char>(name[0]));
            if ((c >= KEY_A && c <= KEY_Z) || (c >= KEY_ZERO && c <= KEY_NINE)) return c;
        }
        for (const auto &[code, candidate] : NAMED_KEYS) {
            if (equalsIgnoreCase(candidate, name)) return code;
        }
        return std::nullopt;
    }

    std::optional<uint8_t> modFromName(std::string_view name) {
        if (equalsIgnoreCase(name, "Control")) return ModCtrl;
        for (const auto &[mod, modName] : MOD_NAMES) {
            if (equalsIgnoreCase(modName, name)) return mod;
        }
        return std::nullopt;
    }

}

std::string keybindToString(const Keybind &keybind) {
    std::string key = keyName(keybind.key);
    if (key.empty()) return "";

    std::string text;
    for (const auto &[mod, name] : MOD_NAMES) {
        if (keybind.mods & mod) {
            text += name;
            text += '+';
        }
    }
    return text + key;
}

std::optional<Keybind> keybindFromString(std::string_view text) {
    uint8_t mods = ModNone;

    // Every token before the last '+' is a mod, the last one is the key
    size_t separator;
    while ((separator = text.find('+')) != std::string_view::npos) {
        std::optional<uint8_t> mod = modFromName(trim(text.substr(0, separator)));
        if (!mod) return std::nullopt;
        mods |= *mod;
        text.remove_prefix(separator + 1);
    }

    std::optional<int> key = keyFromName(trim(text));
    if (!key) return std::nullopt;
    return Keybind{ *key, mods };
}
