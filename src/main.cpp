#include "audio/AudioManager.hpp"
#include "data/Actions.hpp"
#include "data/AppState.hpp"
#include "StubState.hpp"
#include "ui/UI.hpp"
#include "utils/overloaded.hpp"
#include <iostream>

void apply(Action action, AppState& appState, AudioManager& audioManager) {
    std::visit(overloaded {
        [&](action::ImportFiles) { std::cout <<  "import files action" << std::endl; },
        [&](action::StopAll) { std::cout <<  "stop all action" << std::endl; },
        [&](action::PlaySound sound) { audioManager.playSound(sound.id); },
        [&](action::SetOutputVolume volume) {
            if (volume.output == OutputSlot::Monitor) {
                appState.settings.physicalOut.volume = volume.volume;
                // audioManager.setPhysicalDevice(appState.settings.physicalOut.deviceName);
            } else if (volume.output == OutputSlot::Mic) {
                if (appState.settings.virtualOut) {
                    appState.settings.virtualOut->volume = volume.volume;
                    // audioManager.setVirtualDevice(appState.settings.virtualOut->deviceName);
                }
            }
        },
        [&](action::SetRetrigger retrigger) { appState.settings.retrigger = retrigger.mode; },
        [&](action::ToggleHotkeys) { appState.settings.hotkeysActive = !appState.settings.hotkeysActive; }
    }, action);
}

int main(void) {
    AudioManager& audioManager = AudioManager::getInstance();
    UI ui { 1920, 1080, "SoundBored" };
    AppState appState = getStubAppState();

    displayAppState(appState);
    while (!ui.shouldClose()) {
        std::vector<Action> actions = ui.frame(appState);
        for (const Action& action : actions)
            apply(action, appState, audioManager);
    }
    return 0;
}
