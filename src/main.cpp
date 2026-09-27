#include "ui/UI.hpp"

int main(void) {
    UI ui { 1280, 800, "SoundBored" };
    while (!ui.shouldClose()) {
        ui.frame();
    }
    return 0;
}
