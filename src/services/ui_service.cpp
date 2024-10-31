#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include <vector>

#include "frontend/window.hpp"
#include "services/ui_service.hpp"
#include "frontend/types.hpp"

namespace UI {
    std::vector<Types::Button> main_menu_buttons = {
        {{400, 200, 200, 50}, "Add Record", false, false},
        {{400, 300, 200, 50}, "View Data", false, false},
        {{400, 400, 200, 50}, "Exit", false, true}
    };

    Types::Screen currentScreen = Types::Screen::MAIN_MENU;

    Types::Button backButton = {{10, 10, 100, 30}, "Back", false, false};
    Types::Record newRecord;
    std::string searchQuery;

    std::string newRecordValue;
    bool isInputActive = false;
    std::vector<std::string> recordValues;

    void render_background(SDL_Renderer* renderer) {
        // Clear Screen with gradient background
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        SDL_SetRenderDrawColor(renderer, BACKGROUND.r, BACKGROUND.g, BACKGROUND.b, BACKGROUND.a);
        SDL_RenderClear(renderer);

        // Add subtle gradient overlay
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        for (int i = 0; i < WINDOW_HEIGHT; i++) {
            float alpha = 5.0f * (1.0f - (float)i / WINDOW_HEIGHT);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, static_cast<Uint8>(alpha));
            SDL_RenderDrawLine(renderer, 0, i, WINDOW_WIDTH, i);
        }
    }

    void draw_rounded_rectangle(SDL_Renderer* renderer, int x, int y, int w, int h, int radius) {
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
                    SDL_RenderDrawPoint(Window::renderer, x + radius - i, y + radius - j);                // Top-left
                    SDL_RenderDrawPoint(Window::renderer, x + w - radius + i - 1, y + radius - j);         // Top-right
                    SDL_RenderDrawPoint(Window::renderer, x + radius - i, y + h - radius + j - 1);         // Bottom-left
                    SDL_RenderDrawPoint(Window::renderer, x + w - radius + i - 1, y + h - radius + j - 1); // Bottom-right
                }
            }
        }
    }

    void render_text(const std::string& text, int x, int y, SDL_Color color, TTF_Font* currentFont) {
        if (!currentFont) currentFont = Window::font;
        SDL_Surface* surface = TTF_RenderText_Blended(currentFont, text.c_str(), color);
        if (!surface) return;

        SDL_Texture* texture = SDL_CreateTextureFromSurface(Window::renderer, surface);
        if (!texture) {
            SDL_FreeSurface(surface);
            return;
        }

        SDL_Rect rect;
        rect.x = x;
        rect.y = y;
        rect.w = surface->w;
        rect.h = surface->h;

        SDL_RenderCopy(Window::renderer, texture, nullptr, &rect);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    void render_card(int x, int y, int w, int h) {
        // Card shadow
        SDL_SetRenderDrawBlendMode(Window::renderer, SDL_BLENDMODE_BLEND);
        for (int i = 0; i < 4; i++) {
            SDL_SetRenderDrawColor(Window::renderer, 0, 0, 0, 40 - i * 10);
            draw_rounded_rectangle(Window::renderer, x + i, y + i, w, h, CORNER_RADIUS);
        }

        // Card background
        SDL_SetRenderDrawColor(Window::renderer, CARD_BG.r, CARD_BG.g, CARD_BG.b, CARD_BG.a);
        draw_rounded_rectangle(Window::renderer, x, y, w, h, CORNER_RADIUS);

        // Subtle gradient overlay
        SDL_SetRenderDrawBlendMode(Window::renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(Window::renderer, 255, 255, 255, 5);
        for (int i = 0; i < h / 2; i++) {
            SDL_RenderDrawLine(Window::renderer, x, y + i, x + w, y + i);
        }
    }

    void render_button(const Types::Button& button) {
        SDL_SetRenderDrawBlendMode(Window::renderer, SDL_BLENDMODE_BLEND);

        // Button shadow
        for (int i = 0; i < 3; i++) {
            SDL_SetRenderDrawColor(Window::renderer, 0, 0, 0, 30 - i * 10);
            draw_rounded_rectangle(Window::renderer, button.rect.x + i, button.rect.y + i, button.rect.w, button.rect.h, CORNER_RADIUS);
        }

        // Button background
        if (button.isSecondary) {
            if (button.hovered) {
                SDL_SetRenderDrawColor(Window::renderer, BUTTON_SECONDARY_HOVER.r, BUTTON_SECONDARY_HOVER.g, BUTTON_SECONDARY_HOVER.b, 255);
            } else {
                SDL_SetRenderDrawColor(Window::renderer, BUTTON_SECONDARY.r, BUTTON_SECONDARY.g, BUTTON_SECONDARY.b, 255);
            }
        } else {
            if (button.hovered) {
                SDL_SetRenderDrawColor(Window::renderer, PRIMARY_HOVER.r, PRIMARY_HOVER.g, PRIMARY_HOVER.b, 255);
            } else {
                SDL_SetRenderDrawColor(Window::renderer, PRIMARY.r, PRIMARY.g, PRIMARY.b, 255);
            }
        }

        draw_rounded_rectangle(Window::renderer, button.rect.x, button.rect.y, button.rect.w, button.rect.h, CORNER_RADIUS);

        // Button text with subtle shadow
        SDL_Color shadowColor = {0, 0, 0, 100};
        SDL_Color textColor = {TEXT_LIGHT.r, TEXT_LIGHT.g, TEXT_LIGHT.b, TEXT_LIGHT.a};
        std::string displayText = button.text;

        int textWidth = displayText.length() * (button.isSecondary ? 7 : 8);
        int textX = button.rect.x + (button.rect.w - textWidth) / 2;
        int textY = button.rect.y + (button.rect.h - 16) / 2;

        // Render text shadow
        render_text(displayText, textX + 1, textY + 1, shadowColor, Window::font);
        // Render main text
        render_text(displayText, textX, textY, textColor, Window::font);
    }

    bool handle_click(int x, int y) {
        if (currentScreen != Types::Screen::MAIN_MENU) {
            const SDL_Rect& backRect = backButton.rect;
            if (x >= backRect.x && x < backRect.x + backRect.w && y >= backRect.y && y < backRect.y + backRect.h) {
                currentScreen = Types::Screen::MAIN_MENU;
                return true;
            }
        }

        if (currentScreen == Types::Screen::MAIN_MENU) {
            for (size_t i = 0; i < main_menu_buttons.size(); i++) {
                const SDL_Rect& rect = main_menu_buttons[i].rect;
                if (x >= rect.x && x < rect.x + rect.w && y >= rect.y && y < rect.y + rect.h) {
                    if (i == 0) currentScreen = Types::Screen::SEARCH_RECORD;
                    else if (i == 1) currentScreen = Types::Screen::ADD_RECORD;
                    else if (i == 2) return false;
                    break;
                }
            }
        } else if (currentScreen == Types::Screen::ADD_RECORD) {
            SDL_Rect inputRect = {
                PADDING * 2,
                PADDING * 4 + 30,
                WINDOW_WIDTH - PADDING * 4,
                40
            };

            if (x >= inputRect.x && x < inputRect.x + inputRect.w && y >= inputRect.y && y < inputRect.y + inputRect.h) {
                isInputActive = true;
            } else {
                isInputActive = false;
            }

            SDL_Rect addRect = {
                WINDOW_WIDTH / 2 - BUTTON_WIDTH / 2,
                PADDING * 4 + 40 * 4,
                BUTTON_WIDTH,
                BUTTON_HEIGHT
            };

            if (x >= addRect.x && x < addRect.x + addRect.w && y >= addRect.y && y < addRect.y + addRect.h) {
                recordValues.push_back(newRecordValue);
                newRecordValue.clear();
                currentScreen = Types::Screen::MAIN_MENU;
            }
        }

        return true;
    }

    void handle_keyboard(SDL_Event& event) {
        if (currentScreen == Types::Screen::ADD_RECORD && isInputActive) {
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_BACKSPACE && !newRecordValue.empty()) {
                    newRecordValue.pop_back();
                } else if (event.key.keysym.sym == SDLK_RETURN) {
                    recordValues.push_back(newRecordValue);
                    newRecordValue.clear();
                } else if (event.key.keysym.sym != SDLK_RETURN) {
                    newRecordValue += event.key.keysym.sym;
                }
            }
        }
    }

    void update_button_hover(int x, int y) {
        for (auto& button : main_menu_buttons) {
            button.hovered = (x >= button.rect.x && x < button.rect.x + button.rect.w &&
                              y >= button.rect.y && y < button.rect.y + button.rect.h);
        }

        if (currentScreen != Types::Screen::MAIN_MENU) {
            backButton.hovered = (x >= backButton.rect.x &&
                                  x < backButton.rect.x + backButton.rect.w &&
                                  y >= backButton.rect.y &&
                                  y < backButton.rect.y + backButton.rect.h);
        }
    }

    int get_window_width() {
        return WINDOW_WIDTH;
    }

    int get_window_height() {
        return WINDOW_HEIGHT;
    }

    int get_button_width() {
        return BUTTON_WIDTH;
    }

    int get_button_height() {
        return BUTTON_HEIGHT;
    }

    int get_padding() {
        return PADDING;
    }

    bool is_main_menu() {
        return currentScreen == Types::Screen::MAIN_MENU;
    }

    bool is_search_record() {
        return currentScreen == Types::Screen::SEARCH_RECORD;
    }

    bool is_add_record() {
        return currentScreen == Types::Screen::ADD_RECORD;
    }

    bool is_remove_record() {
        return currentScreen == Types::Screen::REMOVE_RECORD;
    }

    TTF_Font* get_title_font() {
        return Window::titleFont;
    }

    TTF_Font* get_font() {
        return Window::font;
    }

    SDL_Color get_text_light_color() {
        return {TEXT_LIGHT.r, TEXT_LIGHT.g, TEXT_LIGHT.b, TEXT_LIGHT.a};
    }
    SDL_Color get_text_dark_color() {
        return {TEXT_DARK.r, TEXT_DARK.g, TEXT_DARK.b, TEXT_DARK.a};
    }

    std::string get_new_record_value() {
        return newRecordValue;
    }



    void render_input_box(int x, int y, int width, int height, const std::string& inputText, bool isActive) {
        // Draw input box background
        SDL_SetRenderDrawColor(Window::renderer, isActive ? INPUT_BOX_ACTIVE_BG.r : INPUT_BOX_BG.r, 
                                      isActive ? INPUT_BOX_ACTIVE_BG.g : INPUT_BOX_BG.g, 
                                      isActive ? INPUT_BOX_ACTIVE_BG.b : INPUT_BOX_BG.b, 
                                      255);
        SDL_Rect inputRect = { x, y, width, height };
        SDL_RenderFillRect(Window::renderer, &inputRect);

        // Draw input text
        SDL_Color textColor = { TEXT_COLOR.r, TEXT_COLOR.g, TEXT_COLOR.b, TEXT_COLOR.a };
        render_text(inputText, x + 5, y + 5, textColor, Window::font);

        // Draw border for input box
        SDL_SetRenderDrawColor(Window::renderer, BORDER_COLOR.r, BORDER_COLOR.g, BORDER_COLOR.b, 255);
        SDL_RenderDrawRect(Window::renderer, &inputRect);
    }


    TTF_Font* load_font(const std::vector<std::string>& paths, int size) {
        for (const auto& path : paths) {
            TTF_Font* loadedFont = TTF_OpenFont(path.c_str(), size);
            if (loadedFont) return loadedFont;
        }
        std::cout << "Font load error: " << TTF_GetError() << std::endl;
        return nullptr;
    }

    std::string get_database_file_path() {
        // TODO: Ask this from user
        return "../database/winkel.db";
    }

    std::string get_index_file_path() {
        // TODO: Ask this from user
        return "../database/index.idx";
    }
}