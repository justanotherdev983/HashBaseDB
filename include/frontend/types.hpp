#pragma once

#include <SDL2/SDL.h>
#include <string>

namespace Types {
    struct Button {
        SDL_Rect rect;
        std::string text;
        bool hovered;
        bool isSecondary;
    };

    // struct Color {
    //     Uint8 r, g, b, a;
    // };

    struct Record {
        std::string key;
        std::vector<std::string> values;
    };

    enum class Screen {
        MAIN_MENU,
        ADD_RECORD,
        REMOVE_RECORD,
        SEARCH_RECORD,
    };

    // SDL_Color to_sdl_color(const Color& color) {
    //     return { color.r, color.g, color.b, color.a };
    // }
}