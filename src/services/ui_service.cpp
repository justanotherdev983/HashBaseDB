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
        {{400, 300, 200, 50}, "Search Record", false, false}, 
        {{400, 400, 200, 50}, "Remove Record", false, false}, 
        {{400, 500, 200, 50}, "Exit", false, true}
    };

    Types::Screen currentScreen = Types::Screen::MAIN_MENU;

    Types::Button backButton = {{10, 10, 100, 30}, "Back", false, false};
    Types::Record newRecord;
    std::string searchQuery;
    std::string newRecordUUID;

    std::string newRecordValue;
    bool isInputActive = false;
    std::vector<std::string> recordValues;

    std::string currentUUID;
    std::vector<std::string> currentValues;
    std::string currentInputValue;
    bool isUUIDInputActive = true;
    bool isValueInputActive = false;

    void reset_input_state() {
        currentUUID.clear();
        currentValues.clear();
        currentInputValue.clear();
        isUUIDInputActive = true;
        isValueInputActive = false;
    }

    void render_background(SDL_Renderer* renderer) {
        // Clear screen with a flat dark background
        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);  // Dark grey background
        SDL_RenderClear(renderer);

        // Add subtle vertical gradient overlay
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        for (int i = 0; i < WINDOW_HEIGHT; i++) {
            float alpha = 3.0f * (1.0f - (float)i / WINDOW_HEIGHT);
            SDL_SetRenderDrawColor(renderer, 40, 40, 40, static_cast<Uint8>(alpha)); // Slightly lighter shade
            SDL_RenderDrawLine(renderer, 0, i, WINDOW_WIDTH, i);
        }
    }

    void draw_rounded_rectangle(SDL_Renderer* renderer, int x, int y, int w, int h) {
        // Simplified rectangle with no rounded corners
        SDL_Rect rect = {x, y, w, h};
        SDL_RenderFillRect(renderer, &rect);
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

        SDL_Rect rect = {x, y, surface->w, surface->h};
        SDL_RenderCopy(Window::renderer, texture, nullptr, &rect);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    void render_card(int x, int y, int w, int h) {
        // Flat, minimalistic card background
        SDL_SetRenderDrawBlendMode(Window::renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(Window::renderer, 40, 40, 40, 255);  // Medium grey

        draw_rounded_rectangle(Window::renderer, x, y, w, h);

        // Add a simple horizontal gradient overlay on the top half
        for (int i = 0; i < h / 2; i++) {
            SDL_SetRenderDrawColor(Window::renderer, 60, 60, 60, 3); // Slightly lighter shade
            SDL_RenderDrawLine(Window::renderer, x, y + i, x + w, y + i);
        }
    }

    void render_button(const Types::Button& button) {
        // Flat, monochromatic button with hover effect
        SDL_SetRenderDrawBlendMode(Window::renderer, SDL_BLENDMODE_BLEND);

        SDL_Color btnColor;
        if (button.isSecondary) {
            btnColor = button.hovered ? SDL_Color{80, 80, 80, 255} : SDL_Color{60, 60, 60, 255};  // Dark grey shades
        } else {
            btnColor = button.hovered ? SDL_Color{255, 255, 255, 255} : SDL_Color{200, 200, 200, 255};  // Light grey shades
        }

        SDL_SetRenderDrawColor(Window::renderer, btnColor.r, btnColor.g, btnColor.b, btnColor.a);
        draw_rounded_rectangle(Window::renderer, button.rect.x, button.rect.y, button.rect.w, button.rect.h);

        // Render centered text in button
        SDL_Color textColor = {20, 20, 20, 255}; // Dark text for contrast
        std::string displayText = button.text;

        int textWidth = displayText.length() * (button.isSecondary ? 7 : 8);
        int textX = button.rect.x + (button.rect.w - textWidth) / 2;
        int textY = button.rect.y + (button.rect.h - 16) / 2;

        render_text(displayText, textX, textY, textColor, Window::font);
    }

    void render_input_box(int x, int y, int width, int height, const std::string& inputText, bool isActive) {
        // Input box with a flat color background
        SDL_SetRenderDrawColor(Window::renderer, isActive ? 255 : 50, isActive ? 255 : 50, isActive ? 255 : 50, 255);
        SDL_Rect inputRect = {x, y, width, height};
        SDL_RenderFillRect(Window::renderer, &inputRect);

        // Input text
        SDL_Color textColor = {200, 200, 200, 255}; // Light text color
        render_text(inputText, x + 5, y + (height - 16) / 2, textColor, Window::font);

        // Draw a simple outline for the input box
        SDL_SetRenderDrawColor(Window::renderer, 80, 80, 80, 255);
        SDL_RenderDrawRect(Window::renderer, &inputRect);
    }

    bool handle_click(int x, int y) {
        if (currentScreen != Types::Screen::MAIN_MENU) {
            const SDL_Rect& backRect = backButton.rect;
            if (x >= backRect.x - PADDING && 
                x < backRect.x + backRect.w + PADDING && 
                y >= backRect.y - PADDING && 
                y < backRect.y + backRect.h + PADDING) {
                    currentScreen = Types::Screen::MAIN_MENU;
                    return true;
            }
        }

        if (currentScreen == Types::Screen::MAIN_MENU) {
            for (const auto& button : main_menu_buttons) {
                const SDL_Rect& rect = button.rect;
                if (x >= rect.x && x < rect.x + rect.w && y >= rect.y && y < rect.y + rect.h) {
                    if (button.text == "Add Record") currentScreen = Types::Screen::ADD_RECORD;
                    else if (button.text == "Search Record") currentScreen = Types::Screen::SEARCH_RECORD;
                    else if (button.text == "Remove Record") currentScreen = Types::Screen::REMOVE_RECORD;
                    else if (button.text == "Exit") return false;
                    break;
                }
            }
        } else if (currentScreen == Types::Screen::ADD_RECORD) {
            // UUID input box
            SDL_Rect uuidRect = {
                get_padding() * 2,
                get_padding() * 4 + 90,
                get_window_width() - get_padding() * 4, 
                40
            };

            // Value input box
            SDL_Rect inputRect = {
                get_padding() * 2, 
                get_padding() * 4 + 250,
                get_window_width() - get_padding() * 4 - get_button_width() - get_padding(), 
                40
            };

            // New button rect - matches visual position exactly
            SDL_Rect newButtonRect = {
                inputRect.x + inputRect.w + get_padding(),
                inputRect.y,
                get_button_width(),
                get_button_height()
            };

            // Add button rect - matches visual position exactly
            SDL_Rect addButtonRect = {
                get_window_width() / 2 - get_button_width() / 2,
                inputRect.y + 50,
                get_button_width(),
                get_button_height()
            };

            // Check UUID input box click
            if (x >= uuidRect.x && 
                x < uuidRect.x + uuidRect.w && 
                y >= uuidRect.y && 
                y < uuidRect.y + uuidRect.h) {
                isUUIDInputActive = true;
                isValueInputActive = false;
                return true;
            }

            // Check Value input box click
            if (x >= inputRect.x && 
                x < inputRect.x + inputRect.w && 
                y >= inputRect.y && 
                y < inputRect.y + inputRect.h) {
                isUUIDInputActive = false;
                isValueInputActive = true;
                return true;
            }

            // Check New button click - no padding offset
            if (x >= newButtonRect.x && 
                x < newButtonRect.x + newButtonRect.w &&
                y >= newButtonRect.y && 
                y < newButtonRect.y + newButtonRect.h) {
                if (!currentInputValue.empty()) {
                    currentValues.push_back(currentInputValue);
                    currentInputValue.clear();
                }
                return true;
            }

            // Check Add button click - no padding offset
            if (x >= addButtonRect.x && 
                x < addButtonRect.x + addButtonRect.w &&
                y >= addButtonRect.y && 
                y < addButtonRect.y + addButtonRect.h) {
                Database_service::add_record(currentUUID, currentValues);
                std::cout << "UUID: " << currentUUID << std::endl;
                std::cout << "Values:" << std::endl;
                for (const auto& value : currentValues) {
                    std::cout << "- " << value << std::endl;
                }
                reset_input_state();
                return true;
            }
        } else if (currentScreen == Types::Screen::REMOVE_RECORD) {
            // UUID input box
            SDL_Rect uuidRect = {
                get_padding() * 2,
                get_padding() * 4 + 90,
                get_window_width() - get_padding() * 4,
                40
            };

            // Delete button
            SDL_Rect deleteButtonRect = {
                get_window_width() - get_padding() * 2 - get_button_width(),
                get_padding() * 4 + 90 + 60,
                get_button_width(),
                get_button_height()
            };

            // Check UUID input box click
            if (x >= uuidRect.x && 
                x < uuidRect.x + uuidRect.w && 
                y >= uuidRect.y && 
                y < uuidRect.y + uuidRect.h) {
                isUUIDInputActive = true;
                return true;
            }

            // Check Delete button click
            if (x >= deleteButtonRect.x && 
                x < deleteButtonRect.x + deleteButtonRect.w && 
                y >= deleteButtonRect.y && 
                y < deleteButtonRect.y + deleteButtonRect.h) {
                if (!currentUUID.empty()) {
                    Database_service::remove_record(currentUUID);
                    std::cout << "Deleted record with UUID: " << currentUUID << std::endl;
                    currentUUID.clear();
                }
                    return true;
                }
            } else if (currentScreen == Types::Screen::SEARCH_RECORD) {
                // UUID input box
                SDL_Rect uuidRect = {
                    get_padding() * 2,
                    get_padding() * 4 + 60,
                    get_window_width() - get_padding() * 4,
                    40
                };

                // Search button
                SDL_Rect searchButtonRect = {
                    get_window_width() - get_padding() * 2 - get_button_width(),
                    get_padding() * 4 + 90 + 60,
                    get_button_width(),
                    get_button_height()
                };

                // Check Search button click
                if (x >= searchButtonRect.x && 
                    x < searchButtonRect.x + searchButtonRect.w && 
                    y >= searchButtonRect.y && 
                    y < searchButtonRect.y + searchButtonRect.h) {
                    if (!currentUUID.empty()) {
                        Database::index_file.seekp(0, std::ios::beg);
                        Database_service::search_database(currentUUID);
                        std::cout << "Searched record with UUID: " << currentUUID << std::endl;
                        currentUUID.clear();
                    }
                        return true;
                    }

                // Check UUID input box click
                if (x >= uuidRect.x && 
                    x < uuidRect.x + uuidRect.w && 
                    y >= uuidRect.y && 
                    y < uuidRect.y + uuidRect.h) {
                    isUUIDInputActive = true;
                    return true;
                }

            }

            return true;
    }

    void handle_keyboard(SDL_Event& event) {
        if (currentScreen == Types::Screen::ADD_RECORD || 
        currentScreen == Types::Screen::REMOVE_RECORD || currentScreen == Types::Screen::SEARCH_RECORD) {
            if (event.type == SDL_KEYDOWN) {
                if (isUUIDInputActive) {
                    // UUID input handling
                    if (event.key.keysym.sym == SDLK_BACKSPACE && !currentUUID.empty()) {
                        currentUUID.pop_back();
                    } else if (event.key.keysym.sym != SDLK_RETURN) {
                        // Ignore RETURN and add other characters
                        currentUUID += static_cast<char>(event.key.keysym.sym);
                    }
                } else if (isValueInputActive) {
                    // Value input handling
                    if (event.key.keysym.sym == SDLK_BACKSPACE && !currentInputValue.empty()) {
                        currentInputValue.pop_back();
                    } else if (event.key.keysym.sym != SDLK_RETURN) {
                        // Ignore RETURN and add other characters
                        currentInputValue += static_cast<char>(event.key.keysym.sym);
                    }
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

    std::string get_new_record_uuid() {
        return newRecordUUID;
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

    std::string get_current_UUID() { return currentUUID; }
    std::vector<std::string> get_current_values() { return currentValues; }
    std::string get_current_input_value() { return currentInputValue; }
    bool is_uuid_input_active() { return isUUIDInputActive; }
    bool is_value_input_active() { return isValueInputActive; }


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