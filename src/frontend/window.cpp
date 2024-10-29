#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include <vector>
#include "../include/services/db_service.hpp"

namespace Window {
    // Constants for styling
    constexpr int WINDOW_WIDTH = 1000;
    constexpr int WINDOW_HEIGHT = 700;
    constexpr int BUTTON_WIDTH = 200;
    constexpr int BUTTON_HEIGHT = 50;
    constexpr int PADDING = 30;
    constexpr int CORNER_RADIUS = 8;  // Added for rounded corners
    
    // Modern color scheme inspired by popular dark themes
    struct Color {
        Uint8 r, g, b, a;
    };
    
    const Color BACKGROUND = {15, 23, 42, 255};         // Slate 900
    const Color PRIMARY = {79, 70, 229, 255};          // Indigo 600
    const Color PRIMARY_HOVER = {99, 102, 241, 255};   // Indigo 500
    const Color TEXT_DARK = {226, 232, 240, 255};      // Slate 200
    const Color TEXT_LIGHT = {248, 250, 252, 255};     // Slate 50
    const Color CARD_BG = {30, 41, 59, 255};          // Slate 800
    const Color BUTTON_SECONDARY = {239, 68, 68, 255}; // Red 500
    const Color BUTTON_SECONDARY_HOVER = {220, 38, 38, 255}; // Red 600
    const Color ACCENT = {56, 189, 248, 255};         // Sky 400 (for highlights)
    
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    TTF_Font* font = nullptr;
    TTF_Font* titleFont = nullptr;

    enum class Screen {
        MAIN_MENU,
        VIEW_DATA,
        ADD_RECORD
    };

    struct Button {
        SDL_Rect rect;
        std::string text;
        bool hovered;
        bool isSecondary;
        bool isSmall;
    };

    struct Record {
        std::string uniqueId;
        std::string field1;
        std::string field2;
        std::string field3;
    };

    Record newRecord;
    Screen currentScreen = Screen::MAIN_MENU;
    std::vector<Button> mainMenuButtons;
    Button backButton;
    std::string searchQuery;

    
    // Helper function to draw rounded rectangle
    void drawRoundedRectangle(SDL_Renderer* renderer, int x, int y, int w, int h, int radius) {
        // Draw the main rectangle
        SDL_Rect rect = {x + radius, y, w - 2 * radius, h};
        SDL_RenderFillRect(renderer, &rect);
        rect = {x, y + radius, w, h - 2 * radius};
        SDL_RenderFillRect(renderer, &rect);

        // Draw the corner circles
        for (int i = 0; i <= radius; i++) {
            for (int j = 0; j <= radius; j++) {
                float distance = sqrt(i * i + j * j);
                if (distance <= radius) {
                    SDL_RenderDrawPoint(renderer, x + radius - i, y + radius - j);            // Top-left
                    SDL_RenderDrawPoint(renderer, x + w - radius + i - 1, y + radius - j);    // Top-right
                    SDL_RenderDrawPoint(renderer, x + radius - i, y + h - radius + j - 1);    // Bottom-left
                    SDL_RenderDrawPoint(renderer, x + w - radius + i - 1, y + h - radius + j - 1); // Bottom-right
                }
            }
        }
    }

    
    
    bool initialize() {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            std::cerr << "SDL Error: " << SDL_GetError() << std::endl;
            return false;
        }
        
        if (TTF_Init() != 0) {
            std::cerr << "TTF Error: " << TTF_GetError() << std::endl;
            return false;
        }
        
        window = SDL_CreateWindow(
            "HashBase Database",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            WINDOW_WIDTH, WINDOW_HEIGHT,
            SDL_WINDOW_SHOWN
        );
        
