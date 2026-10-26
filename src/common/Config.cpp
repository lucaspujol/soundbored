#include "Config.hpp"

#include "cord.hpp"
#include "KeybindFormat.hpp"
#include "data/AppState.hpp"
#include "data/Types.hpp"
#include <iostream>

static cord::Schema& getSchema() {
    static cord::Schema schema = [] {
        cord::Schema s{};
        
        s.add<std::string>("physical_device_name");
        s.add<float>("physical_device_volume").min(0.0f).max(1.0f);
        s.add<std::string>("virtual_device_name");
        s.add<float>("virtual_device_volume").min(0.0f).max(1.0f);
        s.add<std::string>("retrigger_mode").oneOf({"Overlap", "Restart", "Stop"});
        s.add<bool>("hotkeys_active");
        
        s.add<std::vector<int>>("sound_ids");
        s.add<std::vector<std::string>>("sound_names");
        s.add<std::vector<std::string>>("sound_paths");
        s.add<std::vector<int>>("sound_durations");
        s.add<std::vector<float>>("sound_volume"); // needs 0..1 bounds check later
        s.add<std::vector<std::string>>("keybinds");
        s.add<std::vector<int>>("board_ids");
        s.add<std::vector<int>>("all_board_ids");
        s.add<std::vector<std::string>>("all_board_names");
        
        s.add<int>("was_playing_id");
        s.add<int>("was_playing_position_ms");
        
        return s;
    }();
    return schema; 
}

std::optional<PersistentState> Config::load(bool verbose) {
    // verify Library vectors all same length
    return std::nullopt;
};

bool Config::save(const Library& lib, const Settings& set, const Playback& pbk) {
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
    for (auto sound : lib.sounds) {
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
    for (auto board : lib.boards) {
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
    } catch (cord::CordException) {
        return false; // maybe could retry
    }
    return true;
};