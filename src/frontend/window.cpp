#include <SDL.h>
#include <iostream>
#include <string>

namespace Window {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    bool initialize() {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
            return false;
        }
        window = SDL_CreateWindow("App", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
        return window != nullptr;
    }

    void run() {
        bool running = true;
        SDL_Event event;
        while (running) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) running = false;
            }
            SDL_RenderClear(renderer);
            SDL_RenderPresent(renderer);
        }
    }

    void cleanup() {
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    std::string get_database_file_path() {
        // TODO: Ask this from user in UI

        return "../database/winkel.db";
    }

    std::string get_index_file_path() {
        // TODO: Ask this from user in UI

        return "../database/index.idx";
    }
}