        if (!window) return false;
        
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!renderer) return false;
        
        // Try multiple font paths for better compatibility
        const char* fontPaths[] = {
            "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
            "C:\\Windows\\Fonts\\arial.ttf",
            "assets/fonts/DejaVuSans.ttf",
            "./DejaVuSans.ttf"
        };
        
        for (const char* path : fontPaths) {
            font = TTF_OpenFont(path, 16);
            if (font) break;
        }
        
        for (const char* path : fontPaths) {
            titleFont = TTF_OpenFont(path, 28);
            if (titleFont) break;
        }
        
        if (!font || !titleFont) {
            std::cerr << "TTF Error: " << TTF_GetError() << std::endl;
            return false;
        }
        
        // Center buttons vertically with more spacing
        int startY = (WINDOW_HEIGHT - (3 * (BUTTON_HEIGHT + PADDING * 2))) / 2;
        mainMenuButtons = {
            {{(WINDOW_WIDTH - BUTTON_WIDTH) / 2, startY, BUTTON_WIDTH, BUTTON_HEIGHT}, 
             "View Database", false, false, false},
            {{(WINDOW_WIDTH - BUTTON_WIDTH) / 2, startY + (BUTTON_HEIGHT + PADDING * 2), BUTTON_WIDTH, BUTTON_HEIGHT}, 
             "Add New Record", false, false, false},
            {{(WINDOW_WIDTH - BUTTON_WIDTH) / 2, startY + 2 * (BUTTON_HEIGHT + PADDING * 2), BUTTON_WIDTH, BUTTON_HEIGHT}, 
             "Exit", false, true, false}
        };
        
        // Initialize back button with arrow symbol
        backButton = {
            {PADDING, PADDING, 100, 40},
            "Back",  // Changed from arrow to text
            false, 
            false,
            true
        };
        
        return true;
    }

    void renderText(const std::string& text, int x, int y, SDL_Color color, TTF_Font* currentFont = nullptr) {
        if (!currentFont) currentFont = font;
        SDL_Surface* surface = TTF_RenderText_Blended(currentFont, text.c_str(), color);
        if (!surface) return;
        
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture) {
            SDL_FreeSurface(surface);
            return;
        }
        
        SDL_Rect rect;
        rect.x = x;
        rect.y = y;
        rect.w = surface->w;
        rect.h = surface->h;
        
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
        
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    void renderCard(int x, int y, int w, int h) {
        // Card shadow
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        for (int i = 0; i < 4; i++) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 40 - i * 10);
            drawRoundedRectangle(renderer, x + i, y + i, w, h, CORNER_RADIUS);
        }
        
        // Card background
        SDL_SetRenderDrawColor(renderer, CARD_BG.r, CARD_BG.g, CARD_BG.b, CARD_BG.a);
        drawRoundedRectangle(renderer, x, y, w, h, CORNER_RADIUS);
        
        // Subtle gradient overlay
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 5);
        for (int i = 0; i < h/2; i++) {
            SDL_RenderDrawLine(renderer, x, y + i, x + w, y + i);
        }
    }
    
    void renderRoundedButton(const Button& button) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        
        // Button shadow
        for (int i = 0; i < 3; i++) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 30 - i * 10);
            drawRoundedRectangle(renderer, 
                               button.rect.x + i, 
                               button.rect.y + i, 
                               button.rect.w, 
                               button.rect.h, 
                               CORNER_RADIUS);
        }
        
        // Button background
        if (button.isSecondary) {
            if (button.hovered) {
                SDL_SetRenderDrawColor(renderer, 
                                     BUTTON_SECONDARY_HOVER.r, 
                                     BUTTON_SECONDARY_HOVER.g, 
                                     BUTTON_SECONDARY_HOVER.b, 
                                     255);
            } else {
                SDL_SetRenderDrawColor(renderer, 
                                     BUTTON_SECONDARY.r, 
                                     BUTTON_SECONDARY.g, 
                                     BUTTON_SECONDARY.b, 
                                     255);
            }
        } else {
            if (button.hovered) {
                SDL_SetRenderDrawColor(renderer, 
                                     PRIMARY_HOVER.r, 
                                     PRIMARY_HOVER.g, 
                                     PRIMARY_HOVER.b, 
                                     255);
            } else {
                SDL_SetRenderDrawColor(renderer, 
                                     PRIMARY.r, 
                                     PRIMARY.g, 
                                     PRIMARY.b, 
                                     255);
            }
        }
        
        drawRoundedRectangle(renderer, 
                           button.rect.x, 
                           button.rect.y, 
                           button.rect.w, 
                           button.rect.h, 
                           CORNER_RADIUS);
        
        // Button text with subtle shadow
        SDL_Color shadowColor = {0, 0, 0, 100};
        SDL_Color textColor = {TEXT_LIGHT.r, TEXT_LIGHT.g, TEXT_LIGHT.b, TEXT_LIGHT.a};
        std::string displayText = button.text;
        
        
        // Calculate text position
        int textWidth = displayText.length() * (button.isSmall ? 7 : 8);
        int textX = button.rect.x + (button.rect.w - textWidth) / 2;
        int textY = button.rect.y + (button.rect.h - 16) / 2;
        
        // Render text shadow
        renderText(displayText, textX + 1, textY + 1, shadowColor);
        // Render main text
        renderText(displayText, textX, textY, textColor);
    }
    
    void renderMainMenu() {
        // Title with modern styling
        SDL_Color glowColor = {ACCENT.r, ACCENT.g, ACCENT.b, 40};
        SDL_Color titleColor = {TEXT_LIGHT.r, TEXT_LIGHT.g, TEXT_LIGHT.b, TEXT_LIGHT.a};
        
        // Render subtitle
        SDL_Color subtitleColor = {TEXT_DARK.r, TEXT_DARK.g, TEXT_DARK.b, TEXT_DARK.a};
        renderText("Simple and Fast NoSQL Database", 
                  WINDOW_WIDTH/2 - 120, 
                  WINDOW_HEIGHT/6 + 40, 
                  subtitleColor);
        
        // Main title with glow effect
        renderText("HashBase", 
                  WINDOW_WIDTH/2 - 70 + 1, 
                  WINDOW_HEIGHT/7 - 20 + 1, 
                  glowColor, 
                  titleFont);
        
        renderText("HashBase", 
                  WINDOW_WIDTH/2 - 70, 
                  WINDOW_HEIGHT/7 - 20, 
                  titleColor, 
                  titleFont);
        
        // Render all buttons
        for (const auto& button : mainMenuButtons) {
            renderRoundedButton(button);
        }
    }
    
     void renderViewData() {
        int cardWidth = WINDOW_WIDTH - (PADDING * 2);
        int cardHeight = WINDOW_HEIGHT - (PADDING * 2);
        renderCard(PADDING, PADDING, cardWidth, cardHeight);

        renderRoundedButton(backButton);

        SDL_Color titleColor = {TEXT_LIGHT.r, TEXT_LIGHT.g, TEXT_LIGHT.b, TEXT_LIGHT.a};
        renderText("Database Records", WINDOW_WIDTH/2 - 120, PADDING * 2, titleColor, titleFont);

        // Display search results
        int resultsY = PADDING * 4;
        renderText("Search Results:", PADDING, resultsY, {TEXT_LIGHT.r, TEXT_LIGHT.g, TEXT_LIGHT.b, 200}, font);
        resultsY += 30;

        // Render search results from your database
        std::vector<std::string> results = Database_service::search_database();
        for (const auto& result : results) {
            renderText(result, PADDING, resultsY, {TEXT_LIGHT.r, TEXT_LIGHT.g, TEXT_LIGHT.b, 255}, font);
            resultsY += 24;
        }
    }
    
    void renderAddRecord() {
        int cardWidth = WINDOW_WIDTH - (PADDING * 2);
        int cardHeight = WINDOW_HEIGHT - (PADDING * 2);
        renderCard(PADDING, PADDING, cardWidth, cardHeight);

        renderRoundedButton(backButton);

        SDL_Color titleColor = {TEXT_LIGHT.r, TEXT_LIGHT.g, TEXT_LIGHT.b, TEXT_LIGHT.a};
        renderText("Add New Record", WINDOW_WIDTH/2 - 100, PADDING * 2, titleColor, titleFont);

        // Render input fields for new record
        int inputY = PADDING * 4;
        renderText("Unique ID:", PADDING, inputY, {TEXT_LIGHT.r, TEXT_LIGHT.g, TEXT_LIGHT.b, 200}, font);
        inputY += 30;

        // Unique ID input field
        SDL_Rect idInputRect = {PADDING * 2, inputY, cardWidth - PADDING * 4, 40};
        SDL_SetRenderDrawColor(renderer, PRIMARY.r, PRIMARY.g, PRIMARY.b, 255);
        drawRoundedRectangle(renderer, idInputRect.x, idInputRect.y, idInputRect.w, idInputRect.h, CORNER_RADIUS);
        renderText(newRecord.uniqueId, idInputRect.x + 10, idInputRect.y + 10, {TEXT_LIGHT.r, TEXT_LIGHT.g, TEXT_LIGHT.b, 255}, font);
        inputY += 50;

        renderText("Field 1:", PADDING, inputY, {TEXT_LIGHT.r, TEXT_LIGHT.g, TEXT_LIGHT.b, 200}, font);
        inputY += 30;

        // Field 1 input field
        SDL_Rect field1InputRect = {PADDING * 2, inputY, cardWidth - PADDING * 4, 40};
        SDL_SetRenderDrawColor(renderer, PRIMARY.r, PRIMARY.g, PRIMARY.b, 255);
        drawRoundedRectangle(renderer, field1InputRect.x, field1InputRect.y, field1InputRect.w, field1InputRect.h, CORNER_RADIUS);
        renderText(newRecord.field1, field1InputRect.x + 10, field1InputRect.y + 10, {TEXT_LIGHT.r, TEXT_LIGHT.g, TEXT_LIGHT.b, 255}, font);
        inputY += 50;

        renderText("Field 2:", PADDING, inputY, {TEXT_LIGHT.r, TEXT_LIGHT.g, TEXT_LIGHT.b, 200}, font);
        inputY += 30;

        // Field 2 input field
        SDL_Rect field2InputRect = {PADDING * 2, inputY, cardWidth - PADDING * 4, 40};
        SDL_SetRenderDrawColor(renderer, PRIMARY.r, PRIMARY.g, PRIMARY.b, 255);
        drawRoundedRectangle(renderer, field2InputRect.x, field2InputRect.y, field2InputRect.w, field2InputRect.h, CORNER_RADIUS);
        renderText(newRecord.field2, field2InputRect.x + 10, field2InputRect.y + 10, {TEXT_LIGHT.r, TEXT_LIGHT.g, TEXT_LIGHT.b, 255}, font);
        inputY += 50;

        renderText("Field 3:", PADDING, inputY, {TEXT_LIGHT.r, TEXT_LIGHT.g, TEXT_LIGHT.b, 200}, font);
        inputY += 30;

        // Field 3 input field
        SDL_Rect field3InputRect = {PADDING * 2, inputY, cardWidth - PADDING * 4, 40};
        SDL_SetRenderDrawColor(renderer, PRIMARY.r, PRIMARY.g, PRIMARY.b, 255);
        drawRoundedRectangle(renderer, field3InputRect.x, field3InputRect.y, field3InputRect.w, field3InputRect.h, CORNER_RADIUS);
        renderText(newRecord.field3, field3InputRect.x + 10, field3InputRect.y + 10, {TEXT_LIGHT.r, TEXT_LIGHT.g, TEXT_LIGHT.b, 255}, font);
        inputY += 50;

        // Render "Add" button
        Button addButton = {
            {WINDOW_WIDTH/2 - BUTTON_WIDTH/2, inputY, BUTTON_WIDTH, BUTTON_HEIGHT},
            "Add",
            false,
            false,
            false
        };
        renderRoundedButton(addButton);
    }

    bool handleClick(int x, int y) {
        if (currentScreen != Screen::MAIN_MENU) {
            const SDL_Rect& backRect = backButton.rect;
            if (x >= backRect.x && x < backRect.x + backRect.w &&
                y >= backRect.y && y < backRect.y + backRect.h) {
                currentScreen = Screen::MAIN_MENU;
                return true;
            }
        }

        if (currentScreen == Screen::MAIN_MENU) {
            for (size_t i = 0; i < mainMenuButtons.size(); i++) {
                const SDL_Rect& rect = mainMenuButtons[i].rect;
                if (x >= rect.x && x < rect.x + rect.w &&
                    y >= rect.y && y < rect.y + rect.h) {
                    if (i == 0) currentScreen = Screen::VIEW_DATA;
                    else if (i == 1) currentScreen = Screen::ADD_RECORD;
                    else if (i == 2) return false;
                    break;
                }
            }
        } else if (currentScreen == Screen::ADD_RECORD) {
            // Handle click on "Add" button
            const SDL_Rect& addRect = {WINDOW_WIDTH/2 - BUTTON_WIDTH/2, PADDING * 4 + 40 * 4, BUTTON_WIDTH, BUTTON_HEIGHT};
            if (x >= addRect.x && x < addRect.x + addRect.w &&
                y >= addRect.y && y < addRect.y + addRect.h) {
                // Call your add function here
                Database_service::add_record(newRecord.uniqueId, {newRecord.field1, newRecord.field2, newRecord.field3});
                currentScreen = Screen::MAIN_MENU;
            }
        }
        return true;
    }

    
   
    
    void updateButtonHover(int x, int y) {
        for (auto& button : mainMenuButtons) {
            button.hovered = (x >=button.rect.x && x < button.rect.x + button.rect.w &&
            y >= button.rect.y && y < button.rect.y + button.rect.h);
        }
        
        if (currentScreen != Screen::MAIN_MENU) {
            backButton.hovered = (x >= backButton.rect.x && x < backButton.rect.x + backButton.rect.w &&
                                y >= backButton.rect.y && y < backButton.rect.y + backButton.rect.h);
        }
    }
    
    void run() {
        bool running = true;
        SDL_Event event;
        
        // Enable VSync
        SDL_RenderSetVSync(renderer, 1);
        
        Uint32 frameStart;
        const int frameDelay = 1000 / 60;  // Cap at 60 FPS
        
        while (running) {
            frameStart = SDL_GetTicks();
            
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_QUIT:
                        running = false;
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        if (event.button.button == SDL_BUTTON_LEFT) {
                            running = handleClick(event.button.x, event.button.y);
                        }
                        break;
                    case SDL_MOUSEMOTION:
                        updateButtonHover(event.motion.x, event.motion.y);
                        break;
                    case SDL_KEYDOWN:
                        if (event.key.keysym.sym == SDLK_ESCAPE) {
                            if (currentScreen != Screen::MAIN_MENU) {
                                currentScreen = Screen::MAIN_MENU;
                            } else {
                                running = false;
                            }
                        }
                        break;
                }
            }
            
            // Clear screen with gradient background
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
            SDL_SetRenderDrawColor(renderer, BACKGROUND.r, BACKGROUND.g, BACKGROUND.b, BACKGROUND.a);
            SDL_RenderClear(renderer);
            
            // Add subtle gradient overlay
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            for (int i = 0; i < WINDOW_HEIGHT; i++) {
                float alpha = 5.0f * (1.0f - (float)i / WINDOW_HEIGHT);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, (Uint8)alpha);
                SDL_RenderDrawLine(renderer, 0, i, WINDOW_WIDTH, i);
            }
            
            switch (currentScreen) {
                case Screen::MAIN_MENU:
                    renderMainMenu();
                    break;
                case Screen::VIEW_DATA:
                    renderViewData();
                    break;
                case Screen::ADD_RECORD:
                    renderAddRecord();
                    break;
            }
            
            SDL_RenderPresent(renderer);
            
            // Frame rate control
            int frameTime = SDL_GetTicks() - frameStart;
            if (frameDelay > frameTime) {
                SDL_Delay(frameDelay - frameTime);
            }
        }
    }
    
    void cleanup() {
        if (font) TTF_CloseFont(font);
        if (titleFont) TTF_CloseFont(titleFont);
        if (renderer) SDL_DestroyRenderer(renderer);
        if (window) SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
    }

    std::string get_database_file_path() {
        //TODO: Ask this from user

        return "../database/winkel.db";
    }

    std::string get_index_file_path() {
        //TODO: Ask this from user
        
        return "../database/index.idx";
    }
}