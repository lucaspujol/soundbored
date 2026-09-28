#pragma once

#include "clay.h"
#include "raylib.h"
#include "ui/Theme.hpp"

class FontCache;

bool Button(FontCache &fonts, Clay_String label, Texture2D *icon, const theme::ButtonStyle &style);
