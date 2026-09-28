#include "Config.hpp"
#include "cord.hpp"
#include <iostream>

cord::Schema Config::getSchema() {
    cord::Schema schema;

    schema.setAllowComments(true);
    schema.setStrict(false);
    schema.setCaseInsensitive(false);

    schema.add<std::string>("physicalDevice");
    schema.add<std::string>("virtualDevice");
    schema.add<std::vector<std::string>>("soundPaths");
    schema.add<int>("onRetrigger").oneOf({ 0, 1, 2 }); // 0=overlap, 1=restart, 2=stop

    return schema;
}

bool Config::load() {
    cord::Schema schema = getSchema();

    result = schema.parseFile(CONFIG_FILE_PATH);
    if (result.hasErrors()) {
        if (verbose) {
            std::cerr << "Error loading config file: " << CONFIG_FILE_PATH << std::endl;
            result.printErrors();
        }
        return false;
    }

    if (result.contains("physicalDevice")) physicalDevice = result.get("physicalDevice").as<std::string>();
    else if (verbose) std::cerr << "[CONFIG] Warning: 'physicalDevice' not found in config file." << std::endl;

    if (result.contains("virtualDevice")) virtualDevice = result.get("virtualDevice").as<std::string>();
    else if (verbose) std::cerr << "[CONFIG] Warning: 'virtualDevice' not found in config file." << std::endl;

    if (result.contains("soundPaths")) soundPaths = result.get("soundPaths").as<std::vector<std::string>>();
    else if (verbose) std::cerr << "[CONFIG] Warning: 'soundPaths' not found in config file." << std::endl;

    if (result.contains("onRetrigger")) onRetrigger = result.get("onRetrigger").as<int>();
    else if (verbose) std::cerr << "[CONFIG] Warning: 'onRetrigger' not found in config file." << std::endl;

    return true;
}

bool Config::save() {
    if (physicalDevice) result.set("physicalDevice", *physicalDevice);
    if (virtualDevice) result.set("virtualDevice", *virtualDevice);
    result.set("soundPaths", soundPaths); // write even if empty, not inherently an error
    if (onRetrigger) result.set("onRetrigger", *onRetrigger);

    try {
        result.writeFile(CONFIG_FILE_PATH);
    } catch (const std::exception& e) {
        if (verbose) std::cerr << "Error saving config file: " << CONFIG_FILE_PATH << std::endl;
        if (verbose) result.printErrors();
        return false;
    }
    
    return true;
}