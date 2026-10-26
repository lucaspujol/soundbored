#include "Config.hpp"

#include "cord.hpp"
#include "KeybindFormat.hpp"
#include "data/AppState.hpp"
#include "data/Types.hpp"
#include <cstddef>
#include <iostream>
#include <optional>

static cord::Schema& getSchema() {
    static cord::Schema schema = [] {
        cord::Schema s{};
        
        s.add<std::string>("physical_device_name").required();
        s.add<float>("physical_device_volume").min(0.0f).max(1.0f).required();
        s.add<std::string>("virtual_device_name");
        s.add<float>("virtual_device_volume").min(0.0f).max(1.0f);
        s.add<std::string>("retrigger_mode").oneOf({"Overlap", "Restart", "Stop"}).required();
        s.add<bool>("hotkeys_active").required();
        
        s.add<std::vector<int>>("sound_ids").required();
        s.add<std::vector<std::string>>("sound_names").required();
        s.add<std::vector<std::string>>("sound_paths").required();
        s.add<std::vector<int>>("sound_durations").required();
        s.add<std::vector<float>>("sound_volumes").required(); // needs 0..1 bounds check later
        s.add<std::vector<std::string>>("keybinds").required();
        s.add<std::vector<int>>("board_ids").required();
        s.add<std::vector<int>>("all_board_ids").required();
        s.add<std::vector<std::string>>("all_board_names").required();
        
        s.add<int>("was_playing_id").default_(-1);
        s.add<int>("was_playing_position_ms").default_(-1);
        
        return s;
    }();
    return schema;
}

/**
 * @brief True if any value is negative. The int arrays map to unsigned fields, a negative would wrap.
 */
static bool hasNegative(const std::vector<int>& values) {
    for (int value : values) {
        if (value < 0) return true;
    }
    return false;
}

/**
 * @brief Zips the parallel library arrays back into sounds and boards.
 * @return std::nullopt if the arrays don't line up or hold negative ids/durations.
 */
static std::optional<Library> loadLibrary(const cord::Result& result, bool verbose) {
    const auto sound_ids = result.get("sound_ids").as<std::vector<int>>();
    const auto sound_names = result.get("sound_names").as<std::vector<std::string>>();
    const auto sound_paths = result.get("sound_paths").as<std::vector<std::string>>();
    const auto sound_durations = result.get("sound_durations").as<std::vector<int>>();
    const auto sound_volumes = result.get("sound_volumes").as<std::vector<float>>();
    const auto keybinds = result.get("keybinds").as<std::vector<std::string>>();
    const auto board_ids = result.get("board_ids").as<std::vector<int>>();
    const auto all_board_ids = result.get("all_board_ids").as<std::vector<int>>();
    const auto all_board_names = result.get("all_board_names").as<std::vector<std::string>>();

    const size_t sound_count = sound_ids.size();
    if (sound_names.size() != sound_count || sound_paths.size() != sound_count
        || sound_durations.size() != sound_count || sound_volumes.size() != sound_count
        || keybinds.size() != sound_count || board_ids.size() != sound_count) {
        if (verbose) std::cerr << "[CONFIG] Error: sound arrays don't all have the same length" << std::endl;
        return std::nullopt;
    }
    if (all_board_names.size() != all_board_ids.size()) {
        if (verbose) std::cerr << "[CONFIG] Error: board arrays don't have the same length" << std::endl;
        return std::nullopt;
    }
    if (hasNegative(sound_ids) || hasNegative(sound_durations) || hasNegative(board_ids) || hasNegative(all_board_ids)) {
        if (verbose) std::cerr << "[CONFIG] Error: negative id or duration" << std::endl;
        return std::nullopt;
    }

    Library library{};
    for (size_t i = 0; i < sound_count; i++) {
        std::optional<Keybind> keybind = std::nullopt;
        if (!keybinds[i].empty()) {
            keybind = keybindFromString(keybinds[i]);
            if (!keybind && verbose) {
                std::cerr << "[CONFIG] Warning: invalid keybind '" << keybinds[i] << "' for sound '" << sound_names[i] << "', dropped" << std::endl;
            }
        }
        library.sounds.push_back({
            .id = static_cast<SoundId>(sound_ids[i]),
            .name = sound_names[i],
            .path = sound_paths[i],
            .durationMs = static_cast<uint64_t>(sound_durations[i]),
            .volume = sound_volumes[i],
            .keybind = keybind,
            .board = static_cast<BoardId>(board_ids[i]),
        });
    }
    for (size_t i = 0; i < all_board_ids.size(); i++) {
        library.boards.push_back({
            .id = static_cast<BoardId>(all_board_ids[i]),
            .name = all_board_names[i],
        });
    }
    return library;
}

