#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include <vector>

#include "frontend/window.hpp"
#include "frontend/types.hpp"

namespace UI {
    constexpr int WINDOW_WIDTH = 1000;
    constexpr int WINDOW_HEIGHT = 700;
    constexpr int BUTTON_WIDTH = 200;
    constexpr int BUTTON_HEIGHT = 50;
    constexpr int PADDING = 30;
    constexpr int CORNER_RADIUS = 8;
    
    
    const SDL_Color BACKGROUND = {15, 23, 42, 255};         // Slate 900
    const SDL_Color PRIMARY = {79, 70, 229, 255};          // Indigo 600
    const SDL_Color PRIMARY_HOVER = {99, 102, 241, 255};   // Indigo 500
    const SDL_Color TEXT_DARK = {226, 232, 240, 255};      // Slate 200
    const SDL_Color TEXT_LIGHT = {248, 250, 252, 255};     // Slate 50
    const SDL_Color CARD_BG = {30, 41, 59, 255};          // Slate 800
    const SDL_Color BUTTON_SECONDARY = {239, 68, 68, 255}; // Red 500
    const SDL_Color BUTTON_SECONDARY_HOVER = {220, 38, 38, 255}; // Red 600
    const SDL_Color ACCENT = {56, 189, 248, 255};         // Sky 400 (for highlights)
    const SDL_Color INPUT_BOX_ACTIVE_BG = { 100, 150, 200, 255 };  // Light blue background for active input box
    const SDL_Color INPUT_BOX_BG = { 240, 240, 240, 255 };         // Light gray background for inactive input box
    const SDL_Color TEXT_COLOR = { 0, 0, 0, 255 };                 // Black color for text
    const SDL_Color BORDER_COLOR = { 50, 50, 50, 255 };            // Dark gray for borders

    extern std::vector<Types::Button> main_menu_buttons;
    extern Types::Screen currentScreen;

    void draw_rounded_rectangle(SDL_Renderer* renderer, int x, int y, int w, int h, int radius);

    void render_text(const std::string& text, int x, int y, SDL_Color color, TTF_Font* currentFont);
    void render_button(const Types::Button& button);
    void render_background(SDL_Renderer* renderer);

    void render_card(int x, int y, int w, int h);

    bool handle_click(int x, int y);

    void handle_keyboard(SDL_Event& event);
    
    void update_button_hover(int x, int y);

    TTF_Font* load_font(const std::vector<std::string>& paths, int size);

    int get_window_width();
    int get_window_height();

    int get_button_width();
    int get_button_height();

    int get_padding();

    bool is_main_menu();
    bool is_search_record();
    bool is_add_record();
    bool is_remove_record();

    SDL_Color get_text_light_color();
    SDL_Color get_text_dark_color();

    std::string get_new_record_value();
    std::string get_new_record_uuid();

    TTF_Font* get_title_font();
    TTF_Font* get_font();

    std::string get_current_UUID();
    std::vector<std::string> get_current_values();
    std::string get_current_input_value();
    bool is_uuid_input_active();
    bool is_value_input_active();

    void render_input_box(int x, int y, int width, int height, const std::string& inputText, bool isActive);

    std::string get_database_file_path();

    std::string get_index_file_path();
}