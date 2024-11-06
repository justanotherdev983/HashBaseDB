#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include <vector>

#include "frontend/window.hpp"
#include "services/ui_service.hpp"

namespace Remove_record {
    void render() {
        int cardWidth = UI::get_window_width() - (UI::get_padding() * 2);
        int cardHeight = UI::get_window_height() - (UI::get_padding() * 2);

        // Render background card
        UI::render_card(UI::get_padding(), UI::get_padding(), cardWidth, cardHeight);

        // Title text for "Remove Record" screen
        SDL_Color titleColor = UI::get_text_light_color();
        SDL_Color titleTypesColor = { titleColor.r, titleColor.g, titleColor.b, titleColor.a };
        UI::render_text("Remove Record", 
                        UI::get_window_width() / 2 - 100, 
                        UI::get_padding() * 2, 
                        titleTypesColor, UI::get_title_font());

        // Unique identifier input
        int UUIDY = UI::get_padding() * 4;
        UI::render_text("Enter the unique identifer for the entry you want to remove: ", 
                        UI::get_padding(), UUIDY, 
                        titleTypesColor, UI::get_font());
        UUIDY += 30;

        SDL_Rect UUIDRect = {
            UI::get_padding() * 2, UUIDY, 
            cardWidth - UI::get_padding() * 4, 40
        };

        // Render input box for UUID
        UI::render_input_box(UUIDRect.x, UUIDRect.y, UUIDRect.w, UUIDRect.h, 
                           UI::get_current_UUID(), UI::is_uuid_input_active());

        // Search button
        SDL_Rect searchButtonRect = {
            UUIDRect.x + UUIDRect.w - UI::get_button_width(),
            UUIDRect.y + UUIDRect.h + UI::get_padding(),
            UI::get_button_width(), 
            UI::get_button_height()
        };
        Types::Button searchButton = { searchButtonRect, "Delete", false, false };
        UI::render_button(searchButton);

        // Render back button
        SDL_Rect backButtonRect = {
            UI::get_padding(), 
            UI::get_padding(), 
            100,  
            40    
        };
        Types::Button backButton = { backButtonRect, "Back", false, false };
        UI::render_button(backButton);
    }
}