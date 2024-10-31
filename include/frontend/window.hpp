#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include <vector>

#include "services/db_service.hpp"
#include "services/ui_service.hpp"
#include "frontend/components/main_menu.hpp"
#include "frontend/components/add_record.hpp"
#include "frontend/components/search_record.hpp"
#include "frontend/components/remove_record.hpp"

namespace Window {
    extern SDL_Window* window;
    extern SDL_Renderer* renderer;
    extern TTF_Font* font;
    extern TTF_Font* titleFont;

    bool initialize();
    void run();
    void cleanup();

    std::string get_database_file_path();
    std::string get_index_file_path();
}