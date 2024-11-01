#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include <vector>

#include "frontend/window.hpp"
#include "services/ui_service.hpp"

namespace Remove_record {
    void render() {
        SDL_Rect backButtonRect = {
            UI::get_padding(), 
            UI::get_padding(), 
            100,  // Adjust width to fit "Back" text
            40    // Standard button height
        };
        Types::Button backButton = { 
            backButtonRect, 
            "Back", 
            false,   // not disabled 
            false    // not selected
        };
        UI::render_button(backButton);
    }
}