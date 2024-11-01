#include "frontend/window.hpp"
#include "services/ui_service.hpp"

namespace Search_record {
    void render() {
        int cardWidth = UI::get_window_width() - (UI::get_padding() * 2);
        int cardHeight = UI::get_window_height() - (UI::get_padding() * 2);
        
        // Render background card
        UI::render_card(UI::get_padding(), UI::get_padding(), cardWidth, cardHeight);

        // Render back button
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

        // Title text
        SDL_Color titleColor = UI::get_text_light_color();
        SDL_Color titleTypesColor = { titleColor.r, titleColor.g, titleColor.b, titleColor.a };
        UI::render_text("Database Records", 
                        UI::get_window_width() / 2 - 120, 
                        UI::get_padding() * 2, 
                        titleTypesColor, UI::get_title_font());

        // Display search results
        int resultsY = UI::get_padding() * 4;
        UI::render_text("Search Results:", 
                        UI::get_padding(), resultsY, 
                        {titleColor.r, titleColor.g, titleColor.b, 200}, UI::get_font());
        resultsY += 30;

        // Render search results from your database
        std::vector<std::string> results = Database_service::search_database();
        for (const auto& result : results) {
            UI::render_text(result, 
                            UI::get_padding(), resultsY, 
                            {titleColor.r, titleColor.g, titleColor.b, 255}, UI::get_font());
            resultsY += 24;
        }
    }
}
