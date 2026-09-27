#define CLAY_IMPLEMENTATION
#include "clay.h"
#include "clay_renderer_raylib.h"

#include <cstdio>
#include <memory>

void HandleClayErrors(Clay_ErrorData errorData) {
  std::fprintf(stderr, "clay: %s\n", errorData.errorText.chars);
}

int main(void) {
  const int width = 800;
  const int height = 600;
  Clay_Raylib_Initialize(width, height, "SoundBored", FLAG_WINDOW_RESIZABLE);

  uint64_t memSize = Clay_MinMemorySize();
  auto memory = std::make_unique<char[]>(memSize);
  Clay_Arena arena = Clay_CreateArenaWithCapacityAndMemory(memSize, memory.get());

  Clay_Dimensions layoutDimensions {
      .width = width,
      .height = height
  };

  Clay_Initialize(arena, layoutDimensions, { HandleClayErrors });

  Font fonts[1];
  fonts[0] = LoadFontEx("/usr/share/fonts/truetype/JetBrainsMono-Regular.ttf", 48, 0, 400);
  SetTextureFilter(fonts[0].texture, TEXTURE_FILTER_BILINEAR);
  Clay_SetMeasureTextFunction(Raylib_MeasureText, fonts);

  while (!WindowShouldClose()) {
      Clay_SetLayoutDimensions({
          .width = (float)GetScreenWidth(),
          .height = (float)GetScreenHeight()
      });
      Clay_SetPointerState({ GetMousePosition().x, GetMousePosition().y }, IsMouseButtonDown(MOUSE_BUTTON_LEFT));
      
      Clay_BeginLayout();

      CLAY(CLAY_ID("root"), Clay_ElementDeclaration {
          .layout = {
              .sizing = {
                  .width  = CLAY_SIZING_GROW(0),
                  .height = CLAY_SIZING_GROW(0),
              },
              .padding = CLAY_PADDING_ALL(10),
              .childGap = 16,
              .layoutDirection = CLAY_LEFT_TO_RIGHT,
          },
          .backgroundColor = { 43, 41, 51, 255 },
      }) {}
    
      Clay_RenderCommandArray commandArray = Clay_EndLayout(GetFrameTime());
    
      // raylib rendering
      BeginDrawing();
      ClearBackground(BLUE);
      Clay_Raylib_Render(commandArray, fonts);
      EndDrawing();
  }

  Clay_Raylib_Close();
}
