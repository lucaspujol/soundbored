#include "ui/UI.hpp"

int main(void) {
    UI ui { 800, 600, "SoundBored" };
    while (!ui.shouldClose()) {
        ui.frame();
    }
    return 0;
}
