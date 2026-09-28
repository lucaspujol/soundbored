#pragma once

class FontCache;
class Icons;

struct HeaderActions {
    bool importClicked  = false;
    bool stopAllClicked = false;
};

HeaderActions Header(FontCache &fonts, Icons &icons);