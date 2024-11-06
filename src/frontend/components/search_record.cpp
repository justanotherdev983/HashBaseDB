#include "frontend/window.hpp"
#include "services/ui_service.hpp"

namespace Search_record {
    void render() {
        int cardWidth = UI::get_window_width() - (UI::get_padding() * 2);
        int cardHeight = UI::get_window_height() - (UI::get_padding() * 2);
        
        // Render main background card
        UI::render_card(UI::get_padding(), UI::get_padding(), cardWidth, cardHeight);
        

        // Render back button
        SDL_Rect backButtonRect = {
            UI::get_padding(), 
            UI::get_padding(), 
            100,  
            40    
        };
        Types::Button backButton = { backButtonRect, "Back", false, false };
        UI::render_button(backButton);

        // Title text with improved positioning
        SDL_Color titleColor = UI::get_text_light_color();
        UI::render_text("Search Database Records", 
                       UI::get_window_width() / 2 - 150, 
                       UI::get_padding() * 2, 
                       titleColor, 
                       UI::get_title_font());

        // Search input section
        int searchSectionY = UI::get_padding() * 4 + 60;
        
        // Search input box
        SDL_Rect searchInputRect = {
            UI::get_padding() * 2,
            searchSectionY,
            cardWidth - UI::get_padding() * 4 - UI::get_button_width() - UI::get_padding(),
            40
        };
        UI::render_input_box(
            searchInputRect.x,
            searchInputRect.y,
            searchInputRect.w,
            searchInputRect.h,
            UI::get_current_UUID(),
            UI::is_value_input_active()
        );

        // Search button
        SDL_Rect searchButtonRect = {
            searchInputRect.x + searchInputRect.w + UI::get_padding(),
            searchSectionY,
            UI::get_button_width(),
            UI::get_button_height()
        };
        Types::Button searchButton = { searchButtonRect, "Search", false, false };
        UI::render_button(searchButton);

        // Results section
        int resultsY = searchSectionY + UI::get_button_height() + UI::get_padding() * 2;
        
        // Results card
        UI::render_card(
            UI::get_padding() * 2,
            resultsY,
            cardWidth - UI::get_padding() * 4,
            cardHeight - resultsY - UI::get_padding() * 2
        );

        // Results header
        SDL_Color textColor = UI::get_text_light_color();
        UI::render_text("Search Results", 
                       UI::get_padding() * 3,
                       resultsY + UI::get_padding(),
                       textColor,
                       UI::get_font());

        // Render search results
        int resultItemY = resultsY + UI::get_padding() * 3;
        std::vector<std::string> results = Database_service::search_database(UI::get_current_UUID());
        
        if (results.empty()) {
            // Show message when no results found
            UI::render_text("No records found. Try a different UUID.", 
                          UI::get_padding() * 3,
                          resultItemY,
                          {textColor.r, textColor.g, textColor.b, 180},
                          UI::get_font());
        } else {
            // Render each result in a card-like container
            for (const auto& result : results) {
                if (resultItemY + 60 > cardHeight - UI::get_padding()) break; // Prevent overflow
                
                // Result item background
                SDL_SetRenderDrawColor(Window::renderer, 255, 255, 255, 15);
                SDL_Rect resultRect = {
                    UI::get_padding() * 3,
                    resultItemY,
                    cardWidth - UI::get_padding() * 6,
                    50
                };
                SDL_RenderFillRect(Window::renderer, &resultRect);
                
                // Result text
                UI::render_text(result,
                              UI::get_padding() * 4,
                              resultItemY + UI::get_padding(),
                              textColor,
                              UI::get_font());
                
                resultItemY += 60; // Space between results
            }
            
            // Show result count
            std::string resultCount = "Showing " + std::to_string(results.size()) + " results";
            UI::render_text(resultCount,
                          UI::get_padding() * 3,
                          cardHeight - UI::get_padding() * 6,
                          {textColor.r, textColor.g, textColor.b, 180},
                          UI::get_font());
        }
    }
}