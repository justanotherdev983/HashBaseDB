#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include "services/db_service.hpp"
#include "services/ui_service.hpp"
#include "frontend/components/main_menu.hpp"
#include "frontend/components/add_record.hpp"
#include "frontend/components/search_record.hpp"
#include "frontend/components/remove_record.hpp"

namespace Window {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    TTF_Font* font = nullptr;
    TTF_Font* titleFont = nullptr;

    bool initialize() {
        //cleanup();

        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cout << "SDL initialization failed: " << SDL_GetError() << std::endl;
            return false;
        }

        if (TTF_Init() == -1) {
            std::cout << "SDL_ttf initialization failed: " << TTF_GetError() << std::endl;
            SDL_Quit();
            return false;
        }

        window = SDL_CreateWindow(
            "HashBase Database", 
            SDL_WINDOWPOS_CENTERED, 
            SDL_WINDOWPOS_CENTERED, 
            UI::get_window_width(), 
            UI::get_window_height(), 
            SDL_WINDOW_SHOWN
        );

        if (!window) {
            std::cout << "Window creation failed: " << SDL_GetError() << std::endl;
            TTF_Quit();
            SDL_Quit();
            return false;
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!renderer) {
            std::cout << "Renderer creation failed: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(window);
            TTF_Quit();
            SDL_Quit();
            return false;
        }

        font = UI::load_font({"assets/fonts/DejaVuSans.ttf", 
                            "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 
                            "C:\\Windows\\Fonts\\arial.ttf"}, 16);
        titleFont = UI::load_font({"assets/fonts/DejaVuSans.ttf", 
                                "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 
                                "C:\\Windows\\Fonts\\arial.ttf"}, 28);

        return true;
    }


    void cleanup() {
        if (font) {
            TTF_CloseFont(font);
            font = nullptr;
        }
        if (titleFont) {
            TTF_CloseFont(titleFont);
            titleFont = nullptr;
        }
        if (renderer) {
            SDL_DestroyRenderer(renderer);
            renderer = nullptr;
        }
        if (window) {
            SDL_DestroyWindow(window);
            window = nullptr;
        }
        TTF_Quit();
        SDL_Quit();
    }

    void run() {
        bool running = true;
        SDL_Event event;

        while (running) {
            try {
                while (SDL_PollEvent(&event)) {
                    switch (event.type) {
                        case SDL_QUIT:
                            running = false;
                            break;
                        case SDL_MOUSEBUTTONDOWN:
                            if (event.button.button == SDL_BUTTON_LEFT) {
                                running = UI::handle_click(event.button.x, event.button.y);
                            }
                            break;
                        case SDL_MOUSEMOTION:
                            UI::update_button_hover(event.motion.x, event.motion.y);
                            break;
                        case SDL_KEYDOWN:
                            if (event.key.keysym.sym == SDLK_ESCAPE && UI::is_main_menu()) {
                                running = false;
                            }
                            UI::handle_keyboard(event);
                            break;
                    }
                }

                if (!renderer) {
                    std::cout << "Renderer became invalid during event loop!" << std::endl;
                    break;
                }

                SDL_RenderClear(renderer);
                
                switch (UI::currentScreen) {
                    case Types::Screen::MAIN_MENU:
                        Main_menu::render();
                        break;
                    case Types::Screen::ADD_RECORD:
                        Add_record::render();
                        break;
                    case Types::Screen::REMOVE_RECORD:
                        Remove_record::render();
                        break;
                    case Types::Screen::SEARCH_RECORD:
                        Search_record::render();

                        break;
                }

                SDL_RenderPresent(renderer);
            }
            catch (const std::exception& e) {
                std::cout << "Exception in event loop: " << e.what() << std::endl;
                running = false;
            }
        }

        cleanup();
    }

}