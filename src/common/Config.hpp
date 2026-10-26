#pragma once

#include "data/AppState.hpp"
#include <optional>


class Config {
public:
    // nullopt when bad / absent
    static std::optional<PersistentState> load(bool verbose = false);
    static bool save(const Library& lib, const Settings& set, const Playback& pbk);
};