std::optional<PersistentState> Config::load(bool verbose) {
    // TODO: remove once the AppState initialization stage validates the domain rules
    std::cerr << "[CONFIG] TODO: loaded state isn't validated yet: volumes in 0..1, unique sound/board ids, sounds pointing to existing boards, playback pointing to an existing sound" << std::endl;

    PersistentState state{};
    cord::Schema& schema = getSchema();
    auto result = schema.parseFile("soundbored.conf");
    if (result.hasErrors()) {
        if (verbose) result.printErrors();
        return std::nullopt;
    }

    state.settings.physicalOut.deviceName = result.get("physical_device_name").as<std::string>();
    state.settings.physicalOut.volume = result.get("physical_device_volume").as<float>();
    if (result.contains("virtual_device_name") && result.contains("virtual_device_volume")) {
        state.settings.virtualOut = {
            result.get("virtual_device_name").as<std::string>(),
            result.get("virtual_device_volume").as<float>()
        };
    } else {
        state.settings.virtualOut = std::nullopt;
    }
    state.settings.retrigger = stringToRetriggerMode(result.get("retrigger_mode").as<std::string>());
    state.settings.hotkeysActive = result.get("hotkeys_active").as<bool>();
    
    std::optional<Library> library = loadLibrary(result, verbose);
    if (!library) return std::nullopt;
    state.library = std::move(*library);

    if (result.get("was_playing_id").as<int>() != -1 && result.get("was_playing_position_ms").as<int>() != -1) {
        state.playback.currentlyPlaying = result.get("was_playing_id").as<int>();
        state.playback.positionMs       = result.get("was_playing_position_ms").as<int>();
    } else {
        state.playback.currentlyPlaying = std::nullopt;
        state.playback.positionMs = 0;
    }
    
    return state;
};

bool Config::save(const Library& lib, const Settings& set, const Playback& pbk) {
    // TODO: remove once both are fixed
    std::cerr << "[CONFIG] TODO: config path is relative to the cwd, not the executable dir" << std::endl;
    std::cerr << "[CONFIG] TODO: save isn't atomic, a crash mid-write corrupts the config (write to .tmp then rename)" << std::endl;

    cord::Result res{};

    res.set("physical_device_name", set.physicalOut.deviceName);
    res.set("physical_device_volume", set.physicalOut.volume);
    if (set.virtualOut) {
        res.set("virtual_device_name", set.virtualOut->deviceName);
        res.set("virtual_device_volume", set.virtualOut->volume);
    }
    res.set("retrigger_mode", retriggerModeToString(set.retrigger));
    res.set("hotkeys_active", set.hotkeysActive);

    std::vector<int> sound_ids{};
    std::vector<std::string> sound_names{};
    std::vector<std::string> sound_paths{};
    std::vector<int> sound_durations{};
    std::vector<float> sound_volumes{};
    std::vector<std::string> keybinds{};
    std::vector<int> board_ids{};
    for (const auto& sound : lib.sounds) {
        sound_ids.push_back(sound.id);
        sound_names.push_back(sound.name);
        sound_paths.push_back(sound.path);
        sound_durations.push_back(sound.durationMs);
        sound_volumes.push_back(sound.volume);
        if (sound.keybind) keybinds.push_back(keybindToString(sound.keybind.value()));
        else keybinds.push_back("");
        board_ids.push_back(sound.board);
    }
    std::vector<int> all_board_ids{};
    std::vector<std::string> all_board_names{};
    for (const auto& board : lib.boards) {
        all_board_ids.push_back(board.id);
        all_board_names.push_back(board.name);
    }

    res.set("sound_ids", sound_ids);
    res.set("sound_names", sound_names);
    res.set("sound_paths", sound_paths);
    res.set("sound_durations", sound_durations);
    res.set("sound_volumes", sound_volumes);
    res.set("keybinds", keybinds);
    res.set("board_ids", board_ids);
    res.set("all_board_ids", all_board_ids);
    res.set("all_board_names", all_board_names);

    if (pbk.currentlyPlaying) {
        res.set("was_playing_id", static_cast<int>(pbk.currentlyPlaying.value()));
        res.set("was_playing_position_ms", static_cast<int>(pbk.positionMs));
    }

    try {
        res.writeFile("soundbored.conf");
    } catch (const cord::CordException&) {
        return false; // maybe could retry
    }
    return true;
};