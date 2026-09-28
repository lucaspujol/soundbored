#include "AudioManager.hpp"

AudioManager::AudioManager() {
    // TODO: Consider loading config/cache (using cord)
    refreshDeviceCache();
    ma_device_id defaultDeviceId = getDefaultDeviceId();

    physicalEngine = std::make_unique<AudioEngine>(&defaultDeviceId);
    physicalSoundBank = std::make_unique<SoundBank>(*physicalEngine);
}

std::vector<DeviceInfo> AudioManager::getAvailableDevices() {
    refreshDeviceCache();
    return cachedDevices;
}

bool AudioManager::setPhysicalDevice(std::string dName) {
    ma_device_id newDeviceId = {};
    if (!resolveDeviceName(dName, &newDeviceId)) return false;

    std::map<SoundId, std::string> soundMap = physicalSoundBank->getSoundMap();
    physicalEngine = std::make_unique<AudioEngine>(&newDeviceId);
    physicalSoundBank = std::make_unique<SoundBank>(*physicalEngine);
    for (const auto& pair : soundMap) {
        if (!physicalSoundBank->loadSound(pair.first, pair.second)) {
            return false;
        }
    }
    return true;
}

bool AudioManager::setVirtualDevice(std::string dName) {
    ma_device_id newDeviceId = {};
    if (!resolveDeviceName(dName, &newDeviceId)) return false;

    std::map<SoundId, std::string> soundMap = physicalSoundBank->getSoundMap();
    virtualEngine = std::make_unique<AudioEngine>(&newDeviceId);
    virtualSoundBank = std::make_unique<SoundBank>(*virtualEngine);
    for (const auto& pair : soundMap) {
        if (!virtualSoundBank->loadSound(pair.first, pair.second)) {
            return false;
        }
    }
    return true;
}

void AudioManager::clearVirtualDevice() {
    virtualSoundBank.reset();
    virtualEngine.reset();
}

int32_t AudioManager::importSound(std::string path) {
    if (physicalSoundBank && !physicalSoundBank->loadSound(nextSoundId, path)) {
        return -1;
    }

    if (virtualSoundBank && !virtualSoundBank->loadSound(nextSoundId, path)) {
        return -1;
    }

    return nextSoundId++;
}

void AudioManager::removeSound(SoundId id) {
    if (physicalSoundBank) physicalSoundBank->unloadSound(id);
    if (virtualSoundBank)  virtualSoundBank->unloadSound(id);
}

void AudioManager::playSound(SoundId id) {
    if (physicalSoundBank) physicalSoundBank->play(id);
    if (virtualSoundBank)  virtualSoundBank->play(id);
}

void AudioManager::stopSound(SoundId id) {
    if (physicalSoundBank) physicalSoundBank->stop(id);
    if (virtualSoundBank)  virtualSoundBank->stop(id);
}

void AudioManager::restartSound(SoundId id) {
    if (physicalSoundBank) physicalSoundBank->restart(id);
    if (virtualSoundBank)  virtualSoundBank->restart(id);
}

void AudioManager::refreshDeviceCache() {
    cachedDevices.clear();
    ma_device_info* deviceInfos;
    ma_uint32 deviceCount;
    ma_context_get_devices(context.get(), &deviceInfos, &deviceCount, nullptr, nullptr);

    for (ma_uint32 i = 0; i < deviceCount; ++i) {
        DeviceInfo info { 
            deviceInfos[i].name, 
            deviceInfos[i].id, 
            static_cast<bool>(deviceInfos[i].isDefault)
        };
        cachedDevices.push_back(info);
    }
}

ma_device_id AudioManager::getDefaultDeviceId() {
    ma_device_info* deviceInfos;
    ma_uint32 deviceCount;
    ma_context_get_devices(context.get(), &deviceInfos, &deviceCount, nullptr, nullptr);

    for (ma_uint32 i = 0; i < deviceCount; ++i) {
        if (deviceInfos[i].isDefault) {
            return deviceInfos[i].id;
        }
    }

    throw std::runtime_error("No default audio device found.");
}

bool AudioManager::resolveDeviceName(std::string dName, ma_device_id* outId) {
    for (const auto& device : cachedDevices) {
        if (device.name == dName) {
            *outId = device.id;
            return true;
        }
    }
    return false;
}