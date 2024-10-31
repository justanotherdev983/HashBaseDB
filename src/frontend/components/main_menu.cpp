#include "frontend/window.hpp"
#include "services/ui_service.hpp"

namespace Main_menu {
    void render() {

        // Render main menu buttons
        for (const auto& button : UI::main_menu_buttons) {
            UI::render_button(button);
        }

        // Set up colors using UI functions
        SDL_Color sdlTitleColor = UI::get_text_light_color();
        SDL_Color glowColor = {UI::ACCENT.r, UI::ACCENT.g, UI::ACCENT.b, 40};
        SDL_Color sdlSubtitleColor = UI::get_text_dark_color();

        // Convert SDL_Color to SDL_Color
        SDL_Color titleColor = {sdlTitleColor.r, sdlTitleColor.g, sdlTitleColor.b, sdlTitleColor.a};
        SDL_Color glowTypesColor = {glowColor.r, glowColor.g, glowColor.b, glowColor.a};
        SDL_Color subtitleTypesColor = {sdlSubtitleColor.r, sdlSubtitleColor.g, sdlSubtitleColor.b, sdlSubtitleColor.a};

        // Render background card (optional if you want a card-like look)
        int cardWidth = UI::get_window_width() - (UI::get_padding() * 2);
        int cardHeight = UI::get_window_height() - (UI::get_padding() * 2);
        UI::render_card(UI::get_padding(), UI::get_padding(), cardWidth, cardHeight);

        // Render subtitle
        UI::render_text("Simple and Fast NoSQL Database", 
                        UI::get_window_width() / 2 - 120, 
                        UI::get_window_height() / 6 + 40, 
                        subtitleTypesColor, UI::get_font());

        // Render title with shadow effect
        UI::render_text("HashBase", 
                        UI::get_window_width() / 2 - 70 + 1, 
                        UI::get_window_height() / 7 - 20 + 1, 
                        glowTypesColor, UI::get_title_font());
        
        UI::render_text("HashBase", 
                        UI::get_window_width() / 2 - 70, 
                        UI::get_window_height() / 7 - 20, 
                        titleColor, UI::get_title_font());

        // Ensure mainMenuButtons is defined and initialized properly
        // Example: 
        // std::vector<Types::Button> mainMenuButtons = { /* Initialize with appropriate button values */ };

        for (const auto& button : UI::main_menu_buttons) {
            UI::render_button(button);
        }
    }
}
