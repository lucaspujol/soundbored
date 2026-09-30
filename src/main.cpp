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
        [&](action::PlaySound sound) { audioManager.playSound(sound.id); }
    }, action);
}

int main(void) {
    AudioManager& audioManager = AudioManager::getInstance();
    UI ui { 1280, 800, "SoundBored" };
    AppState appState = getStubAppState();

    displayAppState(appState);
    while (!ui.shouldClose()) {
        std::vector<Action> actions = ui.frame(appState);
        for (const Action& action : actions)
            apply(action, appState, audioManager);
    }
    return 0;
}
