#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include <vector>

#include "frontend/window.hpp"
#include "services/ui_service.hpp"

namespace Add_record {
    void render() {
        int cardWidth = UI::get_window_width() - (UI::get_padding() * 2);
        int cardHeight = UI::get_window_height() - (UI::get_padding() * 2);

        // Render background card
        UI::render_card(UI::get_padding(), UI::get_padding(), cardWidth, cardHeight);

        // Render back button
        Types::Button backButton = { /* Initialize with appropriate values */ };
        UI::render_button(backButton);

        // Title text for "Add New Record" screen
        SDL_Color titleColor = UI::get_text_light_color();
        SDL_Color titleTypesColor = { titleColor.r, titleColor.g, titleColor.b, titleColor.a };
        UI::render_text("Add New Record", 
                        UI::get_window_width() / 2 - 100, 
                        UI::get_padding() * 2, 
                        titleTypesColor, UI::get_title_font());

        // Unique identifier input
        int UUIDY = UI::get_padding() * 4;
        UI::render_text("Enter a unique identifier for this entry:", 
                        UI::get_padding(), UUIDY, 
                        titleTypesColor, UI::get_font());
        UUIDY += 30;

        SDL_Rect UUIDRect = {
            UI::get_padding() * 2, UUIDY, 
            cardWidth - UI::get_padding() * 4, 40
        };

        bool isUUIDActive = true;// TODO: add logic for this
        UI::render_input_box(UUIDRect.x, UUIDRect.y, UUIDRect.w, UUIDRect.h, UI::get_new_record_value(), isUUIDActive);

        // Record input field
        int inputY = UI::get_padding() * 4 + 150;
        UI::render_text("Enter a value:", UI::get_padding(), inputY, 
                        titleTypesColor, UI::get_font());
        inputY += 30;

        SDL_Rect inputRect = {
            UI::get_padding() * 2, inputY, 
            cardWidth - UI::get_padding() * 4 - UI::get_button_width() - UI::get_padding(), 40
        };

        bool isInputActive = true; // TODO: add logic for this
        UI::render_input_box(inputRect.x, inputRect.y, inputRect.w, inputRect.h, UI::get_new_record_value(), isInputActive);

        // "New" button next to the input field
        SDL_Rect newButtonRect = {
            inputRect.x + inputRect.w + UI::get_padding(), inputY,
            UI::get_button_width(), UI::get_button_height()
        };
        Types::Button newButton = { newButtonRect, "New", false, false };
        UI::render_button(newButton);

        // "Add" button centered below
        SDL_Rect addButtonRect = {
            UI::get_window_width() / 2 - UI::get_button_width() / 2, 
            inputY + 50, 
            UI::get_button_width(), UI::get_button_height()
        };
        Types::Button addButton = { addButtonRect, "Add", false, false };
        UI::render_button(addButton);
    }
}
