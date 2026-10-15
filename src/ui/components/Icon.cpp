#include "Icon.hpp"

void Icon(Texture2D *icon, Clay_Color color) {
    const float size = static_cast<float>(icon->width);

    CLAY_AUTO_ID(Clay_ElementDeclaration{
        .layout = { .sizing = { CLAY_SIZING_FIXED(size), CLAY_SIZING_FIXED(size) } },
        .overlayColor = color,
        .image = { .imageData = icon },
    }) {}
}
